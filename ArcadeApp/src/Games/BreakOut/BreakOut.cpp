#include "BreakOut.h"
#include "GameController.h"
#include "App.h"
#include <iostream>

void BreakOut::Init(GameController& controller)
{
	controller.ClearAll();

	ButtonAction leftKeyAction;
	leftKeyAction.key = GameController::LeftKey();
	leftKeyAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			mPaddle.SetMovementDirection(PaddleDirection::LEFT);
		}
		else
		{
			mPaddle.UnsetMovementDirection(PaddleDirection::LEFT);
		}
	};
	controller.AddInputActionForKey(leftKeyAction);

	ButtonAction rightKeyAction;
	rightKeyAction.key = GameController::RightKey();
	rightKeyAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			mPaddle.SetMovementDirection(PaddleDirection::RIGHT);
		}
		else
		{
			mPaddle.UnsetMovementDirection(PaddleDirection::RIGHT);
		}
	};
	controller.AddInputActionForKey(rightKeyAction);

	ResetGame();
}

void BreakOut::Update(uint32_t dt)
{
	mPaddle.Update(dt);
}

void BreakOut::Draw(Screen& screen)
{
	mPaddle.Draw(screen);
}

std::string BreakOut::GetName() const
{
	return "Break Out!";
}

void BreakOut::ResetGame()
{
	AARectangle paddleRect = {Vec2D(App::Singleton().Width()/2 - Paddle::PADDLE_WIDTH/2, 
		App::Singleton().Height() - 3 * Paddle::PADDLE_HEIGHT),
		Paddle::PADDLE_WIDTH, Paddle::PADDLE_HEIGHT};
	AARectangle levelBoundary = {Vec2D::Zero, App::Singleton().Width(), App::Singleton().Height()};
	mPaddle.Init(paddleRect, levelBoundary);
}
