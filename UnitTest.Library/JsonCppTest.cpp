#include "pch.h"
#include <string>
#include <sstream>
#include <ostream>
#include <iostream>
#include <utility>
#include <json/json.h>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(JsonCppTest)
	{
		

	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&s_start_mem_state);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{

#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState end_mem_state, diff_mem_state;
			_CrtMemCheckpoint(&end_mem_state);
			if (_CrtMemDifference(&diff_mem_state, &s_start_mem_state, &end_mem_state))
			{
				_CrtMemDumpStatistics(&diff_mem_state);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Deserialization)
		{
			std::stringstream inputStream;
			inputStream << R"({ "Health": { "type": "integer", "value": 50 }})";

			Json::Value val;
			inputStream >> val;

			std::stringstream outputStream;
			OutputJsonToStringStream(outputStream, val);
			Logger::WriteMessage(outputStream.str().c_str());
			
		}

		void Indent(std::ostream& output, int indent)
		{
			for (int i = 0; i < indent; ++i)
			{
				output << ' ';
			}
		}

		void OutputJsonToStringStream(std::ostream& outputStream, const Json::Value& val, int indent = 0)
		{
			switch (val.type())
			{
			case Json::nullValue:
				outputStream << "null";
				break;
			case Json::booleanValue:
				outputStream << (val.asBool() ? "true" : "false");
				break;
			case Json::intValue:
				outputStream << val.asLargestInt();
				break;
			case Json::uintValue:
				outputStream << val.asLargestUInt();
				break;
			case Json::realValue:
				outputStream << val.asDouble();
				break;
			case Json::stringValue:
				outputStream << '"' << val.asString() << '"';
				break;
			case Json::arrayValue:
			{
				Json::ArrayIndex size = val.size();
				if (size == 0)
				{
					outputStream << "[]";
				}
				else
				{
					outputStream << "[\n";
					int newIndent = indent + 4;
					for (Json::ArrayIndex i = 0; i < size; ++i)
					{
						Indent(outputStream, newIndent);
						OutputJsonToStringStream(outputStream, val[i], newIndent);
						outputStream << (i + 1 == size ? "\n" : ",\n");
					}
					Indent(outputStream, indent);
					outputStream << ']';
				}
				break;
			}
			case Json::objectValue:
			{
				if (val.empty())
				{
					outputStream << "{}";
				}
				else
				{
					outputStream << "{\n";
					int newIndent = indent + 4;
					std::vector<std::string> keys = val.getMemberNames();
					for (size_t i = 0; i < keys.size(); ++i)
					{
						Indent(outputStream, newIndent);
						const std::string& key = keys[i];
						outputStream << '"' << key << '"' << " : ";
						OutputJsonToStringStream(outputStream, val[key], newIndent);
						outputStream << (i + 1 == keys.size() ? "\n" : ",\n");
					}
					Indent(outputStream, indent);
					outputStream << '}';
				}
				break;
			}
			default:
				std::cerr << "Wrong type!" << std::endl;
				exit(0);

			}
		}


		

	private:
		static _CrtMemState s_start_mem_state;
	};
	_CrtMemState JsonCppTest::s_start_mem_state;
}