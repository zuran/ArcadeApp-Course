#pragma once

#include "Ghost.h"
#include <random>
#include "PacmanGameUtils.h"

class PacmanLevel;
class Screen;
class Pacman;

enum GhostAIState
{
	GHOST_AI_STATE_START = 0,
	GHOST_AI_STATE_IN_PEN,
	GHOST_AI_STATE_EXIT_PEN,
	GHOST_AI_STATE_CHASE,
	GHOST_AI_STATE_SCATTER,
	GHOST_AI_STATE_GO_TO_PEN
};

class GhostAI
{
public:
	GhostAI();
	void Init(Ghost& ghost, int lookAheadDistance, const Vec2D& scatterTarget,
		GhostName name);

	PacmanMovement Update(int dt, const Pacman& pacman, 
		const PacmanLevel& level, const std::vector<Ghost>& ghosts);
	void Draw(Screen& screen);

private:
	void SetState(GhostAIState state);
	void ChangeTarget(const Vec2D& target);
	Vec2D GetChaseTarget(int dt, const Pacman& pacman, const PacmanLevel& level,
		const std::vector<Ghost>& ghosts);

	Vec2D mScatterTarget;
	Vec2D mTarget;
	int mLookAheadDistance;
	Ghost* mnoptrGhost;
	GhostName mName;
	std::default_random_engine mAIRandomGenerator;
	GhostAIState mState;
	GhostAIState mLastState;
	uint64_t mTimer;
};