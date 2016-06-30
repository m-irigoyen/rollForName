#include <rollForName/Generator.h>

#include <rollForName/FileUtils.h>
#include <rollForName/Logger.h>

#define ERRORTAG_GENERATOR "Generator"

namespace rfn
{

}

rfn::Generator::Generator() : ready_(false)
{
}

bool rfn::Generator::init()
{
	std::string tableName = "base.tbl";
	if (!manager_.loadTables(tableName))
	{
		Logger::errlogs("Couldn't load tables " + tableName
			, ERRORTAG_GENERATOR, "init");
		return false;
	}

	ready_ = true;
	return true;
}
