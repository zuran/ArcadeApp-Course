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
	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt", &mPacmanSpriteSheet);

	SetupGhosts();
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
	
	for (int i = 0; i < NUM_GHOSTS; ++i)
	{
		if (static_cast<GhostName>(i) == BLINKY)
		{
			GhostAI& ghostAI = mGhostAIs[i];
			auto dir = ghostAI.Update(dt, mPacman, mLevel, mGhosts);
			if (mGhosts[i].CanChangeDirection() && dir != mGhosts[i].GetMovementDirection())
			{
				mGhosts[i].SetMovementDirection(dir);
			}
		}

		mGhosts[i].Update(dt);
	}
	
	mLevel.Update(dt, mPacman, mGhosts, mGhostAIs);

	if (mLevel.IsLevelOver())
	{
		mLevel.IncreaseLevel();
		ResetLevel();
	}
}

void PacmanGame::Draw(Screen& screen)
{
	mLevel.Draw(screen);
	mPacman.Draw(screen);

	for (auto& ghost : mGhosts)
	{
		ghost.Draw(screen);
	}

	for (auto& ghostAI : mGhostAIs)
	{
		ghostAI.Draw(screen);
	}

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
	ResetLevel();
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
	mPacman.MoveTo(mLevel.GetPacmanSpawnLocation());
	mPacman.ResetToFirstAnimation();
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

void PacmanGame::SetupGhosts()
{
	const Vec2D BLINKY_SCATTER_POS = Vec2D(App::Singleton().Width() - 24, 0);
	const Vec2D INKY_SCATTER_POS = Vec2D(App::Singleton().Width(), App::Singleton().Height());
	const Vec2D PINKY_SCATTER_POS = Vec2D(24, 0);
	const Vec2D CLYDE_SCATTER_POS = Vec2D(0, App::Singleton().Height());

	mGhosts.resize(NUM_GHOSTS);
	mGhostAIs.resize(NUM_GHOSTS);

	Ghost blinky;
	blinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", 
		mLevel.GetGhostSpawnPoints()[BLINKY], GHOST_MOVEMENT_SPEED, true, Color::Red());
	blinky.SetMovementDirection(PACMAN_MOVEMENT_LEFT);
	mGhosts[BLINKY] = blinky;

	auto blinkyAI = GhostAI();
	blinkyAI.Init(mGhosts[BLINKY], blinky.GetBoundingBox().GetWidth(), BLINKY_SCATTER_POS, 
		mLevel.GetGhostSpawnPoints()[BLINKY], mLevel.GetGhostSpawnPoints()[BLINKY], BLINKY);

	mGhostAIs[BLINKY] = blinkyAI;

	Ghost inky;
	inky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt",
		mLevel.GetGhostSpawnPoints()[INKY], GHOST_MOVEMENT_SPEED, true, Color::Pink());
	inky.SetMovementDirection(PACMAN_MOVEMENT_DOWN);
	mGhosts[INKY] = inky;

	auto inkyAI = GhostAI();
	inkyAI.Init(mGhosts[INKY], inky.GetBoundingBox().GetWidth(), INKY_SCATTER_POS,
		mLevel.GetGhostSpawnPoints()[INKY], mLevel.GetGhostSpawnPoints()[BLINKY], INKY);
	mGhostAIs[INKY] = inkyAI;

	Ghost pinky;
	pinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt",
		mLevel.GetGhostSpawnPoints()[PINKY], GHOST_MOVEMENT_SPEED, true, Color::Cyan());
	pinky.SetMovementDirection(PACMAN_MOVEMENT_UP);
	mGhosts[PINKY] = pinky;

	auto pinkyAI = GhostAI();
	pinkyAI.Init(mGhosts[PINKY], pinky.GetBoundingBox().GetWidth(), PINKY_SCATTER_POS,
		mLevel.GetGhostSpawnPoints()[BLINKY], mLevel.GetGhostSpawnPoints()[BLINKY], PINKY);
	mGhostAIs[PINKY] = pinkyAI;

	Ghost clyde;
	clyde.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt",
		mLevel.GetGhostSpawnPoints()[CLYDE], GHOST_MOVEMENT_SPEED, true, Color::Orange());
	clyde.SetMovementDirection(PACMAN_MOVEMENT_UP);
	mGhosts[CLYDE] = clyde;

	auto clydeAI = GhostAI();
	clydeAI.Init(mGhosts[CLYDE], clyde.GetBoundingBox().GetWidth(), CLYDE_SCATTER_POS,
		mLevel.GetGhostSpawnPoints()[BLINKY], mLevel.GetGhostSpawnPoints()[BLINKY], CLYDE);
	mGhostAIs[CLYDE] = clydeAI;
}
