#include <rollForName/Random.h>

#include <rollForName/Parser.h>
#include <rollForName/StringHelpers.h>


namespace rfn
{
	bool roll(const ustring& line, int& rollResult)
	{
		std::vector<Roll> r;
		rollResult = 0;

		ustring lineCopy = line;
		removeSpaces(lineCopy);

		if (Parser::parseRoll(lineCopy, r))
		{
			for (Roll oneRoll : r)
			{
				rollResult += roll(oneRoll);
			}
		}
		else
		{
			return false;
		}
	}

}