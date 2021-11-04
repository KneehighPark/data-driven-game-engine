#pragma once
#include "Attributed.h"
#include "TypeRegistry.h"

namespace Library
{
	/// <summary>
	/// Forward declaration of World class
	/// </summary>
	class World;

	/// <summary>
	/// EventMessageAttributed
	/// Extends Attributed
	/// </summary>
	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)

	public:
		
		static const std::string SubtypeKey;

		/// <summary>
		/// Signatures to be passed to TypeRegistry
		/// </summary>
		static Vector<Signature> Signatures();

	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		EventMessageAttributed();

		/// <summary>
		/// Parameterized constructor
		/// </summary>
		/// <param name="world">world</param>
		explicit EventMessageAttributed(World& world);

		/// <summary>
		/// Parameterized constructor
		/// </summary>
		/// <param name="world">World</param>
		/// <param name="subtype">Subtype</param>
		EventMessageAttributed(World& world, const std::string& subtype);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const L-value reference to EventMessageAttributed that's to be copied</param>
		EventMessageAttributed(const EventMessageAttributed& rhs) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to EventMessageAttributed that's to be moved</param>
		EventMessageAttributed(EventMessageAttributed && rhs) = default;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const L-value reference to EventMessageAttributed that's to be copied</param>
		/// <returns>Reference to newly copied EventMessageAttributed</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed & rhs) = default;

		/// <summary>
		/// Move asignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to EventMessageAttributed that's to be moved</param>
		/// <returns>Reference to newly moved EventMessageAttributed</returns>
		EventMessageAttributed& operator=(EventMessageAttributed && rhs) = default;

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~EventMessageAttributed() = default;

		/// <summary>
		/// Clones this EventMessageAttributed
		/// </summary>
		/// <returns>Pointer to cloned EventMessageAttributed</returns>
		virtual gsl::owner<EventMessageAttributed*> Clone() const override;

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
		/// Getter for World
		/// </summary>
		/// <returns>World pointer</returns>
		World* GetWorld() const;

		/// <summary>
		/// Setter for world
		/// </summary>
		/// <param name="world">Reference to world</param>
		void SetWorld(World& world);

	private:
		/// <summary>
		/// String used to disambiguate event 
		/// </summary>
		std::string mSubtype;

		/// <summary>
		/// The world that will process this message
		/// </summary>
		World* mWorld = nullptr;
	};
}