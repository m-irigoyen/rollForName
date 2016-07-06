#pragma once

#include <rollForName/Generator.h>
#include <rollForName/TableManager.h>
#include <rollForName\GameDictionary.h>

#include <map>
#include <fstream>

namespace rfn
{
	// Generator manager, and loading tables and all
	class RollForName
	{
	public :

		RollForName();

		bool loadTablesFromFile(std::string fileName, bool isCompletePath = false);
		bool loadGeneratorsFromFile(std::string fileName, bool isCompletePath = false);
		bool loadTablesFromFile(ustring fileName, bool isCompletePath = false);
		bool loadGeneratorsFromFile(ustring fileName, bool isCompletePath = false);
		bool loadFromFile(std::string fileName);
		bool loadFromFile(ustring fileName);

		bool generate(ustring generatorName, ustringVector& result, int level = 0, bool isTopLevel = false);
		bool generate(std::string generatorName, ustringVector& result, int level = 0, bool isTopLevel = false);

		bool drawFromTable(ustring tableName, ustringVector& result, int level);

	private:
		typedef std::map<ustring, Generator> GeneratorMap;
		typedef std::map<ustring, Table> TableMap;

		GeneratorMap generators_;
		TableMap tables_;
		std::vector<ustring> loadedTableFiles_,
			loadedGeneratorFiles_;	// The name of every table/generator file that already loaded
		GameDictionary dictionary_;

		// Open file
		bool openFile(ustring fileName, std::wifstream& stream);

		// Loading
		bool loadTablesFromStream(std::wifstream& stream);
		bool loadGeneratorsFromStream(std::wifstream& stream);

		bool existsTable(ustring tableName);
		bool existsTable(ustring tableName, Table& table);
		bool existsGenerator(ustring tableName);
		bool existsGenerator(ustring tableName, Generator& generator);
		bool executeInstruction(Instruction i, int level, ustringVector& result);
	};
}