#pragma once
#include "Reaction.h"

namespace Library
{
	/// <summary>
	/// Subscribes to Event<EventMessageAttributed> events.
	/// Extends Reaction
	/// </summary>
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

	public:

		static const std::string SubtypeKey;
		static const std::size_t SubtypeIndex;

		/// <summary>
		/// Gets signatures to be passed to TypeRegistry
		/// </summary>
		/// <returns>Signatures</returns>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// Parameterized constructor, subscribes to publisher
		/// </summary>
		/// <param name="subtype">Subtype</param>
		/// <param name="name">Name</param>
		explicit ReactionAttributed(const std::string& subtype = std::string(), const std::string& name = std::string());

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const L-value reference to ReactionAttributed that's to be copied</param>
		ReactionAttributed(const ReactionAttributed& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to ReactionAttributed that's to be moved</param>
		ReactionAttributed(ReactionAttributed&& rhs);

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const L-value reference to ReactionAttributed that's to be copied</param>
		/// <returns>Reference to newly copied ReactionAttributed</returns>
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = default;

		/// <summary>
		/// Move asignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to ReactionAttributed that's to be moved</param>
		/// <returns>Reference to newly moved ReactionAttributed</returns>
		ReactionAttributed& operator=(ReactionAttributed&& rhs) = default;

		/// <summary>
		/// Destructor, unsubscribes from publisher
		/// </summary>
		virtual ~ReactionAttributed();

		/// <summary>
		/// Clones this ReactionAttributed
		/// </summary>
		/// <returns>Pointer to ReactionAttributed</returns>
		virtual gsl::owner<ReactionAttributed*> Clone() const override;

		/// <summary>
		/// Accept attributed events. 
		/// If the event subtype matches the reaction subtype, 
		/// copy the attribute “arguments” to this instance of ReactionAttribute, 
		/// and then execute ActionList::Update.
		/// </summary>
		/// <param name="event">Event/param>
		virtual void Notify(const EventPublisher& event) override;

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

	private:
		/// <summary>
		/// Datum of type integer or string indicating value of event subtype to which this Reaction responds. 
		/// </summary>
		std::string mSubtype;
	};

	CONCRETE_FACTORY(ReactionAttributed, Scope)
}