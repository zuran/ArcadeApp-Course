#pragma once

#include "Game.h"
#include "PacmanLevel.h"
#include "Pacman.h"
#include "SpriteSheet.h"
#include "InputAction.h"
#include "Ghost.h"
#include "GhostAI.h"

enum PacmanGameState
{
	LEVEL_STARTING = 0,
	PLAY_GAME,
	LOST_LIFE,
	GAME_OVER
};

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
	void ResetLevel();
	void DrawLives(Screen& screen);
	void SetupGhosts();

	PacmanLevel mLevel;
	PacmanMovement mPressedDirection;
	SpriteSheet mPacmanSpriteSheet;
	Pacman mPacman;
	int mNumLives;
	std::vector<Ghost> mGhosts;
	std::vector<GhostAI> mGhostAIs;
	int mReleaseGhostTimer;
	PacmanGameState mGameState;
	int mLevelStartingTimer;
	AARectangle mStringRect;
};