#pragma once

#include "AnimatedSprite.h"
#include "Vec2D.h"
#include "PacmanGameUtils.h"
#include "AARectangle.h"
#include "Color.h"
#include <string>

class Screen;
class SpriteSheet;
class Actor
{
public:
	virtual ~Actor() {}
	virtual void Init(const SpriteSheet& spriteSheet, 
		const std::string& animationsPath, const Vec2D& initialPos,
		int movementSpeed, bool updateSpriteOnMovement, 
		const Color& spriteColor = Color::White());
	virtual void Update(int dt);
	virtual void Draw(Screen& screen);
	virtual void Stop();
	AARectangle GetEatingBoundingBox() const;

	inline bool IsFinishedAnimation() const { return mSprite.IsFinishedPlayingAnimation(); }
	inline const AARectangle GetBoundingBox() const { return mSprite.GetBoundingBox(); }
	inline void MoveBy(const Vec2D& delta) { mSprite.MoveBy(delta); }
	inline void MoveTo(const Vec2D& pos) { mSprite.SetPosition(pos); }
	inline Vec2D GetPosition() const { return mSprite.GetPosition(); }
	inline PacmanMovement GetMovementDirection() const { return mMovementDirection; }
	virtual inline void SetMovementDirection(PacmanMovement dir) { mMovementDirection = dir; }
	inline const Color& GetSpriteColor() const { return mSprite.GetColor(); }
	
protected:
	void SetAnimation(const std::string& animationName, bool looped);
	inline void ResetDelta() { mDelta = Vec2D::Zero; }
	inline void SetMovementSpeed(int movementSpeed) { mMovementSpeed = movementSpeed; }

	AnimatedSprite mSprite;

private:
	Vec2D mDelta;
	PacmanMovement mMovementDirection;
	int mMovementSpeed;
	bool mUpdateSpriteOnUpdate;
};