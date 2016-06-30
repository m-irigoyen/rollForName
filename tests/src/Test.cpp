#include "Test.h"

#include <rollForName/Logger.h>
#include <rollForName/Random.h>
#include <rollForName/GameDictionary.h>
#include <rollForName/Parser.h>

#include <iostream>

#define ROLL_NB 10000	// nb of rolls done for each roll test
//#define ROLL_NB 1	// nb of rolls done for each roll test
#define ERRORTAG_TEST "Tests"
#define ERRORTAG_TEST_L L"Tests"

namespace rfn
{
	//! @class Tester
	//! @brief The Tester template class
	template <typename ResultType>
	class Tester
	{
		typedef std::pair<ustring, ResultType> TAR;
		typedef std::vector<std::pair<ustring, ResultType>> TARList;
		typedef std::vector<ResultType> RList;

	public :
		//! Tests the given testSet. Any failed TAR is put in the failed TARList. If displayFailed,
		// failed tests are displayed in console
		template <class Functor>
		float test(
			TARList testSet
			, TARList& failedSet
			, RList& failedResults
			, Functor & functor
			, void * data = NULL);
	};

	template<typename ResultType>
	template<class Functor>
	float Tester<ResultType>::test(
		TARList testSet
		, TARList& failedSet
		, RList& failedResults
		, Functor & functor
		, void * data)
	{
		int nbFails = 0;
		for (TAR tar : testSet)
		{
			ResultType result;
			if (!functor(tar.first, tar.second, data, result))
			{
				++nbFails;
				failedSet.push_back(tar);
				failedResults.push_back(result);
			}
		}

		return (1.0f - (float)nbFails / (float)testSet.size()) * 100.0f;
	}


	//---------
	// FUNCTORS
	//---------

	struct FunctorRollTest
	{
			bool operator()(ustring test, Range expected, void*, Range& result)
			{
				result.min = ROLL_NB;
				result.max = -ROLL_NB;

				int t;
				for (int i = 0; i < ROLL_NB; ++i)
				{
					if (roll(test, t))
					{
						if (t > result.max)
							result.max = t;
						if (t < result.min)
							result.min = t;
					}
					else
					{
						return false;
					}
				}

				return expected == result;
			}
	};

	// Test range
	struct FunctorRangeTest
	{
		bool operator()(ustring test, Range expected, void*, Range& result)
		{
			if (Parser::parseRange(test, result))
			{
				return expected == result;
			}
			else
			{
				return false;
			}
		}
	};

	// Test tableentry parsing
	struct FunctorTableEntryTest
	{
		bool operator()(ustring test, TableEntry expected, void*, TableEntry& result)
		{
			if (Parser::parseRange(test, result))
			{
				return expected == result;
			}
			else
			{
				return false;
			}
		}
	};

	//struct FunctorNameIdTest
	//{
	//	bool operator()(ustring test, bool expected, void*, bool& result)
	//	{
	//		result = isValidId(test);
	//		return result == expected;
	//	}
	//};

	//struct FunctorVariableAndConditionalTextTest
	//{
	//	bool operator()(ustring test, ustring expected, void* interpreter, ustring& result)
	//	{
	//		Interpreter* cast = (Interpreter*)interpreter;
	//		result = cast->interpret(test);
	//		return compare(result, expected);
	//	}
	//};

	//struct FunctorActionTest
	//{
	//	bool operator()(ustring test, ustring expected, void* interpreter, ustring& result)
	//	{
	//		Interpreter* cast = (Interpreter*)interpreter;
	//		bool applied = cast->applyAction(test);
	//		if (applied)
	//		{
	//			bool bResult;
	//			ustring expectedResolved;
	//			if (cast->resolveToBool(expected, bResult))
	//			{
	//				return bResult;
	//			}
	//		}
	//		return false;
	//	}
	//};

