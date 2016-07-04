#pragma once

#include <rollForName/typedefs.h>

namespace rfn
{


	struct Generator
	{
		Generator(ustring name = L""
			, ustring requiredGenerators = L""
			, ustring requiredTables = L"");
		Generator(ustring name
			, ustring requiredGenerators
			, ustring requiredTables
			, std::vector<Instruction> instructions);

		bool isValid();

		bool operator==(const Generator& other);

		ustring name;
		ustring requiredGenerators;
		ustring requiredTables;
		std::vector<Instruction> instructions;
	};
}