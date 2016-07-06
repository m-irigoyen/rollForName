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
		makeValidIdInPlace(key);
		return dictionary_.find(key) != dictionary_.end();
	}

	int GameDictionary::get(ustring key) const
	{
		makeValidIdInPlace(key);
		Dictionary::const_iterator it = dictionary_.find(key);
		if (it != dictionary_.end())
		{
			return it->second;
		}
		else
		{
			return 0;
		}
	}

	void GameDictionary::set(ustring key, int value)
	{
		makeValidIdInPlace(key);
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


