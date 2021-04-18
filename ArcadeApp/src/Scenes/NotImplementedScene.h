#pragma once

#include "Scene.h"

#include <string>

class NotImplementedScene : public Scene
{
	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& theScreen) override;
	virtual const std::string& GetSceneName() const override;
};