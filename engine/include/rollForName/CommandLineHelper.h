#pragma once

#include <rollForName/rollForName.h>
#include <iostream>

namespace rfn
{
	class CommandLineHelper
	{
	public:
		static bool parseAndApplyArguments(int argc
			, char *argv[]
			, RollForName& rfn
			, std::string& generatorName
			, bool& interactive);

		//! Displays help in the console
		static void help();

		static bool parseFilename(const std::string& line, std::string& filename);
		static bool parseIsAbsolutePath(const std::string& line, bool& isAbsolutePath);
		static bool parseGeneratorName(const std::string& line, std::string& generatorName);
		static bool parseIsInteractive(const std::string& line, bool& isInteractive);
	};
}