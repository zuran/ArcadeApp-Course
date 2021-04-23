#pragma once

#include "Vec2D.h"
#include "Excluder.h"
#include <string>
#include <vector>
#include "PacmanGameUtils.h"

class Screen;
class Pacman;

class PacmanLevel
{
public:
	bool Init(const std::string& levelPath, Pacman* noptrPacman);
	void Update(int dt);
	void Draw(Screen& screen);

	bool WillCollide(const AARectangle& bbox, PacmanMovement dir) const;

private:
	struct Tile
	{
		Vec2D position = Vec2D::Zero;
		Vec2D offset = Vec2D::Zero;
		int width = 0;
		int collidable = 0;
		int isTeleportTile = 0;
		int excludePelletTile = 0;
		char teleportToSymbol = 0;
		char symbol = '-';
	};

	struct Pellet
	{
		int score = 0;
		AARectangle bBox;
		int powerPellet = 0;
		int eaten = 0;
	};

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	void ResetPellets();
	void ResetLevel();

	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;

	std::vector<Tile> mExclusionTiles;
	std::vector<Pellet> mPellets;

	Vec2D mLayoutOffset;
	int mTileHeight;

	Pacman* mnoptrPacman;
};