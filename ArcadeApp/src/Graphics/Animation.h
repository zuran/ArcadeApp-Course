#pragma once

#include <string>
#include <vector>
#include "Vec2D.h"
#include "Color.h"
#include "Utils.h"

struct AnimationFrame
{
	std::string frame = "";
	std::string overlay = "";
	Color frameColor = Color::White();
	Color overlayColor = Color::White();
	Size size;
	Vec2D offset = Vec2D::Zero;
	int frameColorSet = 0;
};

class Animation
{
public:
	Animation();
	
	AnimationFrame GetAnimationFrame(uint32_t frameNum) const;

	inline void AddFrame(const std::string& frame) { mFrames.push_back(frame); }
	inline void AddFrameColor(const Color& color) { mFrameColors.push_back(color); }
	inline void AddOverlayFrameColor(const Color& color) { mOverlayColors.push_back(color); }
	inline void AddFrameOffset(const Vec2D& offset) { mFrameOffsets.push_back(offset); }

	void SetSpriteSheetName(const std::string& spriteSheetName) { mSpriteSheetName = spriteSheetName; }
	const std::string& GetSpriteSheetName() const { return mSpriteSheetName; }

	void SetName(const std::string& animationName) { mAnimationName = animationName; }
	const std::string& GetName() const { return mAnimationName; }

	Size GetSize() const { return mSize; }
	void SetSize(const Size& size) { mSize = size; }

	void SetFps(int fps) { mFps = fps; }
	int GetFps() const { return mFps; }

	void SetOverlay(const std::string& overlayName) { mOverlay = overlayName; }
	const std::string& OverlayName() const { return mOverlay; }

	size_t GetNumFrames() const { return mFrames.size(); }
	size_t GetNumFrameColors() const { return mFrameColors.size(); }
	size_t GetNumOverlayColors() const { return mOverlayColors.size(); }
	size_t GetNumFrameOffsets() const { return mFrameOffsets.size(); }


private:
	std::string mAnimationName;
	std::string mSpriteSheetName;
	std::string mOverlay;
	std::vector<std::string> mFrames;
	std::vector<Color> mFrameColors;
	std::vector<Color> mOverlayColors;
	std::vector<Vec2D> mFrameOffsets;
	Size mSize;
	int mFps;
};