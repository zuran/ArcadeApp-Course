#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer() : mTime(0), mCurrentAnimation(0), mFrame(0), mLooped(false), mIsPlaying(false), mIsFinishedPlaying(false)
{
}

bool AnimationPlayer::Init(const std::string& animationsPath)
{
	mAnimations = Animation::LoadAnimations(animationsPath);
	return mAnimations.size() > 0;
}

bool AnimationPlayer::Play(const std::string& animationName, bool isLooped)
{
	bool found = false;

	for (size_t i = 0; i < mAnimations.size(); ++i)
	{
		if (mAnimations[i].GetName() == animationName)
		{
			mCurrentAnimation = i;
			found = true;
			break;
		}
	}
	
	if (found)
	{
		mIsPlaying = true;
		mFrame = 0;
		mTime = 0;
		mLooped = isLooped;
		mIsFinishedPlaying = false;
	}

	return found;
}

void AnimationPlayer::Pause()
{
	if (!mIsFinishedPlaying)
	{
		mIsPlaying = !mIsPlaying;
	}
}

void AnimationPlayer::Stop()
{
	mIsPlaying = false;
	mTime = 0;
	mFrame = 0;
	mIsFinishedPlaying = true;
}

void AnimationPlayer::Update(uint32_t dt)
{
	if (mIsPlaying)
	{
		mTime += dt;
		int fps = mAnimations[mCurrentAnimation].GetFps();
		float millisecondsPerFrame = static_cast<float>(1000) / static_cast<float>(fps);
		if (static_cast<float>(mTime) > millisecondsPerFrame)
		{
			mTime -= static_cast<uint32_t>(millisecondsPerFrame);
			if (mFrame + 1 < mAnimations[mCurrentAnimation].GetNumFrames())
			{
				++mFrame;
			}
			else if (mFrame + 1 >= mAnimations[mCurrentAnimation].GetNumFrames() && mLooped)
			{
				mFrame = 0;
			}
			else
			{
				mIsFinishedPlaying = true;
			}
		}
	}
}

AnimationFrame AnimationPlayer::GetCurrentAnimationFrame() const
{
	if (mAnimations.empty())
	{
		return AnimationFrame();
	}

	return mAnimations[mCurrentAnimation].GetAnimationFrame(GetCurrentFrameNumber());
}
