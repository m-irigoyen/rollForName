#pragma once

#include <string>

#define DEBUG

namespace rfn
{
#ifdef DEBUG
	const std::string PATH_RES = "../../sources/res/";
#else
	const std::string PATH_RES = "res/";
#endif
	const std::string PATH_TABLES = PATH_RES + "tables/";
	const std::string PATH_GENERATORS = PATH_RES + "generators/";
	const std::string FILE_EXTENSION = ".rfn";

	static std::string makePathTable(std::string tableName)
	{
		return PATH_TABLES + tableName + FILE_EXTENSION;
	}

	static std::string makePathGenerator(std::string generatorName)
	{
		return PATH_GENERATORS + generatorName + FILE_EXTENSION;
	}
}


