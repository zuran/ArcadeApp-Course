#include "PacmanGame.h"
#include "PacmanGameUtils.h"
#include "App.h"

namespace {
	const std::string SCORE_STR = "Score   ";
	const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
	const int MAX_NUM_LIVES = 3;
}

void PacmanGame::Init(GameController& controller)
{
	mPacmanSpriteSheet.Load("PacmanSprites");
	mPacman.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Pacman_animations.txt",
		Vec2D::Zero, PACMAN_MOVEMENT_SPEED, false);
	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt", &mPacmanSpriteSheet, &mPacman);

	ResetGame();

	ButtonAction leftAction;
	leftAction.key = GameController::LeftKey();
	leftAction.action = [this](int dt, InputState state) {
		HandleGameControllerState(dt, state, PACMAN_MOVEMENT_LEFT);
	};

	ButtonAction rightAction;
	rightAction.key = GameController::RightKey();
	rightAction.action = [this](int dt, InputState state) {
		HandleGameControllerState(dt, state, PACMAN_MOVEMENT_RIGHT);
	};

	ButtonAction upAction;
	upAction.key = GameController::UpKey();
	upAction.action = [this](int dt, InputState state) {
		HandleGameControllerState(dt, state, PACMAN_MOVEMENT_UP);
	};

	ButtonAction downAction;
	downAction.key = GameController::DownKey();
	downAction.action = [this](int dt, InputState state) {
		HandleGameControllerState(dt, state, PACMAN_MOVEMENT_DOWN);
	};

	controller.AddInputActionForKey(leftAction);
	controller.AddInputActionForKey(rightAction);
	controller.AddInputActionForKey(upAction);
	controller.AddInputActionForKey(downAction);
}

void PacmanGame::Update(uint32_t dt)
{
	UpdatePacmanMovement();
	mPacman.Update(dt);
	mLevel.Update(dt);

	if (mLevel.IsLevelOver())
	{
		mLevel.IncreaseLevel();
	}
}

void PacmanGame::Draw(Screen& screen)
{
	mLevel.Draw(screen);
	mPacman.Draw(screen);

	// Draw Score
	{
		Vec2D levelOffset = mLevel.GetLayoutOffset();
		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());
		const auto& font = App::Singleton().GetFont();
		Vec2D textDrawPos;
		std::string scoreStr = std::to_string(mPacman.GetScore());
		textDrawPos = font.GetDrawPosition(SCORE_STR + scoreStr, highScoreRect, BFXA_CENTER, BFYA_CENTER);

		screen.Draw(font, SCORE_STR + scoreStr, textDrawPos);
	}

	DrawLives(screen);
}

const std::string& PacmanGame::GetName() const
{
	static std::string name = "!Pacman";
	return name;
}

void PacmanGame::ResetGame()
{
	mNumLives = MAX_NUM_LIVES;
	mPressedDirection = PACMAN_MOVEMENT_NONE;
	mPacman.ResetScore();
	mLevel.ResetToFirstLevel();
}

void PacmanGame::UpdatePacmanMovement()
{
	if (mPressedDirection != PACMAN_MOVEMENT_NONE)
	{
		if (!mLevel.WillCollide(mPacman.GetBoundingBox(), mPressedDirection))
		{
			mPacman.SetMovementDirection(mPressedDirection);
		}
	}
}

void PacmanGame::HandleGameControllerState(int dt, InputState state, PacmanMovement dir)
{
	if (GameController::IsPressed(state))
	{
		mPressedDirection = dir;
	}
	else if (GameController::IsReleased(state) && mPressedDirection == dir)
	{
		mPressedDirection = PACMAN_MOVEMENT_NONE;
	}
}

void PacmanGame::ResetLevel()
{
}

void PacmanGame::DrawLives(Screen& screen)
{
	const int X_PAD = 1;
	auto sprite = mPacmanSpriteSheet.GetSprite(PACMAN_LIFE_SPRITE_NAME);
	int xPos = X_PAD;
	for (int i = 0; i < mNumLives; ++i)
	{
		screen.Draw(mPacmanSpriteSheet, PACMAN_LIFE_SPRITE_NAME, Vec2D(xPos, App::Singleton().Height() - sprite.height));
		xPos += sprite.width + X_PAD;
	}
}
