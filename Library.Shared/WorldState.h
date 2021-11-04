#pragma once
#include "GameClock.h"
#include "GameTime.h"

namespace Library
{
	class WorldState final
	{
	public:

		/// <summary>
		/// Constructor for WorldState
		/// </summary>
		WorldState();

		/// <summary>
		/// Copy constructor 
		/// </summary>
		/// <param name="rhs">Const reference to WorldState that is to be copied</param>
		WorldState(const WorldState& rhs) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to WorldState that is to be moved</param>
		WorldState(WorldState&& rhs) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to world state that is to be copied</param>
		/// <returns>Reference to WorldState</returns>
		WorldState& operator=(const WorldState& rhs) = default;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to WorldState that is to be moved</param>
		/// <returns>Reference to WorldState</returns>
		WorldState& operator=(WorldState&& rhs) = default;

		/// <summary>
		/// Destructor
		/// </summary>
		~WorldState() = default;

		/// <summary>
		/// Getter method for GameTime
		/// </summary>
		/// <returns>Const reference to GameTime</returns>
		const GameTime& GetGameTime() const;

		/// <summary>
		/// Setter method for GameTime
		/// </summary>
		/// <param name="t_game_time">Const reference to GameTime</param>
		void SetGameTime(const GameTime& gameTime);

		class World* World;		 //Address of World currently being processed
		class Sector* Sector;	 //Address of Sector currently being processed
		class Entity* Entity;	 //Address of Entity currently being processed
		class Action* Action;	 //Address of Action currently being processed

	private:
		GameClock mGameClock;
		GameTime mGameTime;
	};

}