#pragma once

#include "typedefs.h"

#include <unordered_map>
#include <string>

namespace rfn
{
	typedef std::unordered_map<ustring, int> Dictionary;


	//! @brief The Game Dictionary stores the game variables
	class GameDictionary
	{
	public:
		GameDictionary();

		//! If key exists in the Dictionary, return true.
		bool exists(ustring key) const;

		//! Returns the value associated to the given key
		int get(ustring key) const;

		//! Sets the given key with the given value. If the key doesn't exist, its created
		void set(ustring key, int value);

	private:
		Dictionary dictionary_;
	};

}
