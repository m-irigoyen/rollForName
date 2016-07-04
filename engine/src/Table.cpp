#include <rollForName/Table.h>

#include <algorithm>

namespace rfn
{
	Table::Table()
	{
	}
	int rfn::Table::size()
	{
		return entries.size();
	}

	TableEntry rfn::Table::getRandomEntry()
	{
		return entries.at(randomIntInclusive(0, entries.size() - 1));
	}

	TableEntry Table::getEntryAt(int x)
	{
		if ((entries.size() > 0)
			&& (x >= 0)
			&& (x < entries.size()))
		{
			return entries.at(x);
		}
		else
		{
			return TableEntry();
		}
	}

	void Table::addEntry(TableEntry entry)
	{
		entries.push_back(entry);
	}

	void Table::clear()
	{
		entries.clear();
	}

	bool Table::operator==(const Table & other)
	{
		// Checking entries
		if (entries.size() != other.entries.size())
			return false;

		for (int i = 0; i < entries.size(); ++i)
		{
			if (entries.at(i) != other.entries.at(i))
				return false;
		}
		// Comparing the rest
		return (name == other.name)
			&& (roll == other.roll)
			&& (requiredTables == other.requiredTables);
	}

	bool Table::isValid()
	{
		// Check there are no range intersections
		for (int i = 0; i < entries.size(); ++i)
		{
			for (int j = 0; j < entries.size(); ++j)
			{
				if (i != j)
				{
					entries.at(i).range.makeValid();
					entries.at(j).range.makeValid();
					if (entries.at(i).range.intersects(entries.at(j).range))
					{
						return false;
					}
				}
			}
		}
		return true;
	}

	void Table::orderEntries()
	{
		for (TableEntry te : entries)
		{
			te.range.makeValid();
		}
		std::sort(entries.begin(), entries.end());
	}

}

