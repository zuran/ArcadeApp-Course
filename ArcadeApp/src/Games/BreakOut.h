#pragma once

#include "Game.h"

class BreakOut : public Game
{
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual std::string GetName() const override;
};