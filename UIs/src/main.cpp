#include <rollForName\rollForName.h>
#include <rollForName\Logger.h>

using namespace rfn;

int main()
{
	RollForName rfn;
	/*if (rfn.loadTablesFromFile("testTable"))
	{
		Logger::logs("Successfully loaded test tables");
	}
	else
	{
		Logger::logs("Failed to load test tables");
	}*/
	if (rfn.loadGeneratorsFromFile("testGenerator"))
	{
		Logger::logs("Successfully loaded test generator");
	}
	else
	{
		Logger::logs("Couldn't load test generator");
	}

	ustring result;
	rfn.generate(L"testGenerator", result, true);

	std::wcout << L"Result :\n" << result << std::endl;

	return 0;
}