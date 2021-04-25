#include "PacmanLevel.h"
#include "FileCommandLoader.h"
#include "Screen.h"
#include "Pacman.h"
#include "App.h"
#include "Circle.h"
#include "Ghost.h"
#include <cassert>

namespace {
	const int NUM_LEVELS = 256;
	const int SPRITE_HEIGHT = 16;
	const int SPRITE_WIDTH = 16;
}

bool PacmanLevel::Init(const std::string& levelPath, const SpriteSheet* noptrSpriteSheet)
{
	mCurrentLevel = 0;
	mnoptrSpriteSheet = noptrSpriteSheet;
	mBonusItemSpriteName = "";
	std::random_device r;
	mGenerator.seed(r());
	mGhostSpawnPoints.resize(NUM_GHOSTS);

	bool levelLoaded = LoadLevel(levelPath);
	if (levelLoaded) {
		ResetToFirstLevel();
	}

	return levelLoaded;
}

void PacmanLevel::Update(int dt, Pacman& pacman, std::vector<Ghost>& ghosts,
	std::vector<GhostAI>& ghostAIs)
{
	for (const auto& wall : mWalls)
	{
		BoundaryEdge edge;

		if (wall.HasCollided(pacman.GetBoundingBox(), edge))
		{
			Vec2D offset = wall.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (auto& ghost : ghosts)
		{
			if (wall.HasCollided(ghost.GetBoundingBox(), edge))
			{
				Vec2D offset = wall.GetCollisionOffset(ghost.GetBoundingBox());
				ghost.MoveBy(offset);
				ghost.Stop();
			}
		}
	}

	for (const auto& gate : mGate)
	{
		BoundaryEdge edge;
		if (gate.HasCollided(pacman.GetBoundingBox(), edge))
		{
			Vec2D offset = gate.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (int i = 0; i < NUM_GHOSTS; ++i)
		{
			GhostAI& ghostAI = ghostAIs[i];
			Ghost& ghost = ghosts[i];

			if (!(ghostAI.WantsToLeavePen() || ghostAI.IsEnteringPen()) &&
				gate.HasCollided(ghost.GetBoundingBox(), edge))
			{
				Vec2D offset = gate.GetCollisionOffset(ghost.GetBoundingBox());
				ghost.MoveBy(offset);
				ghost.Stop();
			}
		}
	}

	for (Tile t : mTiles)
	{
		if (t.isTeleportTile)
		{
			AARectangle teleportTileAABB(t.position, t.width, static_cast<float>(mTileHeight));
			Tile* teleportToTile = GetTileForSymbol(t.teleportToSymbol);
			assert(teleportToTile);

			if(teleportToTile->isTeleportTile)
			{
				if (teleportTileAABB.Intersects(pacman.GetBoundingBox()))
				{
					pacman.MoveTo(teleportToTile->position + teleportToTile->offset);
				}

				for (auto& ghost : ghosts)
				{
					if (teleportTileAABB.Intersects(ghost.GetBoundingBox()))
					{
						ghost.MoveTo(teleportToTile->position + teleportToTile->offset);
					}
				}
			}
		}
	}

	for (auto& pellet : mPellets)
	{
		if (!pellet.eaten)
		{
			if (pacman.GetEatingBoundingBox().Intersects(pellet.bBox))
			{
				pellet.eaten = true;
				pacman.AteItem(pellet.score);

				if (pellet.powerPellet)
				{
					pacman.ResetGhostEatenMultiplier();
					//TODO: make ghosts go vulnerable
				}
			}
		}
	}

	if (ShouldSpawnBonusItem())
	{
		SpawnBonusItem();
	}

	if (mBonusItem.spawned && !mBonusItem.eaten)
	{
		if(pacman.GetEatingBoundingBox().Intersects(mBonusItem.bbox))
		{
			mBonusItem.eaten = true;
			pacman.AteItem(mBonusItem.score);
		}
	}
}

void PacmanLevel::Draw(Screen& screen)
{
	// Debug for walls
	for (const auto& wall : mWalls)
	{
		screen.Draw(wall.GetAARectangle(), Color::Blue());
	}

	for (const auto& pellet : mPellets)
	{
		if (!pellet.eaten)
		{
			if (!pellet.powerPellet)
			{
				screen.Draw(pellet.bBox, Color::White());
			}
			else
			{
				Circle c(pellet.bBox.GetCenterPoint(), pellet.bBox.GetWidth()/2.0f);
				screen.Draw(c, Color::White(), true, Color::White());
			}
		}
	}

	if (mBonusItem.spawned && !mBonusItem.eaten)
	{
		screen.Draw(*mnoptrSpriteSheet, mBonusItemSpriteName, mBonusItem.bbox.GetTopLeftPoint());
	}
}

bool PacmanLevel::WillCollide(const AARectangle& aBBox, PacmanMovement dir) const
{
	AARectangle bbox = aBBox;
	bbox.MoveBy(GetMovementVector(dir));

	BoundaryEdge edge;
	for (const auto& wall : mWalls)
	{
		if (wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	for (const auto& gate : mGate)
	{
		if (gate.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;
}

bool PacmanLevel::WillCollide(const Ghost& ghost, const GhostAI& ghostAI, PacmanMovement dir) const
{
	AARectangle bbox = ghost.GetBoundingBox();
	bbox.MoveBy(GetMovementVector(dir));

	BoundaryEdge edge;
	for (const auto& wall : mWalls)
	{
		if (wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	for (const auto& gate : mGate)
	{
		if (!(ghostAI.IsEnteringPen() || ghostAI.WantsToLeavePen()) && gate.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;
}

bool PacmanLevel::LoadLevel(const std::string& levelPath)
{
	FileCommandLoader fileLoader;
	
	Command tileWidthCommand;
	tileWidthCommand.command = "tile_width";
	tileWidthCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().width = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileWidthCommand);

	Command tileHeightCommand;
	tileHeightCommand.command = "tile_height";
	tileHeightCommand.parseFunc = [this](ParseFuncParams params) {
		mTileHeight = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileHeightCommand);

	Command tileCommand;
	tileCommand.command = "tile";
	tileCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.push_back(Tile());
	};
	fileLoader.AddCommand(tileCommand);

	Command tileSymbolCommand;
	tileSymbolCommand.command = "tile_symbol";
	tileSymbolCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().symbol = FileCommandLoader::ReadChar(params);
	};
	fileLoader.AddCommand(tileSymbolCommand);

	Command tileCollisionCommand;
	tileCollisionCommand.command = "tile_collision";
	tileCollisionCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().collidable = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileCollisionCommand);

	Vec2D layoutOffset;
	Command layoutOffsetCommand;
	layoutOffsetCommand.command = "layout_offset";
	layoutOffsetCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params) {
		mLayoutOffset = FileCommandLoader::ReadSize(params);
		layoutOffset = mLayoutOffset;
	};
	fileLoader.AddCommand(layoutOffsetCommand);

	Command tileIsTeleportTileCommand;
	tileIsTeleportTileCommand.command = "tile_is_teleport_tile";
	tileIsTeleportTileCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().isTeleportTile = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileIsTeleportTileCommand);

	Command tileToTeleportToCommand;
	tileToTeleportToCommand.command = "tile_teleport_to_symbol";
	tileToTeleportToCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().teleportToSymbol = FileCommandLoader::ReadChar(params);
	};
	fileLoader.AddCommand(tileToTeleportToCommand);

	Command tileOffsetCommand;
	tileOffsetCommand.command = "tile_offset";
	tileOffsetCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().offset = FileCommandLoader::ReadSize(params);
	};
	fileLoader.AddCommand(tileOffsetCommand);

	Command tileExcludePelletCommand;
	tileExcludePelletCommand.command = "tile_exclude_pellet";
	tileExcludePelletCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().excludePelletTile = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileExcludePelletCommand);

	Command tilePacmanSpawnPointCommand;
	tilePacmanSpawnPointCommand.command = "tile_pacman_spawn_point";
	tilePacmanSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().pacmanSpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tilePacmanSpawnPointCommand);

	Command tileItemSpawnPointCommand;
	tileItemSpawnPointCommand.command = "tile_item_spawn_point";
	tileItemSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().itemSpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileItemSpawnPointCommand);

	Command tileBlinkySpawnPointCommand;
	tileBlinkySpawnPointCommand.command = "tile_blinky_spawn_point";
	tileBlinkySpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().blinkySpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileBlinkySpawnPointCommand);

	Command tileInkySpawnPointCommand;
	tileInkySpawnPointCommand.command = "tile_inky_spawn_point";
	tileInkySpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().inkySpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileInkySpawnPointCommand);

	Command tilePinkySpawnPointCommand;
	tilePinkySpawnPointCommand.command = "tile_pinky_spawn_point";
	tilePinkySpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().pinkySpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tilePinkySpawnPointCommand);

	Command tileClydeSpawnPointCommand;
	tileClydeSpawnPointCommand.command = "tile_clyde_spawn_point";
	tileClydeSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().clydeSpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileClydeSpawnPointCommand);

	Command tileGateCommand;
	tileGateCommand.command = "tile_is_gate";
	tileGateCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isGate = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileGateCommand);


	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params)
	{
		int startingX = layoutOffset.GetX();
		for (int c = 0; c < params.line.length(); ++c)
		{
			Tile* tile = GetTileForSymbol(params.line[c]);
			if (tile)
			{
				tile->position = Vec2D(startingX, layoutOffset.GetY());

				if (tile->isGate > 0)
				{
					Excluder gate;
					gate.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width, static_cast<int>(mTileHeight)));
					mGate.push_back(gate);
				}
				else if (tile->collidable > 0)
				{
					Excluder wall;
					wall.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width, static_cast<int>(mTileHeight)));
					mWalls.push_back(wall);
				}

				if (tile->pacmanSpawnPoint > 0)
				{
					mPacmanSpaceLocation = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->itemSpawnPoint > 0)
				{
					mBonusItem.bbox = AARectangle(Vec2D(startingX + tile->offset.GetX(),
						layoutOffset.GetY() + tile->offset.GetY()), SPRITE_WIDTH, SPRITE_HEIGHT);
				}
				else if (tile->blinkySpawnPoint > 0)
				{
					mGhostSpawnPoints[BLINKY] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->pinkySpawnPoint > 0)
				{
					mGhostSpawnPoints[PINKY] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->inkySpawnPoint > 0)
				{
					mGhostSpawnPoints[INKY] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->clydeSpawnPoint > 0)
				{
					mGhostSpawnPoints[CLYDE] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}


				if (tile->excludePelletTile > 0)
				{
					mExclusionTiles.push_back(*tile);
				}

				startingX += tile->width;
			}
		}
		layoutOffset += Vec2D(0, mTileHeight);
	};
	fileLoader.AddCommand(layoutCommand);

	Command bonusItemCommand;
	bonusItemCommand.command = "bonus_item";
	bonusItemCommand.parseFunc = [this](ParseFuncParams params)
	{
		BonusItemLevelProperties newProperty;
		mBonusItemProperties.push_back(newProperty);
	};
	fileLoader.AddCommand(bonusItemCommand);

	Command bonusItemSpriteNameCommand;
	bonusItemSpriteNameCommand.command = "bonus_item_sprite_name";
	bonusItemSpriteNameCommand.parseFunc = [this](ParseFuncParams params)
	{
		mBonusItemProperties.back().spriteName = FileCommandLoader::ReadString(params);
	};
	fileLoader.AddCommand(bonusItemSpriteNameCommand);

	Command bonusItemScoreCommand;
	bonusItemScoreCommand.command = "bonus_item_score";
	bonusItemScoreCommand.parseFunc = [this](ParseFuncParams params)
	{
		mBonusItemProperties.back().score = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bonusItemScoreCommand);

	Command bonusItemBeginLevelCommand;
	bonusItemBeginLevelCommand.command = "bonus_item_begin_level";
	bonusItemBeginLevelCommand.parseFunc = [this](ParseFuncParams params)
	{
		mBonusItemProperties.back().begin = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bonusItemBeginLevelCommand);

	Command bonusItemEndLevelCommand;
	bonusItemEndLevelCommand.command = "bonus_item_end_level";
	bonusItemEndLevelCommand.parseFunc = [this](ParseFuncParams params)
	{
		mBonusItemProperties.back().end = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bonusItemEndLevelCommand);

	return fileLoader.LoadFile(levelPath);
}

