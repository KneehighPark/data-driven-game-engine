#pragma once
#include "RTTI.h"
#include "Datum.h"
#include "HashMap.h"
#include "Vector.h"
#include <gsl/gsl>

namespace Library
{
	/// <summary>
	/// Scope class which implements the RTTI interface
	/// </summary>
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)
	
	public:
		using LookupTable = HashMap<std::string, Datum>;
		using LookupTableEntry = LookupTable::PairType;
		using PointersVector = Vector<LookupTableEntry*>;

		/// <summary>
		/// Parameterized constructor
		/// </summary>
		/// <param name="size">Size for HashMap</param>
		explicit Scope(size_t size = 11);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="rhs">Const reference to scope that is to be copied</param>
		Scope(const Scope& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R-value reference to scope that is to be moved</param>
		Scope(Scope&& rhs);

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to scope that is to be copied</param>
		/// <returns>Reference to scope</returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to scope that is to be moved</param>
		/// <returns>Reference to scope</returns>
		Scope& operator=(Scope&& rhs);

		/// <summary>
		/// Destructor for Scope
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Recursively deletes the contents within a scope
		/// </summary>
		virtual void Clear();

		/// <summary>
		/// Wrapper for Append
		/// </summary>
		/// <param name="name">Const reference to name of the datum that's to be added</param>
		/// <returns>Reference of the datum in the Scope</returns>
		Datum& operator[](const std::string& name);

		/// <summary>
		/// Const version of operator[std::string] -- Wraps At
		/// </summary>
		/// <param name="name">Const reference to name of the datum that's to be added</param>
		/// <returns>Const reference of the datum in the Scope</returns>
		const Datum& operator[](const std::string& name) const;

		/// <summary>
		/// Provides reference to datum at provided index
		/// </summary>
		/// <param name="index">Index, corresponding to order that items were appended</param>
		/// <returns>Reference to datum</returns>
		Datum& operator[](const size_t index);
		
		/// <summary>
		/// Const version of operator[size_t]
		/// </summary>
		/// <param name="index">Index, corresponding to order that items were appended</param>
		/// <returns>Const reference to Datum</returns>
		const Datum& operator[](const size_t index) const;

		/// <summary>
		/// Wrapper for Find()
		/// </summary>
		/// <param name="name">Const reference to std::string -- name which is to be searched</param>
		/// <returns>Reference to the datum if found</returns>
		Datum& At(const std::string& name);

		/// <summary>
		/// Const version of At()
		/// </summary>
		/// <param name="name">Const reference to std::string -- name which is to be searched</param>
		/// <returns>Reference to the datum if found</returns>
		const Datum& At(const std::string& name) const;

		/// <summary>
		/// Comparison operator for Scope -- does check of nested scopes and children
		/// </summary>
		/// <param name="rhs">Const reference to Scope that's to be compared to</param>
		/// <returns>True if equal, false if not</returns>
		bool operator==(const Scope& rhs) const;

		/// <summary>
		/// Not equal operator -- does check of nested scopes and children
		/// </summary>
		/// <param name="rhs">Const reference to Scope that's to be compared to</param>
		/// <returns>False if equal, true if not</returns>
		bool operator!=(const Scope& rhs) const;

		/// <summary>
		/// Finds a datum stored in the current scope, searches by given name
		/// </summary>
		/// <param name="name">Given name</param>
		/// <returns>Pointer to datum if found, returns nullptr if not</returns>
		Datum* Find(const std::string& name);

		/// <summary>
		/// Const version of find
		/// </summary>
		/// <param name="name">Given name</param>
		/// <returns>Const pointer to datum if found, returns nullptr if not</returns>
		const Datum* Find(const std::string& name) const;

		/// <summary>
		/// Returns address of the most-closely nested Datum associated with given name or its ancestors
		/// </summary>
		/// <param name="name">Const reference to name that will be searched</param>
		/// <param name="owner">Address of scope object which contains element if it's found</param>
		/// <returns>Address of most-closely nested Datum nullptr if not found</returns>
		Datum* Search(const std::string& name, Scope** owner = nullptr);

		/// <summary>
		/// Adds datum to Scope if it doesn't already exist
		/// </summary>
		/// <param name="name">Name of the datum which is to be added</param>
		/// <returns>Reference to datum in the scope</returns>
		Datum& Append(const std::string& name);

		/// <summary>
		/// Adds scope to a scope if it doesn't already exist
		/// </summary>
		/// <param name="name">Name of datum which scope will be added to</param>
		/// <returns>Reference to scope</returns>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		/// Adopts the passed scope, stored in Datum with passed name
		/// </summary>
		/// <param name="child">Reference to child scope</param>
		/// <param name="name">Const reference to name that of Datum that will store the child</param>
		void Adopt(Scope& child, const std::string& name);

		/// <summary>
		/// Gets parent of scope
		/// </summary>
		/// <returns>Pointer to parent scope</returns>
		Scope* GetParent() const;

		/// <summary>
		/// Finds contained scope
		/// </summary>
		/// <param name="t_child">Constant address of scope to be found</param>
		/// <returns>std::pair of Datum pointer and index at which Scope was found</returns>
		std::pair<Datum*, size_t> FindContainedScope(const Scope& t_child) const;

		/// <summary>
		/// Determines whether provided Scope is an ancestor
		/// </summary>
		/// <param name="scope">Const reference to provided scope</param>
		/// <returns>True if provided Scope is an ancestor, false if not</returns>
		bool IsAncestorOf(const Scope& scope) const;

		/// <summary>
		/// Determines whether provided Scope is a descendent
		/// </summary>
		/// <param name="scope">Const reference to provided scope</param>
		/// <returns>True if provided Scope is an descendent, false if not</returns>
		bool IsDescendentOf(const Scope& scope) const;

		/// <summary>
		/// Determines whether RTTI is a Scope or not -- Overriden from RTTI Interface
		/// </summary>
		/// <param name="rhs">Const pointer to RTTI object</param>
		/// <returns>True if RTTI is Scope, false if not</returns>
		virtual bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Provides name of class -- Overriden from RTTI Interface
		/// </summary>
		/// <returns>Name of the class as a std::string</returns>
		virtual std::string ToString() const override;

		/// <summary>
		/// Returns size of scope (# of entries)
		/// </summary>
		/// <returns></returns>
		size_t Size() const;

		virtual gsl::owner<Scope*> Clone() const;

	protected:
		
		const Vector<std::pair<std::string, Datum>*> GetPointersList() const;
		void RecursivelyCopyChilden(const Scope& rhs);
		void FixParentPointers(Scope&& rhs);
		void Orphan();

		Scope* mParent = nullptr;
		LookupTable mLookupTable;
		PointersVector mPointersVector;

	};
}