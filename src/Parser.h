#pragma once

#include "Table.h"

#include <fstream>


/*
Tables must be of the following form :
TableName
{
"Item1", "Item 1 description"
"Item2", "Item 2 description"
"Item3", "Item 3 description"
}

with as many items as you want
*/

namespace rfn
{
	class Parser
	{
	public:
		//! Returns true if the line is a table name
		static bool isTableName(ustring line, ustring& name);

		//! If a table was succesfully parsed, returns true
		static bool parseTable(std::wifstream& stream, Table& t);
	};
}