#pragma once

#include <rollForName/typedefs.h>

#define DEBUG

namespace rfn
{
#ifdef DEBUG
	const ustring PATH_RES = L"../../sources/res/";
#else
	const ustring PATH_RES = L"res/";
#endif
	const ustring PATH_TABLES = PATH_RES + L"tables/";
	const ustring PATH_GENERATORS = PATH_RES + L"generators/";
	const ustring FILE_EXTENSION = L".rfn";

	static ustring makePathTable(const ustring& tableName)
	{
		return PATH_TABLES + tableName + FILE_EXTENSION;
	}

	static ustring makePathGenerator(const ustring& generatorName)
	{
		return PATH_GENERATORS + generatorName + FILE_EXTENSION;
	}

	static ustring addExtension(const ustring& name)
	{
		return name + FILE_EXTENSION;
	}
}