PacmanLevel::Tile* PacmanLevel::GetTileForSymbol(char symbol)
{
	for (int i = 0; i < mTiles.size(); ++i)
	{
		if (mTiles[i].symbol == symbol)
		{
			return &mTiles[i];
		}
	}
	return nullptr;
}

void PacmanLevel::ResetLevel()
{
	ResetPellets();

	std::uniform_int_distribution<int> distribution(20, mPellets.size() - 50);
	mBonusItem.spawnTime = distribution(mGenerator);

	GetBonusItemSpriteName(mBonusItemSpriteName, mBonusItem.score);
}

bool PacmanLevel::IsLevelOver() const
{
	return HasEatenAllPellets();
}

void PacmanLevel::IncreaseLevel()
{
	mCurrentLevel++;
	if (mCurrentLevel > NUM_LEVELS)
	{
		mCurrentLevel = 1;
	}

	ResetLevel();
}

void PacmanLevel::ResetToFirstLevel()
{
	mCurrentLevel = 1;
	ResetLevel();
}

void PacmanLevel::ResetPellets()
{
	mPellets.clear();
	const int PELLET_SIZE = 2;
	const int PADDING = static_cast<int>(mTileHeight);

	int startingY = mLayoutOffset.GetY() + PADDING + mTileHeight - 1;
	int startingX = PADDING + 3;

	const int LEVEL_HEIGHT = mLayoutOffset.GetY() + 32 * mTileHeight;
	Pellet p;
	p.score = 10;

	int row = 0;

	for (int y = startingY; y < LEVEL_HEIGHT; y += PADDING, ++row)
	{
		for (int x = startingX, col = 0; x < App::Singleton().Width(); x += PADDING, ++col)
		{
			if (row == 0 || row == 22)
			{
				if (col == 0 || col == 25)
				{
					p.powerPellet = 1;
					p.score = 50;
					p.bBox = AARectangle(Vec2D(x - 3, y - 3), mTileHeight, mTileHeight);
					mPellets.push_back(p);

					p.powerPellet = 0;
					p.score = 10;

					continue;
				}
			}

			AARectangle rect = AARectangle(Vec2D(x,y), PELLET_SIZE, PELLET_SIZE);
			bool found = false;
			for (const Excluder& wall : mWalls)
			{
				if (wall.GetAARectangle().Intersects(rect))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				for (const Tile& excludePelletTile : mExclusionTiles)
				{
					if (excludePelletTile.excludePelletTile)
					{
						AARectangle tileAABB(excludePelletTile.position, excludePelletTile.width, mTileHeight);

						if (tileAABB.Intersects(rect))
						{
							found = true;
							break;
						}
					}
				}
			}

			if (!found)
			{
				p.bBox = rect;
				mPellets.push_back(p);
			}
		}
	}
}

bool PacmanLevel::HasEatenAllPellets() const
{
	return NumPelletsEaten() >= mPellets.size() - 4;
}

int PacmanLevel::NumPelletsEaten() const
{
	int numEaten = 0;
	for (const auto& pellet : mPellets)
	{
		if (!pellet.powerPellet && pellet.eaten)
		{
			++numEaten;
		}
	}
	return numEaten;
}

void PacmanLevel::GetBonusItemSpriteName(std::string& spriteName, int& score) const
{
	for (const auto& properties : mBonusItemProperties)
	{
		if (mCurrentLevel >= properties.begin && mCurrentLevel <= properties.end)
		{
			spriteName = properties.spriteName;
			score = properties.score;
			return;
		}
	}
}

void PacmanLevel::SpawnBonusItem()
{
	mBonusItem.spawned = 1;
	mBonusItem.eaten = 0;
}

bool PacmanLevel::ShouldSpawnBonusItem() const
{
	auto numEaten = NumPelletsEaten();
	return !mBonusItem.spawned && numEaten >= mBonusItem.spawnTime;
}