	//struct FunctorConditionTest
	//{
	//	bool operator()(ustring test, bool expected, void* interpreter, bool& result)
	//	{
	//		Interpreter* cast = (Interpreter*)interpreter;
	//		if (cast->resolveToBool(test, result))
	//		{
	//			return expected == result;
	//		}
	//		else
	//		{
	//			return false;
	//		}
	//	}
	//};


	//// ALL TESTS
	float Test::testAll(bool displayFailed)
	{
		Logger::logNewLine();

		float global = 0.0f;

		std::vector<float> results;
		results.push_back(testRoll(displayFailed));
		results.push_back(testTableParse(displayFailed));
		results.push_back(testTableName(displayFailed));
		results.push_back(testTableRequired(displayFailed));
		results.push_back(testTableRange(displayFailed));

		for (float f : results)
		{
			global += f;
		}
		global /= results.size();

		Logger::logNewLine();
		Logger::logs("Global engine validity : " + std::to_string(global) + " %", ERRORTAG_TEST);
		Logger::logNewLine();

		return global;
	}

	float Test::testRoll(bool displayFailed)
	{
		typedef std::pair<ustring, Range> TAR;
		typedef std::vector<TAR> TARList;
		typedef std::vector<Range> RList;

		TARList testSet;
		TARList failedSet;
		RList failedResults;

		GameDictionary dictionary;
		dictionary.set(L"key", L"testValue");
		dictionary.set(L"key2", L"testValue2");
		dictionary.set(L"Digit", L"0");
		dictionary.set(L"Number", L"481516");
		dictionary.set(L"Pi", L"3.1415");

		// Basic rolls
		{
			testSet.push_back(TAR(L"[1d6]", Range(1, 6)));
			testSet.push_back(TAR(L"[2d6]", Range(2,12)));
			testSet.push_back(TAR(L"[1d12]", Range(1, 12)));
			testSet.push_back(TAR(L"[12d2]", Range(12, 24)));
			testSet.push_back(TAR(L"[1d20]", Range(1, 20)));
		}

		// basic modifiers
		{
			testSet.push_back(TAR(L"[1d20 + 5]", Range(6, 25)));
			testSet.push_back(TAR(L"[4d6 - 5]", Range(-1, 19)));
			testSet.push_back(TAR(L"[1d2 + 17 - 2]", Range(16, 17)));
		}

		// Complex rolls
		{
			testSet.push_back(TAR(L"[1d20 + 4d2 + 17]", Range(22, 45)));
			testSet.push_back(TAR(L"[1d1 + 1d2+ 1d3 + 1d4 +1d5]", Range(5, 15)));
		}

		Tester<Range> t;
		float result = t.test(testSet, failedSet, failedResults, FunctorRollTest());

		if (displayFailed)
		{
			if (!failedSet.empty())
			{
				for (int i = 0; i < failedSet.size(); ++i)
				{
					Logger::logs(L"\n----Test     : "
						+ failedSet[i].first + L"\n"
						+ L"----Expected : " + (failedSet[i].second).toUstring() + L"\n"
						+ L"----Got      : " + failedResults[i].toUstring().data()
						, ERRORTAG_TEST_L
						, L"condition resolution tests");
				}
			}
		}

		Logger::logs("Condition resolution results : " + std::to_string(result) + " %"
			, ERRORTAG_TEST);
		return result;
	}

