#include "GhostAI.h"
#include "Screen.h"
#include "PacmanLevel.h"
#include "Pacman.h"
#include <cassert>
#include <algorithm>
#include "Circle.h"

namespace
{
	const int PEN_WAIT_DURATION = 5000;
	const int SCATTER_MODE_DURATION = 10000;
}

GhostAI::GhostAI() : mnoptrGhost(nullptr)
{
}

void GhostAI::Init(Ghost& ghost, int lookAheadDistance, const Vec2D& scatterTarget,
	const Vec2D& ghostPenTarget, const Vec2D& ghostExitPenPos, GhostName name)
{
	mGhostPenTarget = ghostPenTarget;
	mGhostExitPenPosition = ghostExitPenPos;
	mScatterTarget = scatterTarget;
	mLookAheadDistance = lookAheadDistance;
	mnoptrGhost = &ghost;
	mName = name;
	std::random_device r;
	mAIRandomGenerator.seed(r());
	mTimer = 0;
	SetState(GHOST_AI_STATE_SCATTER);
	mLastState = GHOST_AI_STATE_SCATTER;
}

PacmanMovement GhostAI::Update(int dt, const Pacman& pacman, 
	const PacmanLevel& level, const std::vector<Ghost>& ghosts)
{
	if (mnoptrGhost)
	{
		if (mState == GHOST_AI_STATE_START)
		{
			return PACMAN_MOVEMENT_NONE;
		}

		if (mState == GHOST_AI_STATE_IN_PEN)
		{
			mTimer += dt;
			if (mTimer >= PEN_WAIT_DURATION)
			{
				SetState(GHOST_AI_STATE_EXIT_PEN);
			}
			return PACMAN_MOVEMENT_NONE;
		}

		if (mState == GHOST_AI_STATE_GO_TO_PEN && mnoptrGhost->GetPosition() == mGhostPenTarget)
		{
			SetState(GHOST_AI_STATE_IN_PEN);
			mnoptrGhost->SetGhostState(GHOST_STATE_ALIVE);
			return PACMAN_MOVEMENT_NONE;
		}

		if (mState == GHOST_AI_STATE_EXIT_PEN && mnoptrGhost->GetPosition() == mGhostExitPenPosition)
		{
			SetState(GHOST_AI_STATE_SCATTER);
		}

		if (mState == GHOST_AI_STATE_SCATTER)
		{
			mTimer += dt;
			if (mTimer >= SCATTER_MODE_DURATION)
			{
				SetState(GHOST_AI_STATE_CHASE);
			}
		}

		PacmanMovement currentDirection = mnoptrGhost->GetMovementDirection();
		std::vector<PacmanMovement> tempDirs;
		std::vector<PacmanMovement> possibleDirs;

		possibleDirs = GetPerpendicularMovement(currentDirection);
		if (currentDirection != PACMAN_MOVEMENT_NONE)
		{
			possibleDirs.push_back(currentDirection);
		}

		for (const auto& pDir : possibleDirs)
		{
			tempDirs.push_back(pDir);
		}

		possibleDirs.clear();
		
		for (const auto& dir : tempDirs)
		{
			if (!level.WillCollide(*mnoptrGhost, *this, dir))
			{
				possibleDirs.push_back(dir);
			}
		}
		
		assert(possibleDirs.size() > 0 && "ghost stuck");

		std::sort(possibleDirs.begin(), possibleDirs.end(), [](const PacmanMovement& dir1, const PacmanMovement& dir2) {
			return dir1 < dir2;
		});

		if (mnoptrGhost->IsVulnerable() && (mState == GHOST_AI_STATE_SCATTER || mState == GHOST_AI_STATE_CHASE))
		{
			std::uniform_int_distribution<int> distribution(0, possibleDirs.size() - 1);
			return possibleDirs[distribution(mAIRandomGenerator)];
		}

		if (mState == GHOST_AI_STATE_CHASE)
		{
			ChangeTarget(GetChaseTarget(dt, pacman, level, ghosts));
		}

		PacmanMovement directionToGoIn = PACMAN_MOVEMENT_NONE;
		int lowestDistance = INT32_MAX;

		for (const auto& dir : possibleDirs)
		{
			Vec2D movementVec = GetMovementVector(dir) * mLookAheadDistance;
			AARectangle bbox = mnoptrGhost->GetBoundingBox();
			bbox.MoveBy(movementVec);

			int distanceToTarget = bbox.GetCenterPoint().Distance(mTarget);

			if (distanceToTarget < lowestDistance)
			{
				directionToGoIn = dir;
				lowestDistance = distanceToTarget;
			}
		}

		assert(directionToGoIn != PACMAN_MOVEMENT_NONE);
		return directionToGoIn;
	}
	return PACMAN_MOVEMENT_NONE;
}

