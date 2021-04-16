#pragma once

#include "BmpImage.h"
#include <string>
#include <vector>

struct Sprite
{
	uint32_t xPos = 0;
	uint32_t yPos = 0;
	uint32_t width = 0;
	uint32_t height = 0;
};

class SpriteSheet
{
public:
	SpriteSheet();
	bool Load(const std::string& name);
	Sprite GetSprite(const std::string& spriteName) const;

	std::vector<std::string> SpriteNames() const;

	inline const BmpImage& GetBmpImage() const { return mBmpImage; }
	inline uint32_t GetWidth() const { return mBmpImage.GetWidth(); }
	inline uint32_t GetHeight() const { return mBmpImage.GetHeight(); }

private:
	bool LoadSpriteSections(const std::string& path);

	struct BmpImageSection
	{
		std::string key = "";
		Sprite sprite;
	};

	BmpImage mBmpImage;
	std::vector<BmpImageSection> mSections;
};