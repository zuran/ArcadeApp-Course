#include "PacmanGame.h"
#include "PacmanGameUtils.h"
#include "App.h"

void PacmanGame::Init(GameController& controller)
{
	mPacmanSpriteSheet.Load("PacmanSprites");

	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt");

	mPacman.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Pacman_animations.txt", 
		Vec2D::Zero, PACMAN_MOVEMENT_SPEED, false);
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
	mLevel.Update(dt);
	UpdatePacmanMovement();
	mPacman.Update(dt);
}

void PacmanGame::Draw(Screen& screen)
{
	mLevel.Draw(screen);
	mPacman.Draw(screen);
}

const std::string& PacmanGame::GetName() const
{
	static std::string name = "!Pacman";
	return name;
}

void PacmanGame::ResetGame()
{
	mPressedDirection = PACMAN_MOVEMENT_NONE;
	mPacman.ResetScore();
}

void PacmanGame::UpdatePacmanMovement()
{
	if (mPressedDirection != PACMAN_MOVEMENT_NONE)
	{
		mPacman.SetMovementDirection(mPressedDirection);
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