	float Test::testTableParse(bool displayFailed)
	{
		typedef std::pair<ustring, Table> TAR;
		typedef std::vector<TAR> TARList;
		typedef std::vector<Table> RList;

		TARList testSet;
		TARList failedSet;
		RList failedResults;

		GameDictionary dictionary;
		dictionary.set(L"key", L"testValue");
		dictionary.set(L"key2", L"testValue2");
		dictionary.set(L"Digit", L"0");
		dictionary.set(L"Number", L"481516");
		dictionary.set(L"Pi", L"3.1415");

		// Simple table
		{
			Table t;
			t.name_ = L"firsttest";
			t.roll_ = L"[1d6]";
			t.entries_.push_back(TableEntry(Range(1, 3), L"entry1", L"This is the first entry", L""));
			t.entries_.push_back(TableEntry(Range(4, 6), L"entry2", L"This is the second entry", L""));

			ustring line =
				L"FirstTest\n	[1d6]\n	{\n	* <1_3> \"entry1\" : \"This is the first entry\"\n		* <3_6> \"entry2\" : \"This is the second entry\"\n }\n";
			testSet.push_back(TAR(L"[1d6]", Range(1, 6)));
		}

		// basic modifiers
		{
			testSet.push_back(TAR(L"[1d20 + 5]", Range(6, 25)));
			testSet.push_back(TAR(L"[4d6 - 5]", Range(-1, 19)));
			testSet.push_back(TAR(L"[1d2 + 17 - 2]", Range(16, 17)));
		}

		// Complex rolls
		{
			testSet.push_back(TAR(L"[1d20 + 4d2 + 17]", Range(22, 45)));
			testSet.push_back(TAR(L"[1d1 + 1d2+ 1d3 + 1d4 +1d5]", Range(5, 15)));
		}

		Tester<Range> t;
		float result = t.test(testSet, failedSet, failedResults, FunctorRollTest());

		if (displayFailed)
		{
			if (!failedSet.empty())
			{
				for (int i = 0; i < failedSet.size(); ++i)
				{
					Logger::logs(L"\n----Test     : "
						+ failedSet[i].first + L"\n"
						+ L"----Expected : " + (failedSet[i].second).toUstring() + L"\n"
						+ L"----Got      : " + failedResults[i].toUstring().data()
						, ERRORTAG_TEST_L
						, L"condition resolution tests");
				}
			}
		}

		Logger::logs("Condition resolution results : " + std::to_string(result) + " %"
			, ERRORTAG_TEST);
		return result;
	}

	float Test::testTableName(bool displayFailed)
	{
		return 0.0f;
	}

	float Test::testTableRange(bool displayFailed)
	{
		return 0.0f;
	}

	//// Action resolution
	//float Test::testActionResolution(bool displayFailed)
	//{
	//	// This is used as "write an action, then write a condition to check its result. If the condition
	//	// returns true after the action was applied, then its a success
	//	typedef std::pair<ustring, ustring> TAR;
	//	typedef std::vector<TAR> TARList;
	//	typedef std::vector<ustring> RList;

	//	GameDictionary dictionary;
	//	dictionary.set(L"key", L"testValue");
	//	dictionary.set(L"key2", L"testValue2");
	//	dictionary.set(L"Digit", L"0");
	//	dictionary.set(L"Number", L"481516");
	//	dictionary.set(L"Pi", L"3.1415");

	//	Interpreter interpret(dictionary, std::vector<bool>());

	//	TARList testSet;
	//	TARList failedSet;
	//	RList failedResults;

	//	// Basic affectations
	//	{
	//		testSet.push_back(TAR(L"$test = 1", L"$test == 1"));
	//		testSet.push_back(TAR(L"$test = 0", L"$test == 0"));
	//		testSet.push_back(TAR(L"$test = -7", L"$test == -7"));
	//		testSet.push_back(TAR(L"$test = 2", L"$test == 2"));
	//		testSet.push_back(TAR(L"$test *= 4", L"$test == 8"));
	//		testSet.push_back(TAR(L"$test /= 2", L"$test == 4"));
	//		testSet.push_back(TAR(L"$test %= 3", L"$test == 1"));
	//		testSet.push_back(TAR(L"$test += 17", L"$test == 18"));
	//		testSet.push_back(TAR(L"$test -= 6", L"$test == 12"));
	//	}


	//	Tester<ustring> t;
	//	float result = t.test(testSet, failedSet, failedResults, FunctorActionTest());

	//	if (displayFailed)
	//	{
	//		if (!failedSet.empty())
	//		{
	//			for (int i = 0; i < failedSet.size(); ++i)
	//			{
	//				Logger::logs(L"\n----Test     : "
	//					+ failedSet[i].first + L"\n"
	//					+ L"----Expected : " + failedSet[i].second + L"\n"
	//					, ERRORTAG_TEST_L
	//					, L"condition resolution tests");
	//			}
	//		}
	//	}

