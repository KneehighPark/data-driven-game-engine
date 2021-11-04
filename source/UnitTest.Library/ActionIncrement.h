#pragma once
#include "Action.h"
#include "Entity.h"

namespace UnitTests
{
	class ActionIncrement final : public Library::Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:
		
		ActionIncrement();
		explicit ActionIncrement(const std::string& name);
		ActionIncrement(const ActionIncrement& t_rhs) = default;
		ActionIncrement(ActionIncrement&& t_rhs) = default;
		ActionIncrement& operator=(const ActionIncrement& t_rhs) = default;
		ActionIncrement& operator=(ActionIncrement&& t_rhs) = default;
		virtual ~ActionIncrement() = default;
		
		virtual void Update(Library::WorldState& t_world_state) override;
		static Library::Vector<Library::Signature> Signatures();
		virtual gsl::owner<Scope*> Clone() const override;

	private:

		std::string mTarget;
		Library::Datum* mReference = nullptr;
	};

	CONCRETE_FACTORY(ActionIncrement, Library::Scope)
}