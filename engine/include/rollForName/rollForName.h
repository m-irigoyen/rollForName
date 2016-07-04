#pragma once

#include <rollForName/Generator.h>
#include <rollForName/TableManager.h>

#include <map>
#include <fstream>

namespace rfn
{
	// Generator manager, and loading tables and all
	class RollForName
	{
	public :

		RollForName();

		bool loadTablesFromFile(std::string fileName, bool completePath = false);
		bool loadGeneratorsFromFile(std::string fileName, bool completePath = false);
		bool loadFromFile(std::string fileName);

		bool generate(ustring generatorName, ustring& result, bool isTopLevel = false);

		bool drawFromTable(ustring tableName, ustring& result);

	private:
		typedef std::map<ustring, Generator> GeneratorMap;
		typedef std::map<ustring, Table> TableMap;

		GeneratorMap generators_;
		TableMap tables_;

		// Open file
		bool openFile(std::string fileName, std::wifstream& stream);

		bool existsTable(ustring tableName);
		bool existsTable(ustring tableName, Table& table);
		bool existsGenerator(ustring tableName);
		bool existsGenerator(ustring tableName, Generator& generator);
		bool executeInstruction(Instruction i, ustring& result);
	};
}