	//	Logger::logs("Condition resolution results : " + std::to_string(result) + " %"
	//		, ERRORTAG_TEST);
	//	return result;
	//}

	//// Condition resolution
	//float Test::testConditionResolution(bool displayFailed)
	//{
	//	typedef std::pair<ustring, bool> TAR;
	//	typedef std::vector<TAR> TARList;
	//	typedef std::vector<bool> RList;

	//	GameDictionary dictionary;
	//	dictionary.set(L"key", L"testValue");
	//	dictionary.set(L"key2", L"testValue2");
	//	dictionary.set(L"Digit", L"0");
	//	dictionary.set(L"Number", L"481516");
	//	dictionary.set(L"Pi", L"3.1415");

	//	Interpreter interpret(dictionary, std::vector<bool>());

	//	TARList testSet;
	//	TARList failedSet;
	//	RList failedResults;

	//	// Basic values
	//	{
	//		testSet.push_back(TAR(L"", false));
	//		testSet.push_back(TAR(L"true", true));
	//		testSet.push_back(TAR(L"false", false));
	//		testSet.push_back(TAR(L"1", true));
	//		testSet.push_back(TAR(L"0", false));
	//	}

	//	// Negation
	//	{
	//		testSet.push_back(TAR(L"!true", false));
	//		testSet.push_back(TAR(L"!false", true));
	//		testSet.push_back(TAR(L"!1", false));
	//		testSet.push_back(TAR(L"!0", true));
	//	}

	//	// Basic AND tests
	//	{
	//		testSet.push_back(TAR(L"1 && 1", true));
	//		testSet.push_back(TAR(L"0 && 1", false));
	//		testSet.push_back(TAR(L"1 && 0", false));
	//		testSet.push_back(TAR(L"0 && 0", false));
	//		testSet.push_back(TAR(L"1 AND 1", true));
	//		testSet.push_back(TAR(L"0 AND 1", false));
	//		testSet.push_back(TAR(L"1 AND 0", false));
	//		testSet.push_back(TAR(L"0 AND 0", false));

	//		testSet.push_back(TAR(L"!0 and 1", true));
	//		testSet.push_back(TAR(L"1 and !1", false));

	//	}

	//	// Basic OR tests
	//	{
	//		testSet.push_back(TAR(L"1 || 1", true));
	//		testSet.push_back(TAR(L"0 || 1", true));
	//		testSet.push_back(TAR(L"1 || 0", true));
	//		testSet.push_back(TAR(L"0 || 0", false));
	//		testSet.push_back(TAR(L"1 or 1", true));
	//		testSet.push_back(TAR(L"0 or 1", true));
	//		testSet.push_back(TAR(L"1 or 0", true));
	//		testSet.push_back(TAR(L"0 or 0", false));
	//	}

	//	// Basic XOR tests
	//	{
	//		testSet.push_back(TAR(L"1 xor 1", false));
	//		testSet.push_back(TAR(L"0 xor 1", true));
	//		testSet.push_back(TAR(L"1 XOR 0", true));
	//		testSet.push_back(TAR(L"0 XOR 0", false));
	//	}

	//	// Basic NAND tests
	//	{
	//		testSet.push_back(TAR(L"1 NAND 1", false));
	//		testSet.push_back(TAR(L"0 NAND 1", true));
	//		testSet.push_back(TAR(L"1 NAND 0", true));
	//		testSet.push_back(TAR(L"0 NAND 0", true));
	//	}

	//	// Parenthesis
	//	{
	//		//// Simple stuff
	//		//testSet.push_back(TAR(L"(1 and 1)", true));
	//		//testSet.push_back(TAR(L"((1 and 1))", true));
	//		//testSet.push_back(TAR(L"(((((1 and 1)))))", true));



