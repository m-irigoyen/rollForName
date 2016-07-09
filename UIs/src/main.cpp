#include <rollForName\CommandLineHelper.h>
#include <rollForName\Logger.h>
#include <rollForName\StringHelpers.h>

using namespace rfn;

void interactiveMenu(RollForName& rfn, std::string& generatorName);
void generationMenu(RollForName& rfn, const std::string& generatorName);
void displayResults(ustringVector& result);

int main(int argc, char *argv[])
{
	// Parsing command line arguments

	RollForName rfn;
	std::string generatorName;
	ustringVector result;
	bool interactive = false;
	if (CommandLineHelper::parseAndApplyArguments(argc, argv, rfn, generatorName, interactive))
	{
		if (interactive)
		{
			interactiveMenu(rfn, generatorName);
		}
		else
		{
			if (generatorName.empty())
			{
				Logger::logs("No generator specified.\nBye!");
				return 0;
			}
			else
			{
				if (rfn.generate(generatorName, result, 0, true))
				{
					displayResults(result);
					generationMenu(rfn, generatorName);
				}
			}
		}
	}
	else
	{
		return 1;
	}

	std::cout << std::endl << "Bye!" << std::endl << std::endl;
	return 0;
}

void interactiveMenu(RollForName & rfn, std::string & generatorName)
{
	ustringVector result;
	bool keepGoing = true;
	while (keepGoing)
	{
		std::string line;
		std::cout << std::endl;
		std::cout << "\t____Interactive mode____" << std::endl;
		std::cout << "Current generator : " << generatorName << std::endl << std::endl;
		std::cout << "1 - Generate" << std::endl;
		std::cout << "2 - Change current generator" << std::endl;
		std::cout << "3 - Load generator file" << std::endl;
		std::cout << "4 - Load table file" << std::endl;
		std::cout << "5 - List available generators" << std::endl;
		std::cout << "0 - Quit" << std::endl;


		std::cout << "command : ";
		std::cin >> line;
		trim(line);
		lowercase(line);
		if ((line.compare("1") == 0)
			|| (line.compare("roll") == 0)
			|| (line.compare("r") == 0)
			|| (line.compare("generate") == 0)
			|| (line.compare("g") == 0)
			)
		{
			result.clear();
			if (rfn.generate(generatorName, result, 0, true))
			{
				displayResults(result);
			}
			else
			{
				Logger::logs("Couldn't generate " + generatorName);
				generatorName = "";;
			}
		}
		else if ((line.compare("2") == 0)
			|| (line.compare("change") == 0)
			|| (line.compare("c") == 0)
			)
		{
			std::cout << "Enter new generator : ";
			std::cin >> generatorName;
			trim(generatorName);
		}
		else if ((line.compare("3") == 0)
			|| (line.compare("load generator file") == 0)
			|| (line.compare("load generator") == 0)
			|| (line.compare("load generators") == 0)
			|| (line.compare("generator") == 0)
			|| (line.compare("lg") == 0)
			)
		{
			std::string load;
			std::cout << "Enter name (or relative path) of generator file to load : ";
			std::cin >> load;
			trim(load);

			std::cout << std::endl;
			if (rfn.loadGeneratorsFromFile(load))
			{
				std::cout << "Sucessfuly loaded generator file " << load << std::endl;
			}
		}
		else if ((line.compare("4") == 0)
			|| (line.compare("load table file") == 0)
			|| (line.compare("load table") == 0)
			|| (line.compare("load tables") == 0)
			|| (line.compare("table") == 0)
			)
		{
			std::string load;
			std::cout << "Enter name (or relative path) of table file to load : ";
			std::cin >> load;
			trim(load);

			std::cout << std::endl;
			if (rfn.loadTablesFromFile(load))
			{
				std::cout << "Sucessfuly loaded table file " << load << std::endl;
			}
		}
		else if ((line.compare("5") == 0)
			|| (line.compare("list") == 0)
			|| (line.compare("list generators") == 0)
			|| (line.compare("l") == 0)
			)
		{
			std::cout << "\tAvailable generators : " << std::endl;
			ustringVector v = rfn.listGenerators();
			for (ustring u : v)
			{
				std::wcout << u << std::endl;
			}
			std::cout << std::endl;
		}
		else if ((line.compare("0") == 0)
			|| (line.compare("quit") == 0)
			|| (line.compare("q") == 0)
			|| (line.compare("escape") == 0)
			|| (line.compare("esc") == 0)
			|| (line.compare("return") == 0)
			)
		{
			return;
		}
		else
		{
			std::cout << "Unrecognized command." << std::endl;
		}
	}
}

void generationMenu(RollForName& rfn, const std::string& generatorName)
{
	ustringVector result;
	bool keepGoing = true;
	while (keepGoing)
	{
		std::string line;
		std::cout << "Enter command :\n\reroll (shortcuts : 'r', '1')\n\tquit" << std::endl;
		std::cout << "command : ";
		std::cin >> line;
		trim(line);
		lowercase(line);
		if ((line.compare("1") == 0)
			|| (line.compare("reroll") == 0)
			|| (line.compare("roll") == 0)
			|| (line.compare("r") == 0)
			|| (line.compare("generate") == 0)
			|| (line.compare("g") == 0)
			)
		{
			result.clear();
			if (rfn.generate(generatorName, result, 0, true))
			{
				displayResults(result);
			}
			else
			{
				return;
			}
		}
		else
		{
			keepGoing = false;
		}
	}
}

void displayResults(ustringVector& result)
{
	std::wcout << std::endl << std::endl;
	Logger::logs(L"-------------------------------------------------\n---- GENERATING : "
		+ result.front() + L"\n-------------------------------------------------\n");
	for (ustringVector::iterator it = result.begin() +1;
		it != result.end();
		++it)
	{
		std::wcout << *it << std::endl;
	}
	std::wcout << std::endl
		<< L"-------------------------------------------------"
		<< std::endl << std::endl;;
}