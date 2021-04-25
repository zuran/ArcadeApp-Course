#include "GhostAI.h"
#include "Screen.h"
#include "PacmanLevel.h"
#include <cassert>
#include <algorithm>

GhostAI::GhostAI() : mnoptrGhost(nullptr)
{
}

void GhostAI::Init(Ghost& ghost, GhostName name)
{
	mnoptrGhost = &ghost;
	mName = name;
	std::random_device r;
	mAIRandomGenerator.seed(r());
}

PacmanMovement GhostAI::Update(int dt, const PacmanLevel& level)
{
	if (mnoptrGhost)
	{
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
			if (!level.WillCollide(mnoptrGhost->GetBoundingBox(), dir))
			{
				possibleDirs.push_back(dir);
			}
		}
		
		assert(possibleDirs.size() > 0 && "ghost stuck");

		std::sort(possibleDirs.begin(), possibleDirs.end(), [](const PacmanMovement& dir1, const PacmanMovement& dir2) {
			return dir1 < dir2;
		});

		std::uniform_int_distribution<int> distribution(0, possibleDirs.size()-1);
		return possibleDirs[distribution(mAIRandomGenerator)];
	}
	return PACMAN_MOVEMENT_NONE;
}

void GhostAI::Draw(Screen& screen)
{
	if (mnoptrGhost)
	{
		AARectangle bbox = mnoptrGhost->GetBoundingBox();
		bbox.MoveBy(GetMovementVector(mnoptrGhost->GetMovementDirection()) * 16);
		Color c = Color(mnoptrGhost->GetSpriteColor().GetRed(),
			mnoptrGhost->GetSpriteColor().GetGreen(),
			mnoptrGhost->GetSpriteColor().GetBlue(),
			200);
		screen.Draw(bbox, mnoptrGhost->GetSpriteColor(), true, c);
	}
}
