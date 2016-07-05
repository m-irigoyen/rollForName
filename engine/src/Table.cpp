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
		// Checking vector sizes
		if ((entries.size() != other.entries.size())
			|| (requiredTables.size() != other.requiredTables.size()))
			return false;

		// Name and roll
		if ((name.compare(other.name) != 0)
			|| (roll.compare(other.roll) !=0))
		{
			return false;
		}

		// entries
		for (int i = 0; i < entries.size(); ++i)
		{
			if (entries.at(i) != other.entries.at(i))
				return false;
		}

		// required tables
		for (int i = 0; i < requiredTables.size(); ++i)
		{
			if (requiredTables.at(i).compare(other.requiredTables.at(i)) != 0)
				return false;
		}

		return true;
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

