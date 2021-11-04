#pragma once
#include "ActionList.h"

namespace Library
{
	class ActionListIf : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)
	
		static const std::string ConditionKey;
		static const std::string ThenKey;
		static const std::string ElseKey;
		static const std::size_t ThenIndex;
		static const std::size_t ElseIndex;


	public:
		/// <summary>Default constructor</summary>
		ActionListIf();

		/// <summary>
		/// Parameterized constructor
		/// </summary>
		/// <param name="name">Const reference to string name with which ActionList will be initialized</param>
		explicit ActionListIf(const std::string& name);

		/// <summary>Defaulted destructor</summary>
		virtual ~ActionListIf() = default;

		/// <summary>Defaulted copy constructor</summary>
		/// <param name="rhs">Const reference to ActionListIf that is to be copied.</param>
		ActionListIf(const ActionListIf& rhs) = default;

		/// <summary>Defaulted move constructor</summary>
		/// <param name="rhs">R-Value reference to ActionListIf that is to be moved.</param>
		ActionListIf(ActionListIf&& rhs) = default;

		/// <summary>Defaulted copy assignment operator</summary>
		/// <param name="rhs">Const reference to ActionListIf that is to be copied.</param>
		/// <returns>Reference to ActionListIf.</returns>
		ActionListIf& operator=(const ActionListIf& rhs) = default;

		/// <summary>Defaulted move assignment operator</summary>
		/// <param name="rhs">R-Value reference to ActionListIf that is to be moved.</param>
		/// <returns>Reference to ActionListIf.</returns>
		ActionListIf& operator=(ActionListIf&& rhs) = default;

		/// <summary>Update method for ActionListIf</summary>
		/// <param name="worldState"> WorldState reference </param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>Sets condition value</summary>
		/// <param name="condition">Integer that's to be assigned for if/else condition</param>
		void SetCondition(const size_t condition);

		/// <summary>Sets the If block</summary>
		/// <param name="ifAction">Reference to an Action that will be assigned for "if" condition</param>
		void SetIfBlock(Action& ifAction);

		/// <summary>Sets the Else block</summary>
		/// <param name="elseAction">Reference to an Action that will be assigned for "else" condition</param>
		void SetElseBlock(Action& elseAction);

		/// <summary>Static method to get Signatures that will be passed to the Type Registry</summary>
		/// <returns>Vector of Signatures</returns>
		static Vector<Signature> Signatures();

		/// <summary>Create clone of an ActionListIf</summary>
		/// <returns>Returns ActionListIf clone</returns>
		virtual gsl::owner<Scope*> Clone() const override;

	private:
		size_t mCondition;
	};

	CONCRETE_FACTORY(ActionListIf, Scope)
}