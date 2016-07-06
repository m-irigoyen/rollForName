#include <rollForName/rollForName.h>
#include <rollForName\Logger.h>
#include <rollForName\Paths.h>
#include <rollForName\Parser.h>
#include <rollForName\StringHelpers.h>

#include <iostream>
#include <locale>
#include <codecvt>

#include <boost/algorithm/string/replace.hpp>

#define ERRORTAG_ROLLFORNAME_L L"rollForName"
#define ERRORTAG_ROLLFORNAME "rollForName"

namespace rfn
{
	RollForName::RollForName()
	{
	}

	bool RollForName::loadTablesFromFile(std::string fileName, bool isCompletePath)
	{
		return loadTablesFromFile(stringToUstring(fileName), isCompletePath);
	}

	bool RollForName::loadGeneratorsFromFile(std::string fileName, bool isCompletePath)
	{
		return loadGeneratorsFromFile(stringToUstring(fileName), isCompletePath);
	}

	bool RollForName::loadTablesFromFile(ustring fileName, bool isCompletePath)
	{
		ustring completePath;
		if (!isCompletePath)
		{
			completePath = makePathTable(fileName);
		}
		else
		{
			completePath = fileName;
			Parser::parseNameFromFilepath(fileName, fileName);
		}

		// That file already was loaded.
		if (std::find(loadedTableFiles_.begin(), loadedTableFiles_.end(), fileName)
			!= loadedTableFiles_.end())
		{
			return true;
		}

		std::wifstream stream;
		if (openFile(completePath, stream))
		{
			loadedTableFiles_.push_back(fileName);

			return loadTablesFromStream(stream);
		}

		return false;
	}

	bool RollForName::loadGeneratorsFromFile(ustring fileName, bool isCompletePath)
	{
		ustring completePath;

		if (!isCompletePath)
		{
			completePath = makePathGenerator(fileName);
		}
		else
		{
			completePath = fileName;
			Parser::parseNameFromFilepath(fileName, fileName);
		}

		// That file already was loaded.
		if (std::find(loadedGeneratorFiles_.begin(), loadedGeneratorFiles_.end(), fileName)
			!= loadedGeneratorFiles_.end())
		{
			return true;
		}

		std::wifstream stream;
		if (openFile(completePath, stream))
		{
			loadedGeneratorFiles_.push_back(fileName);

			return loadGeneratorsFromStream(stream);
		}

		return false;
	}

	bool RollForName::loadFromFile(std::string fileName)
	{
		return loadFromFile(stringToUstring(fileName));
	}

	bool RollForName::loadFromFile(ustring fileName)
	{
		if (loadGeneratorsFromFile(fileName))
		{
			Logger::logs(L"Loaded generator " + fileName);
			return true;
		}
		else if (loadTablesFromFile(fileName))
		{
			Logger::logs(L"Loaded table " + fileName);
			return true;
		}
		else
		{
			Logger::errlogs(L"Couldn't find a generator or table with name " + fileName
				, ERRORTAG_ROLLFORNAME_L
				, L"loadFromFile");
			return false;
		}
	}

	bool RollForName::generate(ustring generatorName, ustringVector& result, int level, bool isTopLevel)
	{
		Generator g;
		if (existsGenerator(generatorName, g))
		{
			ustring tab = tabFromLevel(level);
			result.push_back(tab + g.name);

			for (Instruction i : g.instructions)
			{
				executeInstruction(i, level+1, result);
			}
		}
		return false;
	}

	bool RollForName::generate(std::string generatorName, ustringVector & result, int level, bool isTopLevel)
	{
		return generate(stringToUstring(generatorName)
			, result
			, level
			, isTopLevel);
	}

	bool RollForName::drawFromTable(ustring tableName, ustringVector & result, int level)
	{
		Table t;
		if (existsTable(tableName, t))
		{
			ustring tab = tabFromLevel(level);
			TableEntry te;
			// Rolling and drawing an entry
			if (t.roll.empty())
			{
				te = t.getRandomEntry();
			}
			else
			{
				int diceResult;
				if (rollDice(t.roll, diceResult))
				{
					te = t.getEntryAt(diceResult);
				}
				else
				{
					te = t.getRandomEntry();
				}
			}

			// Goto
			ustring gotoName;
			ustring parseResult;
			Action a;
			if (Parser::parseGoto(te.description, gotoName))
			{
				result.push_back(tab + te.name + L" : ");
				drawFromTable(gotoName, result, level + 1);
				return true;
			}
			else if (Parser::parseAction(te.description, a))
			{
				dictionary_.set(a.variableName, a.value);
			}
			else
			{
				// Resolve variables
				ustring variable;
				ustring::iterator it = te.description.begin();
				while (Parser::parseLoneVariable(it
					, te.description.end()
					, variable))
				{
					if (dictionary_.exists(variable))
					{
						ustring fullVariable = L"($" + variable + L")";
						boost::replace_all(te.description
							, fullVariable
							, std::to_wstring(dictionary_.get(fullVariable)));
						it = te.description.begin();
					}
				}

				result.push_back(tab + te.name + L" : " + te.description);
				return true;
			}
		}

		return false;
	}

	bool RollForName::openFile(ustring fileName, std::wifstream & stream)
	{
		// Open given file
		stream.open(fileName);
		stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>()));

		if (stream.is_open())
		{
			Logger::logs(L"Opened file " + fileName
				, ERRORTAG_ROLLFORNAME_L, L"open");
		}
		else
		{
			Logger::errlogs(L"Couldn't open file " + fileName
				, ERRORTAG_ROLLFORNAME_L, L"open");
		}
		return stream.is_open();
	}

	bool RollForName::loadTablesFromStream(std::wifstream& stream)
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

		if (completeFile.empty())
		{
			Logger::errlogs(L"File was empty");
			return false;
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
				ustring tId = makeValidId(t.name);

				if (!t.isValid())
				{
					Logger::errlogs(L"Parsed invalid table");
					return false;
				}
				else if (existsTable(tId))
				{
					Logger::errlogs(L"A table with identical name already exists");
					return false;
				}
				else
				{
					// Adding the table
					tables_.insert(make_pair(tId, t));
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

	bool RollForName::loadGeneratorsFromStream(std::wifstream& stream)
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

		if (completeFile.empty())
		{
			Logger::errlogs(L"File was empty");
			return false;
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
				ustring gId = makeValidId(g.name);
				if (!g.isValid())
				{
					Logger::errlogs(L"Parsed invalid generator");
					return false;
				}
				else if (existsGenerator(gId))
				{
					Logger::errlogs(L"A generator with identical name already exists");
					return false;
				}
				else
				{
					// Adding the table
					generators_.insert(make_pair(gId, g));

					// Loading additionnal generators / tables
					for (ustring s : g.requiredTables)
					{
						loadTablesFromFile(s, false);
					}

					for (ustring s : g.requiredGenerators)
					{
						loadGeneratorsFromFile(s, false);
					}
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
	bool RollForName::executeInstruction(Instruction i, int level, ustringVector & result)
	{
		Generator g;
		Table t;
		if (i.generator)
		{
			return generate(i.name, result, level);
		}
		else
		{
			return drawFromTable(i.name, result, level);
		}
	}
}