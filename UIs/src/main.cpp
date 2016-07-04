#include <rollForName\rollForName.h>

using namespace rfn;

int main()
{
	RollForName rfn;
	rfn.loadTablesFromFile("testTable");
	rfn.loadTablesFromFile("testGenerator");

	return 0;
}