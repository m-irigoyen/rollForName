#pragma once

#include <rollForName/Table.h>
#include <rollForName/Generator.h>

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
		// File navigator helper
		static bool findNextBracketEnd(ustring::iterator& begin, ustring::iterator end, ustring& skipped);

		//! Returns true if the line is a table name
		static bool isTableName(const ustring& line, ustring& name);

		//! If the parse was successful, returns true
		static bool parseTableEntry(const ustring& line, TableEntry& t);

		//! If the parse was successful, returns true
		static bool parseTable(const ustring& line, Table& t);

		static bool parseTable(ustring::iterator& begin, ustring::iterator end, Table& t);

		//! If the parse was successful, returns true
		static bool parseRange(const ustring& line, Range& result);

		//! If the parse was successful, returns true
		static bool parseRoll(const ustring& line, std::vector<Roll>& result);

		//! If the parse was successful, returns true
		static bool parseGeneratorInstruction(const ustring& line, Instruction& i);

		//! If the parse was successful, returns true
		static bool parseGenerator(const ustring& line, Generator& gen);
	};
}