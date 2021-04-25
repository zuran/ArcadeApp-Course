#pragma once

#include "Vec2D.h"
#include "Excluder.h"
#include <string>
#include <vector>
#include "PacmanGameUtils.h"
#include "SpriteSheet.h"
#include "GhostAI.h"
#include <random>
#include "BmpImage.h"

class Screen;
class Pacman;
class Ghost;

class PacmanLevel
{
public:
	bool Init(const std::string& levelPath, const SpriteSheet* noptrSpriteSheet);
	void Update(int dt, Pacman& pacman, std::vector<Ghost>& ghosts,
		std::vector<GhostAI>& ghostAIs);
	void Draw(Screen& screen);

	bool WillCollide(const AARectangle& bbox, PacmanMovement dir) const;
	bool WillCollide(const Ghost& ghost, const GhostAI& ghostAI, PacmanMovement dir) const;
	void ResetLevel();

	inline Vec2D GetLayoutOffset() const { return mLayoutOffset; }
	inline Vec2D GetPacmanSpawnLocation() const { return mPacmanSpaceLocation; }
	bool IsLevelOver() const;
	void IncreaseLevel();
	void ResetToFirstLevel();
	inline const std::vector<Vec2D>& GetGhostSpawnPoints() { return mGhostSpawnPoints; }
	inline int GetInGameTextYPos() const { return mBonusItem.bbox.GetTopLeftPoint().GetY(); }

private:
	struct Tile
	{
		Vec2D position = Vec2D::Zero;
		Vec2D offset = Vec2D::Zero;
		int width = 0;
		int collidable = 0;
		int isTeleportTile = 0;
		int excludePelletTile = 0;
		int pacmanSpawnPoint = 0;
		int itemSpawnPoint = 0;
		int blinkySpawnPoint = 0;
		int inkySpawnPoint = 0;
		int pinkySpawnPoint = 0;
		int clydeSpawnPoint = 0;
		int isGate = 0;
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

	struct BonusItem
	{
		int score = 0;
		AARectangle bbox;
		int eaten = 0;
		int spawned = 0;
		int spawnTime = -1;
	};

	struct BonusItemLevelProperties
	{
		int score = 0;
		std::string spriteName = "";
		int begin = 0;
		int end = 0;
	};

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	void ResetPellets();
	bool HasEatenAllPellets() const;
	int NumPelletsEaten() const;
	void GetBonusItemSpriteName(std::string& spriteName, int& score) const;
	void SpawnBonusItem();
	bool ShouldSpawnBonusItem() const;

	BmpImage mBGImage;	
	std::vector<Vec2D> mGhostSpawnPoints;
	std::default_random_engine mGenerator;
	BonusItem mBonusItem;
	std::string mBonusItemSpriteName;
	const SpriteSheet* mnoptrSpriteSheet;

	std::vector<BonusItemLevelProperties> mBonusItemProperties;
	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;

	std::vector<Tile> mExclusionTiles;
	std::vector<Pellet> mPellets;

	std::vector<Excluder> mGate;

	Vec2D mPacmanSpaceLocation;
	Vec2D mLayoutOffset;
	int mTileHeight;

	int mCurrentLevel;
};