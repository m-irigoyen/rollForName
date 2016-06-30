#include <rollForName/Logger.h>

namespace rfn
{
		// WSTRING
	void Logger::logs(ustring message)
	{
		std::wcout << message.data() << std::endl;
	}

	void Logger::logs(ustring message, ustring author)
	{
		std::wcout << "[" << author.data() << "] : " << message.data() << std::endl;
	}

	void Logger::logs(ustring message, ustring author, ustring section)
	{
		std::wcout << "[" << author.data() << ":" << section.data() << "] : "
			<< message.data() << std::endl;
	}

	void Logger::errlogs(ustring message)
	{
		std::wcerr << "ERROR : " << message.data() << std::endl;
	}

	void Logger::errlogs(ustring message, ustring author)
	{
		std::wcerr << "[" << author.data() << "] ERROR : " << message.data() << std::endl;
	}

	void Logger::errlogs(ustring message, ustring author, ustring section)
	{
		std::wcerr << "[" << author.data() << ":" << section.data() << "] ERROR : "
			<< message.data() << std::endl;
	}

			// STRING
	void Logger::logs(string message)
	{
		std::cout << message.data() << std::endl;
	}

	void Logger::logs(string message, string author)
	{
		std::cout << "[" << author.data() << "] : " << message.data() << std::endl;
	}

	void Logger::logs(string message, string author, string section)
	{
		std::cout << "[" << author.data() << ":" << section.data() << "] : "
			<< message.data() << std::endl;
	}

	void Logger::errlogs(string message)
	{
		std::cerr << "ERROR : " << message.data() << std::endl;
	}

	void Logger::errlogs(string message, string author)
	{
		std::cerr << "[" << author.data() << "] ERROR : " << message.data() << std::endl;
	}

	void Logger::errlogs(string message, string author, string section)
	{
		std::cerr << "[" << author.data() << ":" << section.data() << "] ERROR : "
			<< message.data() << std::endl;
	}

	void Logger::logNewLine()
	{
		std::cout << std::endl;
	}

	void Logger::errlogNewLine()
	{
		std::cerr << std::endl;
	}


}


