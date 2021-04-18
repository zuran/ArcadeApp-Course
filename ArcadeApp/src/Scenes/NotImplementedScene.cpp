#include "NotImplementedScene.h"
#include "App.h"
#include "AARectangle.h"
#include "InputAction.h"

void NotImplementedScene::Init()
{
	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			App::Singleton().PopScene();
		}
	};
	mGameController.AddInputActionForKey(backAction);
}

void NotImplementedScene::Update(uint32_t dt)
{
}

void NotImplementedScene::Draw(Screen& theScreen)
{
	const BitmapFont& font = App::Singleton().GetFont();
	AARectangle rect = {Vec2D::Zero, App::Singleton().Width(), App::Singleton().Height()};
	Vec2D textDrawPos = font.GetDrawPosition(GetSceneName(), rect, BFXA_CENTER, BFYA_CENTER);
	theScreen.Draw(font, GetSceneName(), textDrawPos, Color::Red());
}

const std::string& NotImplementedScene::GetSceneName() const
{
	static std::string name = "Not Implemented!";
	return name;
}
