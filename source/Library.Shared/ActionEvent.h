#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	/// <summary>
	/// Action that queues an event
	/// Extends Action
	/// </summary>
	class ActionEvent final : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)

	public:
		
		static const std::string SubtypeKey;
		static const std::string DelayKey;

		/// <summary>
		/// Gets signatures to be passed to TypeRegistry
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

	public:

		/// <summary>
		/// Parameterized constructor
		/// </summary>
		/// <param name="subtype">Subtype</param>
		/// <param name="name">Name</param>
		explicit ActionEvent(const std::string& subtype = std::string(), const std::string& name = std::string());

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const L-value reference to ActionEvent that's to be copied</param>
		ActionEvent(const ActionEvent& rhs) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to ActionEvent that's to be moved</param>
		ActionEvent(ActionEvent&& rhs) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const L-value reference to ActionEvent that's to be copied</param>
		/// <returns>Reference to newly copied ActionEvent</returns>
		ActionEvent& operator=(const ActionEvent& rhs) = default;

		/// <summary>
		/// Move asignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to ActionEvent that's to be moved</param>
		/// <returns>Reference to newly moved ActionEvent</returns>
		ActionEvent& operator=(ActionEvent&& rhs) = default;

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~ActionEvent() = default;

		/// <summary>
		/// Clones this ActionEvent
		/// </summary>
		/// <returns>Pointer to ActionEvent</returns>
		virtual gsl::owner<ActionEvent*> Clone() const override;

		/// <summary>
		/// Create an attributed event, assign its world and subtype, 
		/// copy all auxiliary parameters into the event and queue the event with the given delay.
		/// </summary>
		/// <param name="worldState">Reference to WorldState</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// Getter for Subtype
		/// </summary>
		/// <returns>Subtype</returns>
		std::string& Subtype();

		/// <summary>
		/// Getter for Subtype (Const)
		/// </summary>
		/// <returns>Subtype</returns>
		const std::string& Subtype() const;

		/// <summary>
		/// Setter for Subtype
		/// </summary>
		/// <param name="subtype">Subtype</param>
		void SetSubtype(const std::string& subtype);

		/// <summary>
		/// Getter for Delay
		/// </summary>
		/// <returns>Dekay</returns>
		std::size_t GetDelay() const;

		/// <summary>
		/// Setter for Delay
		/// </summary>
		/// <param name="delay">Delay</param>
		void SetDelay(const std::size_t delay);

	private:
		/// <summary>
		/// Subtype of event that's to be queued
		/// </summary>
		std::string mSubtype;

		/// <summary>
		/// Delay of event that's to be queued
		/// </summary>
		std::size_t mDelay;
	};

	CONCRETE_FACTORY(ActionEvent, Scope)
}