	//		//testSet.push_back(TAR(L"(0 or 1) and (0 or 1)", true));
	//		//testSet.push_back(TAR(L"(0 and 1) or (0 and 1)", false));
	//		//testSet.push_back(TAR(L"0 and (1 and (1 or 0))", false));
	//		//testSet.push_back(TAR(L"((0 and 1) and 1) or 0", false));
	//		//testSet.push_back(TAR(L"((0 and 1) or 1) or 0", true));
	//	}

	//	// Operator precedence rule
	//	//{
	//	//	testSet.push_back(TAR(L"0 and 0 and 1", false));
	//	//	testSet.push_back(TAR(L"1 and 1 and 1", true));
	//	//	testSet.push_back(TAR(L"1 and 0 and 1", false));
	//	//	testSet.push_back(TAR(L"1 or 0 and 1", true));

	//	//	// TODO : moar


	//	//}

	//	//random words
	//	/*testSet.push_back(TAR(L"test", true));
	//	testSet.push_back(TAR(L"TRPGEngine", true));*/

	//	//// Numerical values
	//	//testSet.push_back(TAR(L"-1", true));
	//	//testSet.push_back(TAR(L"-42", true));
	//	//testSet.push_back(TAR(L"42", true));
	//	//testSet.push_back(TAR(L"1337", true));
	//	//testSet.push_back(TAR(L"000001", true));
	//	//testSet.push_back(TAR(L"100000", true));
	//	//testSet.push_back(TAR(L"000000", false));

	//	// Equality tests
	//	//testSet.push_back(TAR(L"0 == 0", true));
	//	//testSet.push_back(TAR(L"42 == 42", true));
	//	//testSet.push_back(TAR(L"0 == 42", false));
	//	//testSet.push_back(TAR(L"42 == 0", false));
	//	//testSet.push_back(TAR(L"test == test", true));
	//	//testSet.push_back(TAR(L"0 != 0", false));
	//	//testSet.push_back(TAR(L"42 != 42", false));
	//	//testSet.push_back(TAR(L"0 != 42", true));
	//	//testSet.push_back(TAR(L"42 != 0", true));
	//	//testSet.push_back(TAR(L"test != test", false));
	//	//testSet.push_back(TAR(L"éêà@ == éêà@", true));
	//	//

	//	// Numeric expressions
	//	{
	//		/*testSet.push_back(TAR(L"2 == 2", true));
	//		testSet.push_back(TAR(L"2 == 1 + 1", true));
	//		testSet.push_back(TAR(L"2 == (1 + 1)", true));
	//		testSet.push_back(TAR(L"3 == 1 + 1", true));
	//		testSet.push_back(TAR(L"1 - 1", false));
	//		testSet.push_back(TAR(L"4 == (2*2)", true));
	//		testSet.push_back(TAR(L"8 == 2*2 + 4", true));
	//		testSet.push_back(TAR(L"8 == (2*2) + 4", true));
	//		testSet.push_back(TAR(L"8 == 4 + 2*2", true));
	//		testSet.push_back(TAR(L"8 == (4 + 2*2)", true));
	//		testSet.push_back(TAR(L"2 < 1", false));
	//		testSet.push_back(TAR(L"2 > 1", true));
	//		testSet.push_back(TAR(L"2 < 2", false));
	//		testSet.push_back(TAR(L"2 > 2", false));*/
	//	}

	//	// String expressions
	//	{
	//		//TODO : do that
	//	}

	//	Tester<bool> t;
	//	float result = t.test(testSet, failedSet, failedResults, FunctorConditionTest());

	//	if (displayFailed)
	//	{
	//		if (!failedSet.empty())
	//		{
	//			for (int i = 0; i < failedSet.size(); ++i)
	//			{
	//				Logger::logs(L"\n----Test     : "
	//					+ failedSet[i].first + L"\n"
	//					+ L"----Expected : " + std::to_wstring(failedSet[i].second) + L"\n"
	//					+ L"----Got      : " + std::to_wstring(failedResults[i])
	//					, ERRORTAG_TEST_L
	//					, L"condition resolution tests");
	//			}
	//		}
	//	}

