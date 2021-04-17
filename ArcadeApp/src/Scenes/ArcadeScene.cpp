#include "ArcadeScene.h"
#include "Screen.h"
#include "AARectangle.h"
#include "Triangle.h"
#include "Circle.h"
#include "Color.h"
#include "GameController.h"
#include <iostream>
#include "App.h"

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
	const BitmapFont& font = App::Singleton().GetFont();
	AARectangle rect = { Vec2D::Zero, App::Singleton().Width(), App::Singleton().Height() };
	Vec2D drawPos = font.GetDrawPosition(GetSceneName(), rect, BFXA_CENTER, BFYA_CENTER);
	theScreen.Draw(font, GetSceneName(), drawPos, Color::Green());
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade";
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
