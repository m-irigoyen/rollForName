#pragma once

#include "Table.h"

namespace rfn
{

	class TableManager
	{
	public:

		bool loadTables(std::string fileName);
		void clearTables();
	};

}