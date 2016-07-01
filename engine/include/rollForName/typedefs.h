#pragma once

#include <string>
#include <vector>

namespace rfn
{
	typedef std::wstring ustring;
	typedef std::string string;
	typedef std::vector<ustring> ustringVector;

	struct Range
	{
		Range::Range(int min = 0, int max = 0) : min(min), max(max) {}

		int min;
		int max;

		bool operator==(const Range& other)
		{
			return (min == other.min) && (max == other.max);
		}

		bool operator< (const Range& other)
		{
			return max < other.min;
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

		int size()
		{
			return max - min + 1;
		}

		//! Returns true if the two ranges intersect
		bool intersects(Range& other)
		{
			makeValid();
			other.makeValid();
			return ((min >= other.min) && (min <= other.max))
				|| ((max >= other.min) && (max <= other.max))
				|| ((other.min >= min) && (other.min <= max))
				|| ((other.max >= min) && (other.max <= max));
		}
	};

	struct TableEntry
	{
		TableEntry() {};
		TableEntry(Range r, ustring name, ustring description) :
			range(r), name(name), description(description) {}

		Range range;
		ustring name;
		ustring description;

		void clear() {
			range.min = 0;
			range.max = 0;
			name.clear();
			description.clear();
		}

		bool operator==(const TableEntry& other)
		{
			return (range == other.range)
				&& (name == other.name)
				&& (description == other.description);
		}

		bool operator !=(const TableEntry& other)
		{
			return !(*this == other);
		}

		bool operator< (const TableEntry& other)
		{
			return range < other.range;
		}
	};

	typedef std::vector<TableEntry> entryVector;
}