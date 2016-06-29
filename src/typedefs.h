#pragma once

#include <string>
#include <vector>

namespace rfn
{
	typedef std::wstring ustring;

	struct Range
	{
		Range::Range() : min(0), max(0) {}

		int min;
		int max;

		//! Returns true if nb is in the given range
		bool isInRange(int nb)
		{
			return (nb >= min) && (nb <= max);
		}
	};

	struct TableEntry
	{
		Range range;
		ustring name;
		ustring descrpition;
		ustring toOtherTable;

		void clear() {
			range.min = 0;
			range.max = 0;
			name.clear();
			descrpition.clear();
			toOtherTable.clear();
		}
	};

	typedef std::vector<TableEntry> entryVector;
}