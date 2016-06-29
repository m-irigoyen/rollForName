#pragma once

#include "typedefs.h"
#include "Random.h"



namespace rfn
{
	class Table
	{
	public :
		Table();

		//! Returns the number of elements
		int size();

		TableEntry getRandomEntry();
		TableEntry getEntryAt(int x);

		void setName(ustring name);
		void addEntry(TableEntry entry);

		void clear();

	protected:
		ustring name_;
		entryVector entries_;
		ustring roll_;
		ustring requiredTables_;
	};
}