	//	Logger::logs("Condition resolution results : " + std::to_string(result) + " %"
	//		, ERRORTAG_TEST);
	//	return result;
	//}

	//// Conditional Text
	//float Test::testConditionnalText(bool displayFailed)
	//{
	//	typedef std::pair<ustring, ustring> TAR;
	//	typedef std::vector<TAR> TARList;
	//	typedef std::vector<ustring> RList;

	//	GameDictionary dictionary;
	//	dictionary.set(L"key", L"testValue");
	//	dictionary.set(L"key2", L"testValue2");
	//	dictionary.set(L"Digit", L"0");
	//	dictionary.set(L"Number", L"481516");
	//	dictionary.set(L"Pi", L"3.1415");

	//	Interpreter interpret(dictionary, std::vector<bool>());

	//	TARList testSet;
	//	TARList failedSet;
	//	RList failedResults;

	//	// Basic values
	//	{
	//		testSet.push_back(TAR(L"<1>[test]", L"test"));
	//		testSet.push_back(TAR(L"<0>[test]", L""));
	//		testSet.push_back(TAR(L"<true>[test]", L"test"));
	//		testSet.push_back(TAR(L"<false>[test]", L""));
	//	}

	//	// Basic sentence test
	//	{
	//		testSet.push_back(TAR(L"This is <1>[maybe ]a test", L"This is maybe a test"));
	//		testSet.push_back(TAR(L"This is <1>[(maybe) ]a test", L"This is (maybe) a test"));
	//		testSet.push_back(TAR(L"(This is <1>[(maybe) ]a test)", L"(This is (maybe) a test)"));
	//	}

	//	// Multiple conditional texts
	//	{
	//		testSet.push_back(TAR(L"<0>[1]<1>[2]<0>[3]<1>[4]<1>[5]", L"245"));
	//	}

	//	Tester<ustring> t;
	//	float result = t.test(testSet
	//		, failedSet
	//		, failedResults
	//		, FunctorVariableAndConditionalTextTest()
	//		, (void*) &interpret);

	//	if (displayFailed)
	//	{
	//		if (!failedSet.empty())
	//		{
	//			for (int i = 0; i < failedSet.size(); ++i)
	//			{
	//				Logger::logs(L"\n----Test     : "
	//					+ failedSet[i].first + L"\n"
	//					+ L"----Expected : " + failedSet[i].second + L"\n"
	//					+ L"----Got      : " + failedResults[i]
	//					, ERRORTAG_TEST_L
	//					, L"Conditional text tests");
	//			}
	//		}
	//	}

	//	Logger::logs("Conditional text results : " + std::to_string(result) + " %"
	//		, ERRORTAG_TEST);
	//	return result;
	//}

	//float Test::testNameIds(bool displayFailed)
	//{
	//	typedef std::pair<ustring, bool> TAR;
	//	typedef std::vector<TAR> TARList;
	//	typedef std::vector<bool> RList;

	//	TARList testSet;
	//	TARList failedSet;
	//	RList failedResults;
	//	testSet.push_back(TAR(L"", false));
	//	testSet.push_back(TAR(L"    ", false));
	//	testSet.push_back(TAR(L"test", true));
	//	testSet.push_back(TAR(L"   test", false));
	//	testSet.push_back(TAR(L"test   ", false));
	//	testSet.push_back(TAR(L"TesT", false));
	//	testSet.push_back(TAR(L" OUIIII", false));
	//	testSet.push_back(TAR(L"cApSbEDDd    ", false));
	//	testSet.push_back(TAR(L"white space", false));
	//	testSet.push_back(TAR(L"_test", false));
	//	testSet.push_back(TAR(L"_test_", false));
	//	testSet.push_back(TAR(L"é", false));
	//	testSet.push_back(TAR(L"è", false));
	//	testSet.push_back(TAR(L"â", false));
	//	testSet.push_back(TAR(L"Â", false));
	//	testSet.push_back(TAR(L"|", false));
	//	testSet.push_back(TAR(L"^", false));
	//	testSet.push_back(TAR(L"ë", false));
	//	testSet.push_back(TAR(L"camelCaseTest", false));
	//	testSet.push_back(TAR(L"snake_case_test", true));
	//	testSet.push_back(TAR(L"0", false));
	//	testSet.push_back(TAR(L"0test", false));
	//	testSet.push_back(TAR(L"test0", true));
	//	testSet.push_back(TAR(L"t0123456789", true));

