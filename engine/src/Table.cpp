#include <rollForName/Table.h>

namespace rfn
{
	Table::Table()
	{
	}
	int rfn::Table::size()
	{
		return entries_.size();
	}

	TableEntry rfn::Table::getRandomEntry()
	{
		return entries_.at(randomIntInclusive(0, entries_.size() - 1));
	}

	TableEntry Table::getEntryAt(int x)
	{
		if ((entries_.size() > 0)
			&& (x >= 0)
			&& (x < entries_.size()))
		{
			return entries_.at(x);
		}
		else
		{
			return TableEntry();
		}
	}

	void Table::setName(ustring name)
	{
		name_ = name;
	}

	void Table::addEntry(TableEntry entry)
	{
		entries_.push_back(entry);
	}

	void Table::clear()
	{
		entries_.clear();
	}

}

