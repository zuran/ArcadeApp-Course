#pragma once

#include "Vec2D.h"
#include <vector>

class Shape
{
public:
	virtual Vec2D GetCenterPoint() const = 0;
	virtual ~Shape() {}

	inline virtual std::vector<Vec2D> GetPoints() const {return mPoints;}
	void MoveBy(const Vec2D& deltaOffset);
	virtual void MoveTo(const Vec2D& position) = 0;

protected:
	std::vector<Vec2D> mPoints;
};