	//	Tester<bool> t;
	//	float result = t.test(testSet, failedSet, failedResults, FunctorNameIdTest());

	//	if (displayFailed)
	//	{
	//		if (!failedSet.empty())
	//		{
	//			for (int i = 0; i < failedSet.size(); ++i)
	//			{
	//				Logger::logs(L"\n----Test     : "
	//					+ failedSet[i].first + L"\n"
	//					+ L"----Expected : " + std::to_wstring(failedSet[i].second) + L"\n"
	//					+ L"----Got      : " + std::to_wstring(failedResults[i])
	//					, ERRORTAG_TEST_L
	//					, L"name id tests");
	//			}
	//		}
	//	}

	//	Logger::logs("Name id results : " + std::to_string(result) + " %"
	//		, ERRORTAG_TEST);
	//	return result;
	//}

	//float Test::testVariableResolution(bool displayFailed)
	//{
	//	typedef std::pair<ustring, ustring> TAR;
	//	typedef std::vector<TAR> TARList;
	//	typedef std::vector<ustring> RList;

	//	GameDictionary dictionary;
	//	dictionary.set(L"key", L"testValue");
	//	dictionary.set(L"key2", L"testValue2");
	//	dictionary.set(L"Digit", L"0");
	//	dictionary.set(L"Number", L"481516");
	//	dictionary.set(L"Pi", L"3.1415");

	//	Interpreter interpret(dictionary, std::vector<bool>());

	//	TARList testSet;
	//	TARList failedSet;
	//	RList failedResults;
	//	testSet.push_back(TAR(L"", L""));
	//	testSet.push_back(TAR(L"$", L"$"));
	//	testSet.push_back(TAR(L"test$", L"test$"));
	//	testSet.push_back(TAR(L"$key", L"testValue"));
	//	testSet.push_back(TAR(L"$Key", L"testValue"));
	//	testSet.push_back(TAR(L"$KEY", L"testValue"));
	//	testSet.push_back(TAR(L"$(key)", L"testValue"));

	//	testSet.push_back(TAR(L"$key $key2", L"testValue testValue2"));
	//	testSet.push_back(TAR(L"$key        $key2", L"testValue        testValue2"));
	//	testSet.push_back(TAR(L"$key2 $key", L"testValue2 testValue"));
	//	testSet.push_back(TAR(L"$key,$key2", L"testValue,testValue2"));

	//	testSet.push_back(TAR(L"random$key", L"randomtestValue"));
	//	testSet.push_back(TAR(L"$keyrandom", L""));	//keyrandom doesn't exist in the dictionary
	//	testSet.push_back(TAR(L"random$keyrandom", L"random"));
	//	testSet.push_back(TAR(L"random$(key)random", L"randomtestValuerandom"));

	//	testSet.push_back(TAR(L"$key, random", L"testValue, random"));
	//	testSet.push_back(TAR(L"$key| random", L"testValue| random"));
	//	testSet.push_back(TAR(L"$key: random", L"testValue: random"));
	//	testSet.push_back(TAR(L"$key! random", L"testValue! random"));
	//	testSet.push_back(TAR(L"$key. random", L"testValue. random"));
	//	testSet.push_back(TAR(L"$key^test", L"testValue^test"));
	//	testSet.push_back(TAR(L"$keyétest", L"testValueétest"));

	//	testSet.push_back(TAR(L"$,key", L"$,key"));
	//	testSet.push_back(TAR(L"$(,key", L"$(,key"));
	//	testSet.push_back(TAR(L"$(ke)y", L"y"));	//$ke doesn't exist
	//	testSet.push_back(TAR(L"$,(key", L"$,(key"));
	//	testSet.push_back(TAR(L"$((key)", L"$((key)"));
	//	testSet.push_back(TAR(L"$(key))", L"testValue)"));

