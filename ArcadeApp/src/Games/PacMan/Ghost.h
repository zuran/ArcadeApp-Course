#pragma once

#include "Actor.h"
#include "Color.h"
#include "PacmanGameUtils.h"
#include "AARectangle.h"

enum GhostName
{
	BLINKY = 0,
	PINKY,
	INKY,
	CLYDE,
	NUM_GHOSTS
};

enum GhostState
{
	GHOST_STATE_ALIVE = 0,
	GHOST_STATE_VULNERABLE,
	GHOST_STATE_VULNERABLE_ENDING,
	GHOST_STATE_DEAD
};

class Ghost : public Actor
{
public:
	static const int VULNERABILITY_TIME = 6000;
	static const int VULNERABILITY_ENDING_TIME = 4000;

	Ghost();
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, 
		const Vec2D& initialPos, int movementSpeed, bool updateSpriteOnMovement, const Color& spriteColor = Color::White()) override;
	virtual void Update(int dt) override;

	void SetStateToVulnerable();
	virtual void SetMovementDirection(PacmanMovement dir) override;
	virtual void Stop() override;

	void EatenByPacman();
	void ResetToFirstPosition();

	inline bool IsDead() const { return mState == GHOST_STATE_DEAD; }
	inline bool IsVulnerable() const { return mState == GHOST_STATE_VULNERABLE || mState == GHOST_STATE_VULNERABLE_ENDING; }
	inline bool IsAlive() const { return mState == GHOST_STATE_ALIVE; }
	inline int GetPoints() const { return mPoints; }
	inline void LockCanChangeDirection() { mCanChangeDirection = false; }

private:
	void SetGhostState(GhostState state);

	int mVulnerabilityTimer;
	int mPoints;
	GhostState mState;
	bool mCanChangeDirection;
	Vec2D mInitialPos;
};