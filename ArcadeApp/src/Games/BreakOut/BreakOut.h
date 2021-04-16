#pragma once

#include "Game.h"
#include "Paddle.h"
#include "Ball.h"
#include "LevelBoundary.h"
#include "BreakOutGameLevel.h"
#include <vector>

enum BreakOutGameState
{
	IN_PLAY = 0,
	IN_SERVE,
	IN_GAME_OVER
};

class BreakOut : public Game
{
public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

private:
	const int NUM_LIVES = 3;

	void ResetGame(size_t toLevel = 0);

	inline BreakoutGameLevel& GetCurrentLevel() { return mLevels[mCurrentLevel]; }
	void SetToServeState();

	bool IsBallPassedCutoffY() const;
	void ReduceLifeByOne();
	inline bool IsGameOver() const { return mLives < 0; }

	const Vec2D INITIAL_BALL_VELOCITY = Vec2D(100, -100);
	const float INITIAL_BALL_SPEED = 100;

	Paddle mPaddle;
	Ball mBall;
	LevelBoundary mLevelBoundary;
	std::vector<BreakoutGameLevel> mLevels;
	size_t mCurrentLevel;
	BreakOutGameState mGameState;
	int mLives;
	float mYCutoff;
};