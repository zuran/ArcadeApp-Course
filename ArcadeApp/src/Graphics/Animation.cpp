#include "Animation.h"

Animation::Animation() : mSize({0,0}), mSpriteSheetName(""), mAnimationName(""), mFps(0)
{
}

AnimationFrame Animation::GetAnimationFrame(uint32_t frameNum) const
{
	AnimationFrame frame;

	if (frameNum > mFrames.size())
	{
		return frame;
	}

	frame.frame = mFrames[frameNum];
	
	if (frameNum < mFrameColors.size())
	{
		frame.frameColor = mFrameColors[frameNum];
		frame.frameColorSet = 1;
	}

	if (mOverlay.size() > 0)
	{
		frame.overlay = mOverlay;
	}

	if (frameNum < mOverlayColors.size())
	{
		frame.overlayColor = mOverlayColors[frameNum];
	}

	if (frameNum < mFrameOffsets.size())
	{
		frame.offset = mFrameOffsets[frameNum];
	}

	frame.size = mSize;
	return frame;
}
