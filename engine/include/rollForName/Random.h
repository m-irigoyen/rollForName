#pragma once

#include <rollForName/typedefs.h>

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
	};

	//! Returns true if the roll has been successfully made.
	bool rollDice(const ustring& rollDesc, int& rollResult);

	inline int rollrollDice(unsigned int nbDices, unsigned int nbFaces, std::vector<int> modifiers)
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

	inline int rollrollDice(unsigned int nbDices, unsigned int nbFaces)
	{
		return rollrollDice(nbDices, nbFaces, std::vector<int>());
	}

	inline int rollrollDice(Roll r)
	{
		return rollrollDice(r.nbDices, r.nbFaces, r.modifiers);
	}


}