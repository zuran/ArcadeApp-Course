#include "Triangle.h"
#include <cmath>
#include "Utils.h"

Triangle::Triangle() : Triangle(Vec2D::Zero, Vec2D::Zero, Vec2D::Zero)
{
}

Triangle::Triangle(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2)
{
	mPoints.push_back(p0);
	mPoints.push_back(p1);
	mPoints.push_back(p2);
}

void Triangle::MoveTo(const Vec2D& position)
{
	Vec2D center = GetCenterPoint();
	// Get point offset from center
	Vec2D p0o = mPoints[0] - center;
	Vec2D p1o = mPoints[1] - center;
	Vec2D p2o = mPoints[2] - center;

	// Add point offset to new position
	mPoints[0] = p0o + position;
	mPoints[1] = p1o + position;
	mPoints[2] = p2o + position;
}

Vec2D Triangle::GetCenterPoint() const
{
	return Vec2D((mPoints[0].GetX() + mPoints[1].GetX() + mPoints[2].GetX()) / 3.0f,
			(mPoints[0].GetY() + mPoints[1].GetY() + mPoints[2].GetY())/3.0f);
}

float Triangle::Area() const
{
	return Area(GetP0(), GetP1(), GetP2());
}

bool Triangle::ContainsPoint(const Vec2D& p) const
{
	float thisArea = Area();
	float a0 = Area(p, GetP0(), GetP1());
	float a1 = Area(p, GetP1(), GetP2());
	float a2 = Area(p, GetP2(), GetP0());

	return IsEqual(thisArea, (a0 + a1 + a2));
}

float Triangle::Area(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2) const
{
	return fabsf((p0.GetX() * (p1.GetY() - p2.GetY()) + 
				p1.GetX() * (p2.GetY() - p0.GetY()) + 
				p2.GetX() * (p0.GetY() - p1.GetY())) / 2.0f);
}
