#pragma once

#include <rollForName/typedefs.h>
#include <rollForName\GameDictionary.h>

#include <random>
#include <vector>

namespace rfn
{
	// Returns an int between min and max (included)
	inline int randomIntInclusive(int min, int max)
	{
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 eng(rd()); // seed the generator
		std::uniform_int_distribution<> distr(min, max); // define the range

		return distr(eng);
	}

	// DICE ROLLS
	struct Roll
	{
		Roll::Roll() : nbFaces(0), nbDices(0) {}

		unsigned int nbDices;
		unsigned int nbFaces;
		std::vector<int> modifiers;

		bool operator==(const Roll& other)
		{
			if (modifiers.size() != other.modifiers.size())
			{
				return false;
			}
			for (int i = 0; i < modifiers.size(); ++i)
			{
				if (modifiers[i] != other.modifiers[i])
					return false;
			}
			return (
				(nbDices == other.nbDices)
				&& (nbFaces == other.nbFaces)
				);
		}
		bool operator!=(const Roll& other)
		{
			return !(*this == other);
		}
	};

	struct RollAndMod
	{
		std::vector<Roll> rolls;
		ustring mod;

		bool operator == (const RollAndMod& other)
		{
			if (rolls.size() != other.rolls.size())
			{
				return false;
			}
			for (int i = 0; i < rolls.size(); ++i)
			{
				if (rolls[i] != other.rolls[i])
					return false;
			}

			return mod.compare(other.mod) == 0;
		}
	};

	//! Returns true if the roll has been successfully made.
	bool rollDice(const ustring& rollDesc, int& rollResult, const GameDictionary* dictionary = NULL);

	inline int rollDice(unsigned int nbDices, unsigned int nbFaces, std::vector<int> modifiers)
	{
		if ((nbDices == 0) || (nbFaces == 0))
			return -1;

		int total = 0;
		for (unsigned int i = 0; i < nbDices; ++i)
		{
			total += randomIntInclusive(1, nbFaces);
		}
		for (int mod : modifiers)
		{
			total += mod;
		}

		return total;
	}

	inline int rollDice(unsigned int nbDices, unsigned int nbFaces)
	{
		return rollDice(nbDices, nbFaces, std::vector<int>());
	}

	inline int rollDice(Roll r)
	{
		return rollDice(r.nbDices, r.nbFaces, r.modifiers);
	}
}