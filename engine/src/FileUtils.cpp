#include <rollForName/FileUtils.h>
#include <rollForName/Logger.h>

#include <iostream>
#include <locale>
#include <codecvt>

#define ERRORTAG_FILEUTILS "FileUtils"

namespace rfn
{

	bool FileOpener::open(std::string path, wifstream & stream)
	{
		// Open given file
		stream.open(path);
		stream.imbue(std::locale(stream.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>()));

		if (stream.is_open())
		{
			Logger::logs("Opened file " + path
				, ERRORTAG_FILEUTILS, "open");
		}
		else
		{
			Logger::errlogs("Couldn't open file " + path
				, ERRORTAG_FILEUTILS, "open");
		}
		return stream.is_open();
	}
}


