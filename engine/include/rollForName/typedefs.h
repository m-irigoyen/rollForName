#pragma once

#include <string>
#include <vector>

namespace rfn
{
	typedef std::wstring ustring;
	typedef std::string string;

	struct Range
	{
		Range::Range(int min = 0, int max = 0) : min(min), max(max) {}

		int min;
		int max;

		bool operator==(const Range& other)
		{
			return (min == other.min) && (max == other.max);
		}

		//! Returns true if nb is in the given range
		bool isInRange(int nb)
		{
			return (nb >= min) && (nb <= max);
		}

		std::string toString()
		{
			return "<" + std::to_string(min) + "_" + std::to_string(max) + ">";
		}

		ustring toUstring()
		{
			return L"<" + std::to_wstring(min) + L"_" + std::to_wstring(max) + L">";
		}

		void makeValid()
		{
			if (max < min)
			{
				std::swap(min, max);
			}
		}
	};

	struct TableEntry
	{
		TableEntry(Range r, ustring name, ustring description, ustring toOther) :
			range(r), name(name), description(description), toOtherTable(toOther) {}

		Range range;
		ustring name;
		ustring description;
		ustring toOtherTable;

		void clear() {
			range.min = 0;
			range.max = 0;
			name.clear();
			description.clear();
			toOtherTable.clear();
		}
	};

	typedef std::vector<TableEntry> entryVector;
}