#include "Test.h"

#include <rollForName/Logger.h>
#include <rollForName/Random.h>
#include <rollForName/GameDictionary.h>
#include <rollForName/Parser.h>
#include <rollForName/Generator.h>
#include <rollForName/CommandLineHelper.h>

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
					if (rollDice(test, t))
					{
						if (t > result.max)
							result.max = t;
						if (t < result.min)
							result.min = t;
					}
					else
					{
						Logger::errlogs(L"Couldn't parse given roll :\n" + test);
						return false;
					}
				}

				return expected == result;
			}
	};


	struct FunctorNameFromFilepathTest
	{
		bool operator()(ustring test, ustring expected, void*, ustring& result)
		{
			if (!Parser::parseNameFromFilepath(test, result))
			{
				Logger::errlogs(L"Couldn't parse given filepath :\n" + test);
				return false;
			}

			return expected.compare(result) == 0;
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
				Logger::errlogs(L"Couldn't parse given test :\n" + test);
				return false;
			}
		}
	};

	// Test table entry parsing
	struct FunctorTableEntryTest
	{
		bool operator()(ustring test, TableEntry expected, void*, TableEntry& result)
		{
			if (Parser::parseTableEntry(test, result))
			{
				return expected == result;
			}
			else
			{
				Logger::errlogs(L"Couldn't parse given test :\n" + test);
				return false;
			}
		}
	};

	// Test table parsing
	struct FunctorTableTest
	{
		bool operator()(ustring test, Table expected, void*, Table& result)
		{
			if (Parser::parseTable(test, result))
			{
				return expected == result;
			}
			else
			{
				Logger::errlogs(L"Couldn't parse given test :\n" + test);
				return false;
			}
		}
	};

	// Test Instruction parsing
	struct FunctorInstructionTest
	{
		bool operator()(ustring test, Instruction expected, void*, Instruction& result)
		{
			if (Parser::parseGeneratorInstruction(test, result))
			{
				return (expected.generator == result.generator)
					&& (expected.name.compare(result.name) == 0);
			}
			else
			{
				Logger::errlogs(L"Couldn't parse given test :\n" + test);
				return false;
			}
		}
	};

	// Test generator parsing
	struct FunctorGeneratorTest
	{
		bool operator()(ustring test, Generator expected, void*, Generator& result)
		{
			if (Parser::parseGenerator(test, result))
			{
				return expected == result;
			}
			else
			{
				Logger::errlogs(L"Couldn't parse given Generator :\n" + test);
				return false;
			}
		}
	};


	//// ALL TESTS
	float Test::testAll(bool displayFailed)
	{
		Logger::logNewLine();

		float global = 0.0f;

		std::vector<float> results;
		results.push_back(testRange(displayFailed));
		results.push_back(testTableEntryParse(displayFailed));
		results.push_back(testTableParse(displayFailed));
		results.push_back(testInstructionParse(displayFailed));
		results.push_back(testGeneratorParse(displayFailed));
		results.push_back(testExtractNameFromPath(displayFailed));
		results.push_back(testRoll(displayFailed));

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

	float Test::testExtractNameFromPath(bool displayFailed)
	{
		typedef std::pair<ustring, ustring> TAR;
		typedef std::vector<TAR> TARList;
		typedef std::vector<ustring> RList;

		TARList testSet;
		TARList failedSet;
		RList failedResults;

		// Simple
		{
			testSet.push_back(TAR(L"test.rfn", L"test"));
			testSet.push_back(TAR(L"weird.rfn", L"weird"));
		}

		// Complete paths
		{
			testSet.push_back(TAR(L"C:/WOUALLER/hihi/test.rfn", L"test"));
			testSet.push_back(TAR(L"C:/ceci/est/un/chemin/ridiculement/long.rfn", L"long"));
			testSet.push_back(TAR(L"C:\\WOUALLER\\hihi\\test.rfn", L"test"));
			testSet.push_back(TAR(L"C:\\ceci\\est\\un\\chemin\\ridiculement\\long.rfn", L"long"));
		}

		Tester<ustring> t;
		float result = t.test(testSet, failedSet, failedResults, FunctorNameFromFilepathTest());

		if (displayFailed)
		{
			if (!failedSet.empty())
			{
				for (int i = 0; i < failedSet.size(); ++i)
				{
					Logger::logs(L"\n----Test     : "
						+ failedSet[i].first + L"\n"
						+ L"----Expected : " + (failedSet[i].second) + L"\n"
						+ L"----Got      : " + failedResults[i]
						, ERRORTAG_TEST_L
						, L"testNameFromPath");
				}
			}
		}

		Logger::logs("Test name from path results : " + std::to_string(result) + " %"
			, ERRORTAG_TEST);
		return result;
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
						, L"testRoll");
				}
			}
		}

		Logger::logs("Test roll results : " + std::to_string(result) + " %"
			, ERRORTAG_TEST);
		return result;
	}

	float Test::testRange(bool displayFailed)
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

		// Basic ranges
		{
			testSet.push_back(TAR(L"<1_6>", Range(1, 6)));
			testSet.push_back(TAR(L"<2_12>", Range(2, 12)));
			testSet.push_back(TAR(L"<1_12>", Range(1, 12)));
			testSet.push_back(TAR(L"<12_24>", Range(12, 24)));
			testSet.push_back(TAR(L"<1_20>", Range(1, 20)));
		}

		// Negative numbers
		{
			testSet.push_back(TAR(L"<-4_-2>", Range(-4, -2)));
			testSet.push_back(TAR(L"<-10_-1>", Range(-10, -1)));
			testSet.push_back(TAR(L"<-22_-21>", Range(-22, -21)));
		}

		// Mixing it up
		{
			testSet.push_back(TAR(L"<-20_20>", Range(-20, 20)));
			testSet.push_back(TAR(L"<-17_13>", Range(-17, 13)));
		}

		// Reordering
		{
			testSet.push_back(TAR(L"<1_-1>", Range(-1, 1)));
			testSet.push_back(TAR(L"<5_-42>", Range(-42, 5)));
		}

		// Only one number
		{
			testSet.push_back(TAR(L"<-1>", Range(-1, -1)));
			testSet.push_back(TAR(L"<42>", Range(42, 42)));
			testSet.push_back(TAR(L"< 7 >", Range(7, 7)));
		}

		Tester<Range> t;
		float result = t.test(testSet, failedSet, failedResults, FunctorRangeTest());

		if (displayFailed)
		{
			if (!failedSet.empty())
			{
				for (int i = 0; i < failedSet.size(); ++i)
				{
					Logger::logs(L"\n----Test     : "
						+ failedSet[i].first + L"\n"
						+ L"----Expected : " + (failedSet[i].second).toUstring() + L"\n"
						+ L"----Got      : " + failedResults[i].toUstring()
						, ERRORTAG_TEST_L
						, L"testRange");
				}
			}
		}

		Logger::logs("Test range : " + std::to_string(result) + " %"
			, ERRORTAG_TEST);
		return result;
	}

	float Test::testTableEntryParse(bool displayFailed)
	{
		typedef std::pair<ustring, TableEntry> TAR;
		typedef std::vector<TAR> TARList;
		typedef std::vector<TableEntry> RList;

		TARList testSet;
		TARList failedSet;
		RList failedResults;

		GameDictionary dictionary;
		dictionary.set(L"key", L"testValue");
		dictionary.set(L"key2", L"testValue2");
		dictionary.set(L"Digit", L"0");
		dictionary.set(L"Number", L"481516");
		dictionary.set(L"Pi", L"3.1415");

		// Simple entry
		{
			TableEntry t(Range(1, 3), L"entry1", L"This is the first entry");
			ustring line =
				L"* <1_3> \"entry1\" : \"This is the first entry\"\n";
			testSet.push_back(TAR(line, t));
		}

		// Goto
		{
			TableEntry t(Range(4, 6), L"goto", L"->anotherTable");
			ustring line =
				L"* <4_6> \"goto\" : \"->anotherTable\"\n";
			testSet.push_back(TAR(line, t));
		}

		// Roll
		{
			TableEntry t(Range(7, 9), L"roll", L"You are [1d60] years old");
			ustring line =
				L"* <7_9> \"roll\" : \"You are [1d60] years old\"\n";
			testSet.push_back(TAR(line, t));
		}

		// Var
		{
			TableEntry t(Range(10, 13), L"variable", L"This is a $key, yeah!");
			ustring line =
				L"* <10_13> \"variable\" : \"This is a $key, yeah!\"\n";
			testSet.push_back(TAR(line, t));
		}

		// Combo
		{
			TableEntry t(Range(14, 17), L"combo", L"This is a $key, yeah!");
			ustring line =
				L"* <14_17> \"combo\" : \"This is a $key, yeah!\"\n";
			testSet.push_back(TAR(line, t));
		}

		// Simple entry without description
		{
			TableEntry t(Range(1, 3), L"entry1", L"");
			ustring line =
				L"* <1_3> \"entry1\"";
			testSet.push_back(TAR(line, t));
		}

		Tester<TableEntry> t;
		float result = t.test(testSet, failedSet, failedResults, FunctorTableEntryTest());

		if (displayFailed)
		{
			if (!failedSet.empty())
			{
				for (int i = 0; i < failedSet.size(); ++i)
				{
					Logger::logs(L"\n----Test Failed : \n"
						+ failedSet[i].first + L"\n"
						, ERRORTAG_TEST_L
						, L"testTableEntryParse");
				}
			}
		}

		Logger::logs("Test table entry parse : " + std::to_string(result) + " %"
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
			t.name = L"FirstTest";
			t.roll = L"[1d6]";
			t.entries.push_back(TableEntry(Range(1, 3), L"entry1", L"This is the first entry"));
			t.entries.push_back(TableEntry(Range(4, 6), L"entry2", L"This is the second entry"));

			ustring line =
				L"\"FirstTest\"\n	[1d6]\n	{\n	* <1_3> \"entry1\" : \"This is the first entry\"\n		* <4_6> \"entry2\" : \"This is the second entry\"\n }\n";
			testSet.push_back(TAR(line, t));
		}

		// Same with stuff before the name
		{
			Table t;
			t.name = L"FirstTest";
			t.roll = L"[1d6]";
			t.entries.push_back(TableEntry(Range(1, 3), L"entry1", L"This is the first entry"));
			t.entries.push_back(TableEntry(Range(4, 6), L"entry2", L"This is the second entry"));

			ustring line =
				L"421337 lorem ipsum - \"FirstTest\"\n	[1d6]\n	{\n	* <1_3> \"entry1\" : \"This is the first entry\"\n		* <4_6> \"entry2\" : \"This is the second entry\"\n }\n";
			testSet.push_back(TAR(line, t));
		}

		// More complex table
		{
			Table t;
			t.name = L"Another Test";
			t.roll = L"[2d4+1]";
			t.requiredTables.push_back(L"races");
			t.entries.push_back(TableEntry(Range(2, 3), L"entry1", L"This is a $key"));
			t.entries.push_back(TableEntry(Range(4, 6), L"entry2", L"This contains a roll [1d20]"));
			t.entries.push_back(TableEntry(Range(7, 9), L"a simple goto", L"-> races"));

			ustring line =
				L"\"Another Test\"\n	REQUIRED \"races\"\n	[2d4+1]\n	{\n	* <2_3> \"entry1\" : \"This is a $key\"\n		* <4_6> \"entry2\" : \"This contains a roll [1d20]\"\n	* <7_9> \"a simple goto\" : \"-> races\"\n }\n";
			testSet.push_back(TAR(line, t));
		}

		// No roll / range
		{
			Table t;
			t.name = L"No Roll test";
			t.requiredTables.push_back(L"races");
			t.entries.push_back(TableEntry(Range(), L"entry1", L"This is a $key"));
			t.entries.push_back(TableEntry(Range(), L"entry2", L"This contains a roll [1d20]"));
			t.entries.push_back(TableEntry(Range(), L"a simple goto", L"-> races"));

			ustring line =
				L"\"No Roll test\"\n	REQUIRED \"races\"\n	{\n	* \"entry1\" : \"This is a $key\"\n		* \"entry2\" : \"This contains a roll [1d20]\"\n	* \"a simple goto\" : \"-> races\"\n }\n";
			testSet.push_back(TAR(line, t));
		}

		Tester<Table> t;
		float result = t.test(testSet, failedSet, failedResults, FunctorTableTest());

		if (displayFailed)
		{
			if (!failedSet.empty())
			{
				for (int i = 0; i < failedSet.size(); ++i)
				{
					Logger::logs(L"\n----Test Failed : \n"
						+ failedSet[i].first + L"\n"
						, ERRORTAG_TEST_L
						, L"testTableParse");
				}
			}
		}

		Logger::logs("testTableParse results : " + std::to_string(result) + " %"
			, ERRORTAG_TEST);
		return result;
	}

	float Test::testInstructionParse(bool displayFailed)
	{
		typedef std::pair<ustring, Instruction> TAR;
		typedef std::vector<TAR> TARList;
		typedef std::vector<Instruction> RList;

		TARList testSet;
		TARList failedSet;
		RList failedResults;

		GameDictionary dictionary;
		dictionary.set(L"key", L"testValue");
		dictionary.set(L"key2", L"testValue2");
		dictionary.set(L"Digit", L"0");
		dictionary.set(L"Number", L"481516");
		dictionary.set(L"Pi", L"3.1415");

		// Simple instruction
		{
			Instruction i;
			i.generator = true;
			i.name = L"Test";
			ustring line = L"g: \"Test\"";
			testSet.push_back(TAR(line, i));
		}

		// Simple instruction, table
		{
			Instruction i;
			i.generator = false;
			i.name = L"Test";
			ustring line = L"t: \"Test\"";
			testSet.push_back(TAR(line, i));
		}

		// Simple instruction with space name
		{
			Instruction i;
			i.generator = true;
			i.name = L"Test name";
			ustring line = L"g: \"Test name\"";
			testSet.push_back(TAR(line, i));
		}

		// Default to generator
		{
			Instruction i;
			i.generator = true;
			i.name = L"Test name";
			ustring line = L"\"Test name\"";
			testSet.push_back(TAR(line, i));
		}


		Tester<Instruction> t;
		float result = t.test(testSet, failedSet, failedResults, FunctorInstructionTest());

		if (displayFailed)
		{
			if (!failedSet.empty())
			{
				for (int i = 0; i < failedSet.size(); ++i)
				{
					Logger::logs(L"\n----Test Failed : \n"
						+ failedSet[i].first + L"\n"
						, ERRORTAG_TEST_L
						, L"testInstructionParse");
				}
			}
		}

		Logger::logs("testInstructionParse results : " + std::to_string(result) + " %"
			, ERRORTAG_TEST);
		return result;
	}

	float Test::testGeneratorParse(bool displayFailed)
	{
		typedef std::pair<ustring, Generator> TAR;
		typedef std::vector<TAR> TARList;
		typedef std::vector<Generator> RList;

		TARList testSet;
		TARList failedSet;
		RList failedResults;

		GameDictionary dictionary;
		dictionary.set(L"key", L"testValue");
		dictionary.set(L"key2", L"testValue2");
		dictionary.set(L"Digit", L"0");
		dictionary.set(L"Number", L"481516");
		dictionary.set(L"Pi", L"3.1415");

		// Base generator
		{
			Generator g;
			g.name = L"Gen Test";
			g.instructions.push_back(Instruction(false, L"table"));
			g.instructions.push_back(Instruction(true, L"gen"));
			g.instructions.push_back(Instruction(true, L"default"));
			ustring line =
				L"\"Gen Test\"\n	{ t: \"table\"\n	g: \"gen\"\n \"default\"\n }";
			testSet.push_back(TAR(line, g));
		}

		// required gen
		{
			Generator g;
			g.name = L"other gen test";
			g.requiredGenerators.push_back(L"another gen");
			g.instructions.push_back(Instruction(false, L"table"));
			g.instructions.push_back(Instruction(true, L"gen"));
			g.instructions.push_back(Instruction(true, L"default"));
			ustring line =
				L"\"other gen test\"\n	required generators : \"another gen\"\n	{ t: \"table\"\n	g: \"gen\"\n \"default\"\n }";
			testSet.push_back(TAR(line, g));
		}

		// required tables and gen
		{
			Generator g;
			g.name = L"other gen test";
			g.requiredGenerators.push_back(L"another gen");
			g.requiredTables.push_back(L"a table");
			g.instructions.push_back(Instruction(false, L"table"));
			g.instructions.push_back(Instruction(true, L"gen"));
			g.instructions.push_back(Instruction(true, L"default"));
			ustring line =
				L"\"other gen test\"\n	required generators : \"another gen\"\n	required tables : \"a table\"\n {	t: \"table\"\n	g: \"gen\"\n \"default\"\n }";
			testSet.push_back(TAR(line, g));
		}

		// required tables, but no gen
		{
			Generator g;
			g.name = L"other gen test";
			g.requiredTables.push_back(L"a table");
			g.instructions.push_back(Instruction(false, L"table"));
			g.instructions.push_back(Instruction(true, L"gen"));
			g.instructions.push_back(Instruction(true, L"default"));
			ustring line =
				L"\"other gen test\"\n	required tables : \"a table\"\n	{ t: \"table\"\n	g: \"gen\"\n \"default\"\n }";
			testSet.push_back(TAR(line, g));
		}

		Tester<Generator> t;
		float result = t.test(testSet, failedSet, failedResults, FunctorGeneratorTest());

		if (displayFailed)
		{
			if (!failedSet.empty())
			{
				for (int i = 0; i < failedSet.size(); ++i)
				{
					Logger::logs(L"\n----Test Failed : \n"
						+ failedSet[i].first + L"\n"
						, ERRORTAG_TEST_L
						, L"testGeneratorParse");
				}
			}
		}

		Logger::logs("testGeneratorParse results : " + std::to_string(result) + " %"
			, ERRORTAG_TEST);
		return result;
	}
}


