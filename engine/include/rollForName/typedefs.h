#pragma once

#include <string>
#include <vector>

namespace rfn
{
	typedef std::wstring ustring;
	typedef std::string string;
	typedef std::vector<ustring> ustringVector;

	// One line instruction parsed in generator files
	struct Instruction
	{
		Instruction(bool generator = false, ustring name = L"")
			: generator(generator), name(name) {}

		bool generator;
		ustring name;

		bool operator==(const Instruction& other)
		{
			return (generator == other.generator)
				&& (name.compare(other.name) == 0);
		}

		bool operator!=(const Instruction& other)
		{
			return !(*this == other);
		}
	};

	struct Range
	{
		Range::Range() : min(0), max(0) {}
		Range::Range(int min, int max) : min(min), max(max) {}
		Range::Range(int nb) : min(nb), max(nb) {}

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

		void setNumber(int number)
		{
			min = number;
			max = number;
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

	struct Action
	{
		ustring variableName;
		int value;

		bool operator==(const Action& other)
		{
			return ((value == other.value)
				&& (variableName.compare(other.variableName) == 0));
		}

		bool operator!=(const Action& other)
		{
			return !(*this == other);
		}

		bool isValid()
		{
			return !variableName.empty();
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