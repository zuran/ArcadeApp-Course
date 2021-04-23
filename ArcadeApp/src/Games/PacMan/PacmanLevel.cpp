#include "PacmanLevel.h"
#include "FileCommandLoader.h"
#include "Screen.h"
#include "Pacman.h"
#include "App.h"
#include "Circle.h"
#include <cassert>

bool PacmanLevel::Init(const std::string& levelPath, Pacman* noptrPacman)
{
	mnoptrPacman = noptrPacman;

	bool levelLoaded = LoadLevel(levelPath);
	if (levelLoaded) {
		ResetLevel();
	}

	return levelLoaded;
}

void PacmanLevel::Update(int dt)
{
	for (const auto& wall : mWalls)
	{
		BoundaryEdge edge;

		if (wall.HasCollided(mnoptrPacman->GetBoundingBox(), edge))
		{
			Vec2D offset = wall.GetCollisionOffset(mnoptrPacman->GetBoundingBox());
			mnoptrPacman->MoveBy(offset);
			mnoptrPacman->Stop();
		}
	}

	for (const Tile& t : mTiles)
	{
		if (t.isTeleportTile)
		{
			AARectangle teleportTileAABB(t.position, t.width, static_cast<float>(mTileHeight));
			Tile* teleportToTile = GetTileForSymbol(t.teleportToSymbol);
			assert(teleportToTile);

			if (teleportToTile->isTeleportTile && teleportTileAABB.Intersects(mnoptrPacman->GetBoundingBox()))
			{
				mnoptrPacman->MoveTo(teleportToTile->position + teleportToTile->offset);
			}
		}
	}

	for (auto& pellet : mPellets)
	{
		if (!pellet.eaten)
		{
			if (mnoptrPacman->GetEatingBoundingBox().Intersects(pellet.bBox))
			{
				pellet.eaten = true;
				mnoptrPacman->AteItem(pellet.score);

				if (pellet.powerPellet)
				{
					mnoptrPacman->ResetGhostEatenMultiplier();
					//TODO: make ghosts go vulnerable
				}
			}
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
}

bool PacmanLevel::WillCollide(const AARectangle& aBBox, PacmanMovement dir) const
{
	AARectangle bbox = aBBox;
	bbox.MoveBy(GetMovementVector(dir));

	for (const auto& wall : mWalls)
	{
		BoundaryEdge edge;
		if (wall.HasCollided(bbox, edge))
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
				if (tile->collidable > 0)
				{
					Excluder wall;
					wall.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width, static_cast<int>(mTileHeight)));
					mWalls.push_back(wall);
				}

				if (tile->pacmanSpawnPoint > 0)
				{
					mPacmanSpaceLocation = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
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

	if (mnoptrPacman)
	{
		mnoptrPacman->MoveTo(mPacmanSpaceLocation);
		mnoptrPacman->ResetToFirstAnimation();
	}
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

