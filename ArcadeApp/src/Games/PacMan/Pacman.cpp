#include "Pacman.h"
#include "Utils.h"

void Pacman::Init(const SpriteSheet& spriteSheet, const std::string& animationPath, const Vec2D& initialPos, int movementSpeed, bool updateSpriteOnMovement, const Color& spriteColor)
{
	Actor::Init(spriteSheet, animationPath, initialPos, movementSpeed, false, spriteColor);
	ResetToFirstAnimation();
	ResetScore();
	mIsDying = false;
	ResetGhostEatenMultiplier();
}

void Pacman::Update(int dt)
{
	if (mIsDying)
	{
		mSprite.Update(dt);
		mIsDying = !IsFinishedAnimation();
		return;
	}

	Actor::Update(dt);
}

void Pacman::SetMovementDirection(PacmanMovement dir)
{
	PacmanMovement currentDir = GetMovementDirection();
	if (dir == PACMAN_MOVEMENT_LEFT && currentDir != PACMAN_MOVEMENT_LEFT)
	{
		SetAnimation("move_left", true);
		ResetDelta();
	}
	else if (dir == PACMAN_MOVEMENT_RIGHT && currentDir != PACMAN_MOVEMENT_RIGHT)
	{
		SetAnimation("move_right", true);
		ResetDelta();
	}
	else if (dir == PACMAN_MOVEMENT_DOWN && currentDir != PACMAN_MOVEMENT_DOWN)
	{
		SetAnimation("move_down", true);
		ResetDelta();
	}
	else if (dir == PACMAN_MOVEMENT_UP && currentDir != PACMAN_MOVEMENT_UP)
	{
		SetAnimation("move_up", true);
		ResetDelta();
	}

	Actor::SetMovementDirection(dir);
}

void Pacman::ResetToFirstAnimation()
{
	SetAnimation("move_left", true);
	Stop();
}

void Pacman::EatenByGhost()
{
	SetAnimation("death", false);
	mIsDying = true;
	ResetGhostEatenMultiplier();
}

void Pacman::ResetScore()
{
	mScore = 0;
}

void Pacman::AteItem(int value)
{
	AddToScore(value);
}

void Pacman::AteGhost(int value)
{
	AddToScore(value * mGhostMultiplier);
	mGhostMultiplier *= 2;
}

void Pacman::AddToScore(int value)
{
	mScore += value;
}
