#include <rollForName/GameDictionary.h>
#include <rollForName/StringHelpers.h>

#include <algorithm>

namespace rfn
{

	GameDictionary::GameDictionary()
	{
	}

	bool GameDictionary::exists(ustring key) const
	{
		makeValidId(key);
		return dictionary_.find(key) != dictionary_.end();
	}

	ustring GameDictionary::get(ustring key)
	{
		makeValidId(key);
		Dictionary::iterator it = dictionary_.find(key);
		if (it != dictionary_.end())
		{
			return it->second;
		}
		else
		{
			return ustring();
		}
	}

	void GameDictionary::set(ustring key, ustring value)
	{
		makeValidId(key);
		Dictionary::iterator it = dictionary_.find(key);
		if (it != dictionary_.end())
		{
			it->second = value;
		}
		else
		{
			dictionary_.insert(std::make_pair(key, value));
		}
	}

}


