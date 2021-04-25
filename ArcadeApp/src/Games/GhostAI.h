#pragma once

#include "Ghost.h"
#include <random>
#include "PacmanGameUtils.h"

class PacmanLevel;
class Screen;

class GhostAI
{
public:
	GhostAI();
	void Init(Ghost& ghost, GhostName name);

	PacmanMovement Update(int dt, const PacmanLevel& level);
	void Draw(Screen& screen);

private:
	Ghost* mnoptrGhost;
	GhostName mName;
	std::default_random_engine mAIRandomGenerator;
};