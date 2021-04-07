#pragma once

#include "Vec2D.h"

class Star2D
{
public:
	Star2D();
	Star2D(float x, float y, float angle, float radius);
	Star2D(const Vec2D& origin, float angle, float radius);

	inline const Vec2D& GetOrigin() const {return mOrigin;}
	inline float GetAngle() const {return mAngle;}
	inline float GetRadius() const {return mRadius;}

	inline void SetOrigin(const Vec2D& origin) {mOrigin = origin;}
	inline void SetOrigin(float x, float y) {SetOrigin(Vec2D(x, y));}
	inline void SetAngle(float radians) {mAngle = radians;}
	inline void SetRadius(float radius) {mRadius = radius;}

	bool operator==(const Star2D& star) const;

private:
	Vec2D mOrigin;
	float mAngle;
	float mRadius;
};