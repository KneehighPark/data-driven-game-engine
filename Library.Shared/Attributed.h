#pragma once
#include "Scope.h"

namespace Library
{
	/// <summary>
	/// Attributed class, derives from Scope
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Attributed() = default;

		/// <summary>
		/// Checks whether attribute with given name is in the scope
		/// </summary>
		/// <param name="name">Name of attribute</param>
		/// <returns>True if it is in the scope, false if not</returns>
		bool IsAttribute(const std::string& name) const;
		
		/// <summary>
		/// Checks whether prescribed attribute with given name is in the scope
		/// </summary>
		/// <param name="name">Name of attribute</param>
		/// <returns>True if it is in the scope, false if not</returns>
		bool IsPrescribedAttribute(const std::string& name) const;
		
		/// <summary>
		/// Checks whether auxiliary attribute with given name is in the scope
		/// </summary>
		/// <param name="name">Name of attribute</param>
		/// <returns>True if it is in the scope, false if not</returns>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// Appends an auxiliary attribute to the scope, returns it if it already exists
		/// </summary>
		/// <param name="name">Name of the attribute</param>
		/// <returns>Reference to datum</returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/// <summary>
		/// Accessor method for attributes in Scope
		/// </summary>
		/// <returns>Const vector of pointers to string/datum pairs</returns>
		const Vector<std::pair<std::string, Datum>*> GetAttributes() const;

		/// <summary>
		/// Creates a clone of the object (overriden from Scope)
		/// </summary>
		/// <returns>Pointer to cloned object</returns>
		gsl::owner<Scope*> Clone() const override = 0;

	protected:

		/// <summary>
		/// Default constructor (deleted)
		/// </summary>
		Attributed() = delete;

		/// <summary>
		/// Constructor for attributed
		/// </summary>
		/// <param name="typeID">RTTI Type ID</param>
		explicit Attributed(RTTI::IdType typeID);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const reference to attribute that is to be copied</param>
		Attributed(const Attributed& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to attribute that is to be moved</param>
		Attributed(Attributed&& rhs);

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to attribute that is to be copied</param>
		/// <returns>Reference to attribute</returns>
		Attributed& operator=(const Attributed& rhs);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to attribute that is to be moved</param>
		/// <returns>Reference to attribute</returns>
		Attributed& operator=(Attributed&& rhs);
		
	private:

		/// <summary>
		/// Populates scope from data stored in TypeRegistry
		/// </summary>
		/// <param name="typeID">RTTI Type ID</param>
		void Populate(RTTI::IdType typeID);

		/// <summary>
		/// Updates prescribed attributes in scope
		/// </summary>
		/// <param name="typeID">RTTI Type ID</param>
		void UpdateExternalStorage(RTTI::IdType typeID);

	};
}