	//	// TODO : solve the escaping problem
	//	/*testSet.push_back(TAR(L"\\$key", L"$key"));
	//	testSet.push_back(TAR(L"test\\$key", L"test$key"));
	//	testSet.push_back(TAR(L"$\\key", L"$\\key"));*/

	//	Tester<ustring> testVariables;
	//	float result = testVariables.test(testSet
	//		, failedSet
	//		, failedResults
	//		, FunctorVariableAndConditionalTextTest()
	//		, (void*) &interpret);

	//	if (displayFailed)
	//	{
	//		if (!failedSet.empty())
	//		{
	//			Logger::logNewLine();
	//			for (int i = 0; i < failedSet.size(); ++i)
	//			{
	//				Logger::logs(
	//					L"\n----Test     : "
	//					+ failedSet[i].first + L"\n"
	//					+ L"----Expected : " + failedSet[i].second + L"\n"
	//					+ L"----Got      : " + failedResults[i]
	//					, ERRORTAG_TEST_L
	//					, L"variable resolution tests");
	//			}
	//		}
	//	}

	//	Logger::logs("Variable resolution results : " + std::to_string(result) + " %"
	//		, ERRORTAG_TEST);
	//	return result;
	//}

	//float Test::testVariableSpecifierResolution(bool displayFailed)
	//{
	//	typedef std::pair<ustring, ustring> TAR;
	//	typedef std::vector<TAR> TARList;
	//	typedef std::vector<ustring> RList;

	//	GameDictionary dictionary;
	//	dictionary.set(L"key", L"testValue");
	//	dictionary.set(L"key2", L"testValue2");
	//	dictionary.set(L"Digit", L"0");
	//	dictionary.set(L"Number", L"481516");
	//	dictionary.set(L"Pi", L"3.1415");

	//	Interpreter interpret(dictionary, std::vector<bool>());

	//	TARList testSet;
	//	TARList failedSet;
	//	RList failedResults;

	//	// Basic specifier
	//	testSet.push_back(TAR(L"${toUpper}key", L"TESTVALUE"));
	//	testSet.push_back(TAR(L"${toLower}key", L"testvalue"));
	//	testSet.push_back(TAR(L"${toUpper}number", L"481516"));
	//	testSet.push_back(TAR(L"${toLower}number", L"481516"));

	//	// Several in the same sentence
	//	testSet.push_back(TAR(L"${toLower}key thisisatest ${toUpper}key2", L"testvalue TESTVALUE2"));

	//	// Several specifiers per variable
	//	testSet.push_back(TAR(L"${nbToLiteral}{toUpper}number", L"481516"));

	//	Tester<ustring> testVariables;
	//	float result = testVariables.test(testSet
	//		, failedSet
	//		, failedResults
	//		, FunctorVariableAndConditionalTextTest()
	//		, (void*)&interpret);

	//	if (displayFailed)
	//	{
	//		if (!failedSet.empty())
	//		{
	//			Logger::logNewLine();
	//			for (int i = 0; i < failedSet.size(); ++i)
	//			{
	//				Logger::logs(
	//					L"\n----Test     : "
	//					+ failedSet[i].first + L"\n"
	//					+ L"----Expected : " + failedSet[i].second + L"\n"
	//					+ L"----Got      : " + failedResults[i]
	//					, ERRORTAG_TEST_L
	//					, L"variable resolution tests");
	//			}
	//		}
	//	}

	//	Logger::logs("Variable resolution results : " + std::to_string(result) + " %"
	//		, ERRORTAG_TEST);
	//	return result;
	//}

	//float Test::testEvents(bool displayFailed /*= false*/)
	//{
	//	// TODO (implement that)
	//	return 0.f;
	//}

	//float Test::testSceneParsing(bool displayFailed /*= false*/)
	//{
	//	// TODO (implement that)
	//	return 0.f;
	//}

}


