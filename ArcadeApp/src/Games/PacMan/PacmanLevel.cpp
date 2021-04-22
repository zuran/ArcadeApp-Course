#include "PacmanLevel.h"
#include "FileCommandLoader.h"
#include "Screen.h"
#include "Pacman.h"
#include <cassert>

bool PacmanLevel::Init(const std::string& levelPath, Pacman* noptrPacman)
{
	mnoptrPacman = noptrPacman;
	return LoadLevel(levelPath);
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
}

void PacmanLevel::Draw(Screen& screen)
{
	// Debug for walls
	for (const auto& wall : mWalls)
	{
		screen.Draw(wall.GetAARectangle(), Color::Blue());
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
