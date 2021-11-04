#pragma once

//Standard
#include <cstdint>
#include <string>
#include <crtdbg.h>
#include <stdexcept>
#include <cassert>
#include <fstream>
#include <json/json.h>

#include "RTTI.h"
#include "Foo.h"
#include "Bar.h"
#include "SList.h"
#include "Vector.h"
#include "HashFunctions.h"
#include "HashMap.h"
#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "AttributedFoo.h"
#include "Factory.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "JsonTableParseHelper.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "Event.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"
#include "EventQueue.h"
#include "SubscriberFoo.h"
#include "SubscriberBar.h"
#include "Reaction.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"
#include "ActionEvent.h"
#include "EnqueueEventSubscriber.h"
#include "CppUnitTest.h"

inline std::size_t operator "" _z(unsigned long long int x)
{
	return static_cast<size_t>(x);
}