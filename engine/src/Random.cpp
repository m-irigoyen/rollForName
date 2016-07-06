#include <rollForName/Random.h>

#include <rollForName/Parser.h>
#include <rollForName/StringHelpers.h>


namespace rfn
{
	bool rollDice(const ustring& line, int& rollResult, const GameDictionary* dictionary)
	{
		RollAndMod r;
		rollResult = 0;

		ustring lineCopy = line;
		removeSpaces(lineCopy);

		if (Parser::parseRoll(lineCopy, r))
		{
			for (Roll oneRoll : r.rolls)
			{
				rollResult += rollDice(oneRoll);
			}

			if ((!r.mod.empty())
				&& (dictionary != NULL)
				&& (dictionary->exists(r.mod)))
			{
				rollResult += dictionary->get(r.mod);
			}
			return true;
		}
		else
		{
			return false;
		}
	}
}