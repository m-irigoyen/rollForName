#pragma once

#include <random>

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

	inline int roll(ustring rollDesc)
	{
		//TODO : parse the roll desc, and roll it
		return 0;
	}

	inline int roll(unsigned int nbDices, unsigned int nbFaces)
	{
		if ((nbDices == 0) || (nbFaces == 0))
			return -1;

		int total = 0;
		for (int i = 0; i < nbDices; ++i)
		{
			total += randomIntInclusive(1, nbFaces);
		}

		return total;
	}

	struct Roll
	{
		unsigned int nbDices;
		unsigned int nbFaces;
	};
}