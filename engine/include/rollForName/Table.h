#pragma once

#include "typedefs.h"
#include "Random.h"



namespace rfn
{
	struct Table
	{
	public :
		Table();

		//! Returns the number of elements
		int size();

		TableEntry getRandomEntry();
		TableEntry getEntryAt(int rollResult);

		void addEntry(TableEntry entry);

		void clear();

		bool operator==(const Table& other);

		//! returns true if the roll's range is respected by the table entries
		bool isValid();

		//! Orders the entries in ascending order of range
		void orderEntries();

		ustring name;
		entryVector entries;
		ustring roll;
		ustringVector requiredTables;
	};
}