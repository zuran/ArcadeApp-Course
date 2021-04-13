#include "ArcadeScene.h"
#include "Screen.h"
#include "AARectangle.h"
#include "Triangle.h"
#include "Circle.h"
#include "Color.h"
#include "GameController.h"
#include <iostream>

ArcadeScene::ArcadeScene()
{
}

void ArcadeScene::Init()
{
	ButtonAction action;
	action.key = GameController::ActionKey();
	action.action = [](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			std::cout << "Action button was pressed" << std::endl;
		}
	};
	mGameController.AddInputActionForKey(action);

	MouseButtonAction mouseAction;
	mouseAction.mouseButton = GameController::LeftMouseButton();
	mouseAction.mouseInputAction = [](InputState state, const MousePosition& position)
	{
		if (GameController::IsPressed(state))
		{
			std::cout << "Left mouse clicked at " << position.xPos << "," << position.yPos << std::endl;
		}
	};

	mGameController.AddMouseButtonAction(mouseAction);
	mGameController.SetMouseMovedAction([](const MousePosition& mousePosition) {
		std::cout << "Position (" << mousePosition.xPos << "," << mousePosition.yPos << ")" << std::endl;
	});
}

void ArcadeScene::Update(uint32_t dt)
{
}

void ArcadeScene::Draw(Screen& theScreen)
{
	Triangle triangle = { Vec2D(60, 10), Vec2D(10, 110), Vec2D(110, 110) };
	AARectangle rect = { Vec2D(115, 115), 50, 30 };
	Circle circle = { Vec2D(160, 160), 45 };

	theScreen.Draw(triangle, Color::Red(), true, Color::Red());
	theScreen.Draw(rect, Color::Blue(), true, Color::Blue());
	theScreen.Draw(circle, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade Scene";
	return sceneName;
}

std::unique_ptr<Scene> ArcadeScene::GetScene(eGame game)
{
	switch (game)
	{
		case eGame::TETRIS:
		{

		}
		break;
		case eGame::BREAK_OUT:
		{

		}
		break;
		case eGame::ASTEROIDS:
		{

		}
		break;
		case eGame::PACMAN:
		{

		}
		break;
		default:
		{

		}
		break;
	}
	return nullptr;
}
