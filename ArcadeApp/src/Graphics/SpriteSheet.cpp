#include "SpriteSheet.h"
#include "FileCommandLoader.h"
#include "Utils.h"
#include "App.h"

SpriteSheet::SpriteSheet()
{
}

bool SpriteSheet::Load(const std::string& name)
{
	bool loadedImage = mBmpImage.Load(App::Singleton().GetBasePath() + std::string("Assets/") + name + ".bmp");
	bool loadSpriteSections = LoadSpriteSections(App::Singleton().GetBasePath() + std::string("Assets/" + name + ".txt"));
	return loadedImage && loadSpriteSections;
}

Sprite SpriteSheet::GetSprite(const std::string& spriteName) const
{
	size_t length = mSections.size();
	for (size_t i = 0; i < length; ++i)
	{
		if (StringCompare(mSections[i].key, spriteName))
		{
			return mSections[i].sprite;
		}
	}
	return Sprite();
}

std::vector<std::string> SpriteSheet::SpriteNames() const
{
	std::vector<std::string> names;
	for (const auto& section : mSections)
	{
		names.push_back(section.key);
	}
	return names;
}

bool SpriteSheet::LoadSpriteSections(const std::string& path)
{
	FileCommandLoader fileLoader;

	Command spriteCommand;
	spriteCommand.command = "sprite";
	spriteCommand.parseFunc = [&](ParseFuncParams params) {
		BmpImageSection section;
		mSections.push_back(section);
	};
	fileLoader.AddCommand(spriteCommand);

	Command keyCommand;
	keyCommand.command = "key";
	keyCommand.parseFunc = [&](ParseFuncParams params) {
		BmpImageSection section;
		mSections.back().key = FileCommandLoader::ReadString(params);
	};
	fileLoader.AddCommand(keyCommand);

	Command xPosCommand;
	xPosCommand.command = "xPos";
	xPosCommand.parseFunc = [&](ParseFuncParams params) {
		BmpImageSection section;
		mSections.back().sprite.xPos = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(xPosCommand);

	Command yPosCommand;
	yPosCommand.command = "yPos";
	yPosCommand.parseFunc = [&](ParseFuncParams params) {
		BmpImageSection section;
		mSections.back().sprite.yPos = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(yPosCommand);

	Command widthCommand;
	widthCommand.command = "width";
	widthCommand.parseFunc = [&](ParseFuncParams params) {
		BmpImageSection section;
		mSections.back().sprite.width = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(widthCommand);

	Command heightCommand;
	heightCommand.command = "height";
	heightCommand.parseFunc = [&](ParseFuncParams params) {
		BmpImageSection section;
		mSections.back().sprite.height = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(heightCommand);

	return fileLoader.LoadFile(path);
}
