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

		bool generate(std::string generatorName);

	private:
		std::map<ustring, Generator> generators_;
		std::map<ustring, Table> tables_;

		// Open file
		bool openFile(std::string fileName, std::wifstream& stream);

		bool existsTable(ustring tableName);
		bool existsGenerator(ustring tableName);
	};
}