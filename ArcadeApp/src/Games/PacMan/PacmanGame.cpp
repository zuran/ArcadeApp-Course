#include "PacmanGame.h"

void PacmanGame::Init(GameController& controller)
{
}

void PacmanGame::Update(uint32_t dt)
{
}

void PacmanGame::Draw(Screen& screen)
{
}

const std::string& PacmanGame::GetName() const
{
	static std::string name = "!Pacman";
	return name;
}
