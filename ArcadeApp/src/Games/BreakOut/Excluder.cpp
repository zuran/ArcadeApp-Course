#include "Excluder.h"
#include "Utils.h"
#include <cmath>
#include <cassert>

void Excluder::Init(const AARectangle& rect, bool reverseNormals)
{
	mAARect = rect;
	mReverseNormals = reverseNormals;
	SetupEdges();
}

bool Excluder::HasCollided(const AARectangle& rect, BoundaryEdge& edge) const
{
	if (mAARect.Intersects(rect))
	{
		float yMin = mAARect.GetTopLeftPoint().GetY() >= (rect.GetTopLeftPoint().GetY()) ? 
			mAARect.GetTopLeftPoint().GetY() : rect.GetTopLeftPoint().GetY();
		float yMax = mAARect.GetBottomRightPoint().GetY() <= (rect.GetBottomRightPoint().GetY()) ?
			mAARect.GetBottomRightPoint().GetY() : rect.GetBottomRightPoint().GetY();
		float ySize = yMax - yMin;

		float xMin = mAARect.GetTopLeftPoint().GetX() >= (rect.GetTopLeftPoint().GetX()) ?
			mAARect.GetTopLeftPoint().GetX() : rect.GetTopLeftPoint().GetX();
		float xMax = mAARect.GetBottomRightPoint().GetX() <= (rect.GetBottomRightPoint().GetX()) ?
			mAARect.GetBottomRightPoint().GetX() : rect.GetBottomRightPoint().GetX();
		float xSize = xMax - xMin;

		if (xSize > ySize)
		{
			if (rect.GetCenterPoint().GetY() > mAARect.GetCenterPoint().GetY())
			{
				edge = mEdges[BOTTOM_EDGE];
			}
			else
			{
				edge = mEdges[TOP_EDGE];
			}
		}
		else {
			if (rect.GetCenterPoint().GetX() > mAARect.GetCenterPoint().GetX())
			{
				edge = mEdges[RIGHT_EDGE];
			}
			else
			{
				edge = mEdges[LEFT_EDGE];
			}
		}
		return true;
	}
	return false;
}

Vec2D Excluder::GetCollisionOffset(const AARectangle& rect) const
{
	BoundaryEdge edge;
	Vec2D offset = Vec2D::Zero;
	if (HasCollided(rect, edge))
	{
		float yMin = mAARect.GetTopLeftPoint().GetY() >= (rect.GetTopLeftPoint().GetY()) ?
			mAARect.GetTopLeftPoint().GetY() : rect.GetTopLeftPoint().GetY();
		float yMax = mAARect.GetBottomRightPoint().GetY() <= (rect.GetBottomRightPoint().GetY()) ?
			mAARect.GetBottomRightPoint().GetY() : rect.GetBottomRightPoint().GetY();
		float ySize = yMax - yMin;

		float xMin = mAARect.GetTopLeftPoint().GetX() >= (rect.GetTopLeftPoint().GetX()) ?
			mAARect.GetTopLeftPoint().GetX() : rect.GetTopLeftPoint().GetX();
		float xMax = mAARect.GetBottomRightPoint().GetX() <= (rect.GetBottomRightPoint().GetX()) ?
			mAARect.GetBottomRightPoint().GetX() : rect.GetBottomRightPoint().GetX();
		float xSize = xMax - xMin;

		if (!IsEqual(edge.normal.GetY(), 0))
		{
			offset = (ySize + 1) * edge.normal;
		}
		else
		{
			offset = (xSize + 1) * edge.normal;
		}
	}
	return offset;
}

void Excluder::MoveBy(const Vec2D& delta)
{
	mAARect.MoveBy(delta);
	SetupEdges();
}

void Excluder::MoveTo(const Vec2D& point)
{
	mAARect.MoveTo(point);
	SetupEdges();
}

const BoundaryEdge& Excluder::GetEdge(EdgeType edge) const
{
	assert(edge != NUM_EDGES);
	return mEdges[edge];
}

void Excluder::SetupEdges()
{
	auto p = mAARect.GetPoints();
	mEdges[TOP_EDGE].edge = {p[0], p[1]};
	mEdges[TOP_EDGE].normal = UP_DIR;

	mEdges[RIGHT_EDGE].edge = {p[1], p[2]};
	mEdges[RIGHT_EDGE].normal = RIGHT_DIR;

	mEdges[BOTTOM_EDGE].edge = {p[2], p[3]};
	mEdges[BOTTOM_EDGE].normal = DOWN_DIR;

	mEdges[LEFT_EDGE].edge = {p[3], p[0]};
	mEdges[LEFT_EDGE].normal = LEFT_DIR;

	if (mReverseNormals)
	{
		for (auto& edge : mEdges)
		{
			edge.normal = -edge.normal;
		}
	}
}
