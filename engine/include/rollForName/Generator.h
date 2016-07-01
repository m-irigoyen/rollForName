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
			, ustringVector instructions);

		bool isValid();

		bool generate(ustring& generated);


		ustring name;
		ustring requiredGenerators;
		ustring requiredTables;
		ustringVector instructions;
	};
}