#pragma once

#include "Game.h"
#include "PacmanLevel.h"
#include "Pacman.h"
#include "SpriteSheet.h"
#include "InputAction.h"

class PacmanGame : public Game
{
public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

private:
	void ResetGame();
	void UpdatePacmanMovement();
	void HandleGameControllerState(int dt, InputState state, PacmanMovement dir);

	PacmanLevel mLevel;
	PacmanMovement mPressedDirection;
	SpriteSheet mPacmanSpriteSheet;
	Pacman mPacman;
};