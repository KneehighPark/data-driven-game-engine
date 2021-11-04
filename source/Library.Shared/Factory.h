#pragma once
#include "HashMap.h"

namespace Library
{
	template <typename AbstractProductType>
	class Factory
	{
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		Factory() = default;
		
		/// <summary>
		/// Default destructor
		/// </summary>
		virtual ~Factory() = default;

		/// <summary>
		/// Deleted move/copy semantics 
		/// </summary>
		Factory(const Factory& rhs) = delete;
		Factory(Factory&& rhs) = delete;
		Factory& operator= (const Factory& rhs) = delete;
		Factory& operator= (Factory&& rhs) = delete;

		/// <summary>
		/// Finds a concrete factory from the hashmap of factories
		/// </summary>
		/// <param name="name">Const reference to name of factory to be found</param>
		/// <returns>Pointer to factory if found, nullptr if not</returns>
		static Factory* Find(const std::string& name);

		/// <summary>
		/// Creates concrete product associated with a concrete factory class
		/// </summary>
		/// <param name="name">Const reference to name of associated factory</param>
		/// <returns>Pointer to an abstract product type</returns>
		static gsl::owner<AbstractProductType*> Create(const std::string& name);
		
		/// <summary>
		/// Gets size of factories hashmap (no. of elements)
		/// </summary>
		/// <returns>Size of the factories hashmap</returns>
		static size_t GetFactoryTableSize();
		
		/// <summary>
		/// Creates concrete product associated with a concrete factory class (pure virtual function)
		/// </summary>
		/// <returns>Pointer to an abstract product type</returns>
		virtual gsl::owner<AbstractProductType*> Create() = 0;
		
		/// <summary>
		/// Gets the name of the class the factory instantiates.
		/// </summary>
		/// <returns>Const std::string representing the name of the product class</returns>
		virtual const std::string& ClassName() const = 0;

	protected:

		/// <summary>
		/// Given a reference to a concrete factory, add it to the list of factories for this abstract factory 
		/// </summary>
		/// <param name="factory"></param>
		static void Add(Factory& factory);
		
		/// <summary>
		/// Given a reference to a concrete factory, remove it from the list of factories for this abstract factory. 
		/// </summary>
		/// <param name="factory"></param>
		static void Remove(Factory& factory);

	private:
		static HashMap<std::string, Factory<AbstractProductType>*> mFactoriesTable;
	};

#define CONCRETE_FACTORY(ConcreteProductType, AbstractProductType)												\
	class ConcreteProductType##Factory final: public Library::Factory<AbstractProductType>						\
	{																											\
	public:																										\
		ConcreteProductType##Factory() : mClassName(#ConcreteProductType)										\
		{																										\
			Add(*this);																							\
		}																										\
		~ConcreteProductType##Factory()																			\
		{																										\
			Remove(*this);																						\
		}																										\
		virtual const std::string& ClassName() const override													\
		{																										\
			return mClassName;																					\
		}																										\
		virtual gsl::owner<AbstractProductType*> Create() override												\
		{																										\
			return new ConcreteProductType();																	\
		}																										\
		ConcreteProductType##Factory(const ConcreteProductType##Factory& rhs) = delete;							\
		ConcreteProductType##Factory(ConcreteProductType##Factory&& rhs) = delete;								\
		ConcreteProductType##Factory& operator= (const ConcreteProductType##Factory& rhs) = delete;				\
		ConcreteProductType##Factory& operator= (ConcreteProductType##Factory&& rhs) = delete;					\
																												\
	private:																									\
		const std::string mClassName;																			\
	};																											
}			
#include "Factory.inl"

