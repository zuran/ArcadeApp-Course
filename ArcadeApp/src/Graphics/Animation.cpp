#include "Animation.h"
#include "FileCommandLoader.h"
#include <cassert>

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

std::vector<Animation> Animation::LoadAnimations(const std::string& animationFilePath)
{
	std::vector<Animation> animations;
	FileCommandLoader fileLoader;
	Command animationCommand;
	animationCommand.command = "animation";
	animationCommand.parseFunc = [&](ParseFuncParams params) {
		Animation newAnimation;
		newAnimation.SetName(FileCommandLoader::ReadString(params));
		animations.push_back(newAnimation);
	};

	fileLoader.AddCommand(animationCommand);

	Command spriteSheetCommand;
	spriteSheetCommand.command = "sprite_sheet";
	spriteSheetCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().SetSpriteSheetName(FileCommandLoader::ReadString(params));
	};

	fileLoader.AddCommand(spriteSheetCommand);

	Command sizeCommand;
	sizeCommand.command = "size";
	sizeCommand.parseFunc = [&](ParseFuncParams params) {
		Vec2D size = FileCommandLoader::ReadSize(params);
		animations.back().SetSize(Size{static_cast<unsigned int>(size.GetX()), static_cast<unsigned int>(size.GetY())});
	};

	fileLoader.AddCommand(sizeCommand);

	Command fpsCommand;
	fpsCommand.command = "fps";
	fpsCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().SetFps(FileCommandLoader::ReadInt(params));
	};

	fileLoader.AddCommand(fpsCommand);

	Command framesCommand;
	framesCommand.command = "frame_keys";
	framesCommand.commandType = COMMAND_MULTI_LINE;
	framesCommand.parseFunc = [&](ParseFuncParams params)
	{
		animations.back().AddFrame(params.line);
	};

	fileLoader.AddCommand(framesCommand);

	Command overlayCommand;
	overlayCommand.command = "overlay";
	overlayCommand.parseFunc = [&](ParseFuncParams params)
	{
		animations.back().SetOverlay(FileCommandLoader::ReadString(params));
	};

	fileLoader.AddCommand(overlayCommand);

	Command frameColorsCommand;
	frameColorsCommand.command = "frame_colors";
	frameColorsCommand.commandType = COMMAND_MULTI_LINE;
	frameColorsCommand.parseFunc = [&](ParseFuncParams params)
	{
		animations.back().AddFrameColor(FileCommandLoader::ReadColor(params));
	};

	fileLoader.AddCommand(frameColorsCommand);

	Command overlayColorsCommand;
	overlayColorsCommand.command = "overlay_colors";
	overlayColorsCommand.commandType = COMMAND_MULTI_LINE;
	overlayColorsCommand.parseFunc = [&](ParseFuncParams params)
	{
		animations.back().AddOverlayFrameColor(FileCommandLoader::ReadColor(params));
	};

	fileLoader.AddCommand(overlayColorsCommand);

	Command frameOffsetsCommand;
	frameOffsetsCommand.command = "frame_offsets";
	frameOffsetsCommand.commandType = COMMAND_MULTI_LINE;
	frameOffsetsCommand.parseFunc = [&](ParseFuncParams params)
	{
		animations.back().AddFrameOffset(FileCommandLoader::ReadSize(params));
	};

	fileLoader.AddCommand(frameOffsetsCommand);

	assert(fileLoader.LoadFile(animationFilePath));
	
	return animations;
}
