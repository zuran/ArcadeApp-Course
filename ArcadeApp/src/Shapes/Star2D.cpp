#include "Star2D.h"
#include "Utils.h"
#include <cmath>
#include <random>

Star2D::Star2D() : Star2D(Vec2D::Zero, 0.f, 0.f)
{
}

Star2D::Star2D(float x, float y, float angle, float radius) : Star2D(Vec2D(x, y), angle, radius)
{
}

Star2D::Star2D(const Vec2D& origin, float angle, float radius) : mOrigin(origin), mAngle(angle), mRadius(radius)
{
}

bool Star2D::operator==(const Star2D& star) const
{
	return mOrigin == star.GetOrigin() && 
		fabsf(mAngle - star.GetAngle()) < EPSILON && 
		fabsf(mRadius - star.GetRadius() < EPSILON);
}

Star2D Star2D::GetRandomStar(const Vec2D& origin, float minSize, float maxSize)
{
	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_real_distribution<float> uniformDist(minSize, maxSize);

	float randomRadius = uniformDist(e1);
	return Star2D(origin, 0.f, randomRadius);
}