void GhostAI::Draw(Screen& screen)
{
	if (mnoptrGhost)
	{
		Circle targetCircle = Circle(mTarget, 4);
		screen.Draw(targetCircle, mnoptrGhost->GetSpriteColor(), true, mnoptrGhost->GetSpriteColor());

		AARectangle bbox = mnoptrGhost->GetBoundingBox();
		bbox.MoveBy(GetMovementVector(mnoptrGhost->GetMovementDirection()) * 16);
		Color c = Color(mnoptrGhost->GetSpriteColor().GetRed(),
			mnoptrGhost->GetSpriteColor().GetGreen(),
			mnoptrGhost->GetSpriteColor().GetBlue(),
			200);
		screen.Draw(bbox, mnoptrGhost->GetSpriteColor(), true, c);
	}
}

void GhostAI::GhostDelegateGhostStateChangedTo(GhostState lastState, GhostState state)
{
	if (mnoptrGhost && mnoptrGhost->IsReleased() &&
		(lastState == GHOST_STATE_VULNERABLE || lastState == GHOST_STATE_VULNERABLE_ENDING) &&
		!(IsInPen() || WantsToLeavePen()))
	{
		mnoptrGhost->SetMovementDirection(GetOppositeDirection(mnoptrGhost->GetMovementDirection()));
	}

	if (state == GHOST_STATE_DEAD)
	{
		SetState(GHOST_AI_STATE_GO_TO_PEN);
	}
	else if ((lastState == GHOST_STATE_VULNERABLE || lastState == GHOST_STATE_VULNERABLE_ENDING) &&
		state == GHOST_STATE_ALIVE)
	{
		if (mState == GHOST_AI_STATE_CHASE || mState == GHOST_AI_STATE_SCATTER)
		{
			SetState(mLastState);
		}
	}
}

void GhostAI::GhostDelegateWasReleasedFromPen()
{
	if (mState == GHOST_AI_STATE_START)
	{
		SetState(GHOST_AI_STATE_EXIT_PEN);
	}
}

void GhostAI::GhostWasResetToFirstPosition()
{
	SetState(GHOST_AI_STATE_START);
}

void GhostAI::SetState(GhostAIState state)
{
	if (mState == GHOST_AI_STATE_SCATTER || mState == GHOST_AI_STATE_CHASE)
	{
		mLastState = mState;
	}

	mState = state;
	switch (state)
	{
		case GHOST_AI_STATE_IN_PEN:
			mTimer = 0;
			break;
		case GHOST_AI_STATE_GO_TO_PEN:
			{
				Vec2D target = { mGhostPenTarget.GetX() + mnoptrGhost->GetBoundingBox().GetWidth() / 2,
					mGhostPenTarget.GetY() - mnoptrGhost->GetBoundingBox().GetHeight() / 2 };
				ChangeTarget(target);
			}
			break;
		case GHOST_AI_STATE_EXIT_PEN:
			ChangeTarget(mGhostExitPenPosition);
			break;
		case GHOST_AI_STATE_SCATTER:
			mTimer = 0;
			ChangeTarget(mScatterTarget);
			break;
		default:
			break;
	}
}

void GhostAI::ChangeTarget(const Vec2D& target)
{
	mTarget = target;
}

Vec2D GhostAI::GetChaseTarget(int dt, const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost>& ghosts)
{
	Vec2D target;
	switch (mName)
	{
		case BLINKY:
			target = pacman.GetBoundingBox().GetCenterPoint();
			break;
		case PINKY:
			target = pacman.GetBoundingBox().GetCenterPoint() + 2 *
				GetMovementVector(pacman.GetMovementDirection()) *
				pacman.GetBoundingBox().GetWidth(); // Ahead of pacman
			break;
		case INKY:
		{
			Vec2D pacmanOffsetPoint = pacman.GetBoundingBox().GetCenterPoint() +
				(GetMovementVector(pacman.GetMovementDirection()) *
				pacman.GetBoundingBox().GetWidth());
			target = (pacmanOffsetPoint - ghosts[BLINKY].GetBoundingBox().GetCenterPoint()) * 2 +
				ghosts[BLINKY].GetBoundingBox().GetCenterPoint();
			break;
		}
		case CLYDE:
		{
			auto distanceToPacman = mnoptrGhost->GetBoundingBox().GetCenterPoint()
				.Distance(pacman.GetBoundingBox().GetCenterPoint());
			if (distanceToPacman > pacman.GetBoundingBox().GetWidth() * 4)
			{
				target = pacman.GetBoundingBox().GetCenterPoint();
			}
			else
			{
				target = mScatterTarget;
			}
			break;
		}
	}

	return target;
}