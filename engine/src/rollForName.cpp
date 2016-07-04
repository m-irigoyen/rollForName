#include <rollForName/rollForName.h>
#include <rollForName\Logger.h>
#include <rollForName\Paths.h>
#include <rollForName\Parser.h>
#include <rollForName\StringHelpers.h>

#include <iostream>
#include <locale>
#include <codecvt>

#define ERRORTAG_ROLLFORNAME_L L"rollForName"
#define ERRORTAG_ROLLFORNAME "rollForName"

namespace rfn
{
	RollForName::RollForName()
	{
	}

	bool RollForName::loadTablesFromFile(std::string fileName, bool completePath)
	{
		using namespace std;
		if (!completePath)
		{
			fileName = makePathTable(fileName);
		}

		wifstream stream;
		if (openFile(fileName, stream))
		{
			// Reading the entire file in memory
			ustring completeFile;
			ustring line;
			while (stream.good() && !stream.eof())
			{
				std::getline(stream, line);
				// TODO : remove commentaries in the line
				completeFile += line + L"\n";
			}

			// now, parsing tables
			bool keepGoing = true;
			ustring::iterator it = completeFile.begin();
			while (keepGoing)
			{
				Table t;
				if (Parser::parseTable(it
					, completeFile.end()
					, t))
				{
					if (!t.isValid())
					{
						Logger::errlogs(L"Parsed invalid table");
						return false;
					}
					else if (existsTable(t.name))
					{
						Logger::errlogs(L"A table with identical name already exists");
						return false;
					}
					else
					{
						// Adding the table
						tables_.insert(make_pair(t.name, t));
					}
				}
				else
				{
					ustring skipped;
					if (Parser::findNextBracketEnd(it, completeFile.end(), skipped))
					{
						Logger::errlogs(L"Couldn't parse the following table :\n"
							+ skipped
							, ERRORTAG_ROLLFORNAME_L
							, L"loadTableFromFile");
					}
					else
					{
						keepGoing = false;
					}
				}
			}

			return true;
		}

		return false;
	}

	bool RollForName::loadGeneratorsFromFile(std::string fileName, bool completePath)
	{
		using namespace std;
		if (!completePath)
		{
			fileName = makePathGenerator(fileName);
		}

		wifstream stream;
		if (openFile(fileName, stream))
		{
			// Reading the entire file in memory
			ustring completeFile;
			ustring line;
			while (stream.good() && !stream.eof())
			{
				std::getline(stream, line);
				// TODO : remove commentaries in the line
				completeFile += line + L"\n";
			}

			// now, parsing tables
			bool keepGoing = true;
			ustring::iterator it = completeFile.begin();
			while (keepGoing)
			{
				Generator g;
				if (Parser::parseGenerator(it
					, completeFile.end()
					, g))
				{
					if (!g.isValid())
					{
						Logger::errlogs(L"Parsed invalid generator");
						return false;
					}
					else if (existsGenerator(g.name))
					{
						Logger::errlogs(L"A generator with identical name already exists");
						return false;
					}
					else
					{
						// Adding the table
						generators_.insert(make_pair(g.name, g));
					}
				}
				else
				{
					ustring skipped;
					if (Parser::findNextBracketEnd(it, completeFile.end(), skipped))
					{
						Logger::errlogs(L"Couldn't parse the following generator :\n"
							+ skipped
							, ERRORTAG_ROLLFORNAME_L
							, L"loadTableFromFile");
					}
					else
					{
						keepGoing = false;
					}
				}
			}

			return true;
		}

		return false;
	}

	bool RollForName::loadFromFile(std::string fileName)
	{
		if (loadGeneratorsFromFile(fileName))
		{
			Logger::logs("Loaded generator " + fileName);
			return true;
		}
		else if (loadTablesFromFile(fileName))
		{
			Logger::logs("Loaded table " + fileName);
			return true;
		}
		else
		{
			Logger::errlogs("Couldn't find a generator or table with name " + fileName
				, ERRORTAG_ROLLFORNAME
				, "loadFromFile");
			return false;
		}
	}

	bool RollForName::generate(ustring generatorName, ustring& result, bool isTopLevel)
	{
		Generator g;
		if (existsGenerator(generatorName, g))
		{
			for (Instruction i : g.instructions)
			{
				executeInstruction(i, result);
			}
		}
		return false;
	}

	bool RollForName::drawFromTable(ustring tableName, ustring & result)
	{
		Table t;
		if (existsTable(tableName, t))
		{
			TableEntry te = t.getRandomEntry();

			// Goto
			ustring gotoName;
			ustring gotoResult;
			if (Parser::parseGoto(te.description, gotoName)
				&& drawFromTable(gotoName, gotoResult))
			{
				result += te.name + L" : " + gotoName + L" : " + gotoResult;
				return true;
			}
			else
			{
				result += te.name + L" : " + te.description;
				return true;
			}
		}

		return false;
	}

	bool RollForName::openFile(std::string fileName, std::wifstream& stream)
	{
		using namespace std;
		// Open given file
		stream.open(fileName);
		stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>()));

		if (stream.is_open())
		{
			Logger::logs("Opened file " + fileName
				, ERRORTAG_ROLLFORNAME, "open");
		}
		else
		{
			Logger::errlogs("Couldn't open file " + fileName
				, ERRORTAG_ROLLFORNAME, "open");
		}
		return stream.is_open();
	}

		//--------
		// HELPERS
		//--------
	bool RollForName::existsTable(ustring tableName)
	{
		makeValidIdInPlace(tableName);
		return (tables_.find(tableName) != tables_.end());
	}
	bool RollForName::existsTable(ustring tableName, Table & table)
	{
		makeValidIdInPlace(tableName);
		TableMap::iterator it = tables_.find(tableName);
		if (it != tables_.end())
		{
			table = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}
	bool RollForName::existsGenerator(ustring generatorName)
	{
		makeValidIdInPlace(generatorName);
		return (generators_.find(generatorName) != generators_.end());
	}
	bool RollForName::existsGenerator(ustring generatorName, Generator & generator)
	{
		makeValidIdInPlace(generatorName);
		GeneratorMap::iterator it = generators_.find(generatorName);
		if (it != generators_.end())
		{
			generator = it->second;
			return true;
		}
		else
		{
			return false;
		}
	}
	bool RollForName::executeInstruction(Instruction i, ustring & result)
	{
		Generator g;
		Table t;
		if (i.generator)
		{
			return generate(i.name, result);
		}
		else
		{
			return drawFromTable(i.name, result);
		}
	}
}