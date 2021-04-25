#pragma once

#include "Actor.h"

class Screen;
class SpriteSheet;

class Pacman : public Actor
{
public:
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationPath,
		const Vec2D& initialPos, int movementSpeed, bool updateSpriteOnMovement,
		const Color& spriteColor = Color::White());
	virtual void Update(int dt) override;
	virtual void SetMovementDirection(PacmanMovement dir) override;

	void ResetToFirstAnimation();
	void EatenByGhost();
	void ResetScore();
	void AteItem(int value);
	void AteGhost(int value);

	inline void ResetGhostEatenMultiplier() { mGhostMultiplier = 1; }
	inline int GetScore() const { return mScore; }

private:
	void AddToScore(int value);

	int mScore;
	int mGhostMultiplier;
	bool mIsDying;
};