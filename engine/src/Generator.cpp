#include <rollForName/Generator.h>

#include <rollForName/FileUtils.h>
#include <rollForName/Logger.h>
#include <rollForName/StringHelpers.h>

#define ERRORTAG_GENERATOR "Generator"

namespace rfn
{
	Generator::Generator(ustring name, ustring requiredGenerators, ustring requiredTables)
		: name(name)
		, requiredGenerators(requiredGenerators)
		, requiredTables(requiredTables)
	{
	}

	Generator::Generator(ustring name, ustring requiredGenerators, ustring requiredTables, std::vector<Instruction> instructions)
		: name(name)
		, requiredGenerators(requiredGenerators)
		, requiredTables(requiredTables)
		, instructions(instructions)
	{
	}

	bool Generator::isValid()
	{
		return !name.empty() && !instructions.empty();
	}


	bool Generator::operator==(const Generator & other)
	{
		if (instructions.size() != other.instructions.size())
			return false;

		for (int i = 0; i < instructions.size(); ++i)
		{
			if (!(instructions[i] == other.instructions[i]))
				return false;
		}

		return (name.compare(other.name) == 0)
			&& (requiredGenerators.compare(other.requiredGenerators) == 0)
			&& (requiredTables.compare(other.requiredTables) == 0);
	}
}

