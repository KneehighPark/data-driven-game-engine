#pragma once
#include "ActionList.h"
#include "EventSubscriber.h"

namespace Library
{
	/// <summary>
	/// Abstract base class for reactions that iterates through and calls update on the list of actions when notified
	/// Extends both ActionList and EventSubscriber
	/// </summary>
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)

	public:
		/// <summary>
		/// Gets signatures to pass to TypeRegistry
		/// </summary>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Reaction() = default;

		/// <summary>
		/// Clones a reaction
		/// </summary>
		/// <returns>Cloned Reaction pointer</returns>
		virtual gsl::owner<Reaction*> Clone() const override = 0;

		/// <summary>
		/// No implementation
		/// </summary>
		/// <param name="worldState">Reference to world state</param>
		virtual void Update(WorldState&) override;

		/// <summary>
		/// Any derived actions must implement a version of "Notify"
		/// </summary>
		/// <param name=""></param>
		virtual void Notify(const EventPublisher&) override = 0;

	protected:

		/// <summary>
		/// Deleted default constructor
		/// </summary>
		Reaction() = delete;

		/// <summary>
		/// Parameterized constructor
		/// </summary>
		/// <param name="type">RTTI Type ID</param>
		/// <param name="name">Name</param>
		Reaction(RTTI::IdType type, const std::string& name);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const L-value reference to reaction that's to be copied</param>
		Reaction(const Reaction& rhs) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to reaciton that's to be moved</param>
		Reaction(Reaction&& rhs) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const L-value reference to reaction that's to be copied</param>
		/// <returns>Reference to newly copied reaction</returns>
		Reaction& operator=(const Reaction& rhs) = default;

		/// <summary>
		/// Move asignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to reaciton that's to be moved</param>
		/// <returns>Reference to newly moved reaction</returns>
		Reaction& operator=(Reaction&& rhs) = default;
	};
}