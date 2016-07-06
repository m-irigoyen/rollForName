#include <rollForName\CommandLineHelper.h>
#include <rollForName\Logger.h>

using namespace rfn;

int main(int argc, char *argv[])
{
	// Parsing command line arguments

	RollForName rfn;
	std::string generatorName;
	if (CommandLineHelper::parseAndApplyArguments(argc, argv, rfn, generatorName))
	{
		ustringVector result;
		rfn.generate(generatorName, result, 0, true);

		std::wcout << std::endl << std::endl;
		std::wcout << L"---------------------------" << std::endl;
		for (ustring s : result)
		{
			std::wcout << s << std::endl;
		}
	}

	return 0;
}