#include <rollForName/TableManager.h>

#include <rollForName/FileUtils.h>
#include <rollForName/StringHelpers.h>

#include <rollForName/Parser.h>

namespace rfn
{
	bool TableManager::loadTables(std::string fileName)
	{
		wifstream stream;
		if (FileOpener::open(fileName, stream))
		{
			// The stream is open and ready to parse!
			ustring line;
			TableEntry t;
			while (stream.good() && !stream.eof())
			{
				std::getline(stream, line);
				trim(line);

				if (line.empty())
					continue;

				t.clear();

				if (Parser::isTableName(line, t.name))
				{
					// Found a new table!

				}
			}
		}

		return false;
	}
}

