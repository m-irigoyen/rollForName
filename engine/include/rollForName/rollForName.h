#pragma once

#include <rollForName/Generator.h>
#include <rollForName/TableManager.h>

#include <map>

namespace rfn
{
	// Generator manager, and loading tables and all
	class RollForName
	{
	public :

		RollForName();

		bool loadTablesFromFile(std::string fileName);
		bool loadGeneratorsFromFile(std::string fileName);
		bool loadFromFile(std::string fileName);

		bool generate(std::string generatorName);

	private:
		std::map<ustring, Generator> generators_;
		std::map<ustring, Table> tables_;
	};
}