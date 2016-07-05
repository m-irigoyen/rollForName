#include <rollForName/Generator.h>

#include <rollForName/FileUtils.h>
#include <rollForName/Logger.h>
#include <rollForName/StringHelpers.h>

#define ERRORTAG_GENERATOR "Generator"

namespace rfn
{
	Generator::Generator(ustring name) : name(name)
	{
	}

	Generator::Generator(ustring name
		, ustringVector requiredGenerators
		, ustringVector requiredTables)
		: name(name)
		, requiredGenerators(requiredGenerators)
		, requiredTables(requiredTables)
	{
	}

	Generator::Generator(ustring name
		, ustringVector requiredGenerators
		, ustringVector requiredTables
		, std::vector<Instruction> instructions)
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
		// Comparing vector sizes
		if ((instructions.size() != other.instructions.size())
			|| (requiredTables.size() != other.requiredTables.size())
			|| (requiredGenerators.size() != other.requiredGenerators.size()))
		{
			return false;
		}

		// Comparing names
		if (name.compare(other.name) != 0)
		{
			return false;
		}

		// Instructions
		for (int i = 0; i < instructions.size(); ++i)
		{
			if (instructions[i] != other.instructions[i])
				return false;
		}

		// Required tables
		for (int i = 0; i < requiredTables.size(); ++i)
		{
			if (requiredTables[i].compare(other.requiredTables[i]) != 0)
				return false;
		}

		// Required generators
		for (int i = 0; i < requiredGenerators.size(); ++i)
		{
			if (requiredGenerators[i].compare(other.requiredGenerators[i]) != 0)
				return false;
		}


		return true;
	}
}

