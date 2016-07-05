#pragma once

#include <rollForName/typedefs.h>

namespace rfn
{


	struct Generator
	{
		Generator(ustring name = L"");
		Generator(ustring name
			, ustringVector requiredGenerators
			, ustringVector requiredTables);
		Generator(ustring name
			, ustringVector requiredGenerators
			, ustringVector requiredTables
			, std::vector<Instruction> instructions);

		bool isValid();

		bool operator==(const Generator& other);

		ustring name;
		ustringVector requiredGenerators;
		ustringVector requiredTables;
		std::vector<Instruction> instructions;
	};
}