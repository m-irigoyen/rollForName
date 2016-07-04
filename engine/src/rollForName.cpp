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
		return false;
	}

	bool RollForName::loadFromFile(std::string fileName)
	{
		return false;
	}

	bool RollForName::generate(std::string generatorName)
	{
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


	bool RollForName::existsTable(ustring tableName)
	{
		makeValidIdInPlace(tableName);
		return (tables_.find(tableName) != tables_.end());
	}
	bool RollForName::existsGenerator(ustring tableName)
	{
		makeValidIdInPlace(tableName);
		return (generators_.find(tableName) != generators_.end());
	}
}