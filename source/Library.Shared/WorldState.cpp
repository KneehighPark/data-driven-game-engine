#include "pch.h"
#include "WorldState.h"

namespace Library
{
	WorldState::WorldState() : mGameClock(), mGameTime(), World(nullptr), Sector(nullptr), Entity(nullptr), Action(nullptr)
	{

	}

	const GameTime& WorldState::GetGameTime() const
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(const GameTime& gameTime)
	{
		mGameTime = gameTime;
	}
}