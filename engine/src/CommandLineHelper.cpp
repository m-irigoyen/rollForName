#include <rollForName/CommandLineHelper.h>
#include <rollForName/Logger.h>
#include <rollForName/StringHelpers.h>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#define ERRORTAG_CMDLINEHELPER_L L"CommandLineHelper"
#define ERRORTAG_CMDLINEHELPER "CommandLineHelper"

// Using directives
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

namespace rfn
{
	struct commandLineOption
	{
		std::string option;
		std::string value;
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	rfn::commandLineOption,
	(std::string, option)
	(std::string, value)
)

namespace rfn
{
	bool CommandLineHelper::parseAndApplyArguments(int argc
		, char * argv[]
		, RollForName & rfn
		, std::string& generatorName
		, bool& interactive)
	{
		if ((argc == 1) || (argc == 0))
		{
			interactive = true;
			return true;
		}
		else if ((argc < 2) || (argc > 5))
		{
			std::cout << "Invalid number of arguments" << std::endl;
			help();
			return false;
		}
		else
		{
			// Parsing arguments.
			std::string filename;
			bool isAbsolute = false;

			for (int i = 1; i < argc; ++i)
			{
				std::string s(argv[i]);
				{
					std::string result;
					bool boolResult;
					if (parseFilename(s, result))
					{
						filename = lowercased(result);
					}
					else if (parseGeneratorName(s, result))
					{
						generatorName = lowercased(result);
					}
					else if (parseIsAbsolutePath(s, boolResult))
					{
						isAbsolute = boolResult;
					}
					else if (parseIsInteractive(s, boolResult))
					{
						interactive = boolResult;
					}
				}
			}

			if (!interactive && generatorName.empty())
			{
				Logger::errlogs("Invalid command");
				help();
				return false;
			}

			if (filename.empty())
			{
				filename = generatorName;
			}

			if (!filename.empty() && !rfn.loadGeneratorsFromFile(filename, isAbsolute))
			{
				Logger::errlogs("Error while loading file : " + filename);
				return false;
			}

			return true;
		}
	}

	void CommandLineHelper::help()
	{
		using namespace std;
		cout << "Usage : rollForName [--interactive] [--filename] [--absolute-path] [--generator]"
			<< endl << endl;;

		cout << "options : " << endl;
		cout << "\t--generator" << endl << "\t\tThe generator to use." << endl << endl;
		cout << "\t--filename" << endl << "\t\tThe file where the generator is written." << endl
			<< "\t\tExpects either a name without spaces, or a name within quotes (\"\")" << endl;
		cout << "\t--absolute-path (default : false)" << endl << "\t\tIndicates if the given filename is an absolute or relative path."
			<< endl << "\t\tExpected values : true, false, 1, 0" << endl << endl;
		cout << "\t--interactive (default : false)" << endl << "\t\tInteractive mode."
			<< endl << "\t\tExpected values : true, false, 1, 0" << endl << endl;
	}


	// Parsing

	// Parser that reads a string between quotes
	template <typename Iterator, typename Skipper = boost::spirit::standard::space_type>
	struct quotedTextParser : public qi::grammar<Iterator, std::string(), Skipper> // return type is Table
	{
	public:
		quotedTextParser() : quotedTextParser::base_type(start)
		{
			start %= qi::lexeme[qi::lit("\"") >> *(qi::char_ - "\"") >> qi::lit("\"")];
		}

		qi::rule<Iterator, std::string(), Skipper> start;
	};

	// Parser that reads a filename option, and returns the filename
	template <typename Iterator, typename Skipper = boost::spirit::standard::space_type>
	struct commandLineOptionParser : public qi::grammar<Iterator, commandLineOption(), Skipper> // return type is Table
	{
	public:
		commandLineOptionParser() : commandLineOptionParser::base_type(start)
		{
			start %= qi::lexeme[qi::lit("--")
				>> *(qi::char_ - '=')
				>> ((qi::lit("=") >> *qi::char_)
				| qi::attr(""))];
		}

		quotedTextParser<Iterator, Skipper> quotedText;
		qi::rule<Iterator, commandLineOption(), Skipper> start;
	};

	bool CommandLineHelper::parseFilename(const std::string & line, std::string & result)
	{
		std::string lineCopy = line;
		commandLineOption clo;

		commandLineOptionParser<std::string::iterator, boost::spirit::standard::space_type> parser;
		std::string::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard::space
			, clo);

		if (parseResult)
		{
			std::transform(clo.option.begin(), clo.option.end(), clo.option.begin(), ::tolower);
			if (clo.option.compare("filename") == 0)
			{
				result = clo.value;
				return true;
			}
		}
		return false;
	}

	bool CommandLineHelper::parseIsAbsolutePath(const std::string & line, bool & isAbsolutePath)
	{
		std::string lineCopy = line;
		commandLineOption clo;

		commandLineOptionParser<std::string::iterator, boost::spirit::standard::space_type> parser;
		std::string::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard::space
			, clo);

		if (parseResult)
		{
			std::transform(clo.option.begin(), clo.option.end(), clo.option.begin(), ::tolower);
			if (clo.option.compare("absolute-path") == 0)
			{
				std::transform(clo.value.begin(), clo.value.end(), clo.value.begin(), ::tolower);
				if ((clo.value.compare("false") == 0) || (clo.value.compare("0") == 0))
				{
					isAbsolutePath = false;
				}
				else
				{
					isAbsolutePath = true;
				}
				return true;
			}
		}
		return false;
	}

	bool CommandLineHelper::parseGeneratorName(const std::string & line, std::string& result)
	{
		std::string lineCopy = line;
		commandLineOption clo;

		commandLineOptionParser<std::string::iterator, boost::spirit::standard::space_type> parser;
		std::string::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard::space
			, clo);

		if (parseResult)
		{
			std::transform(clo.option.begin(), clo.option.end(), clo.option.begin(), ::tolower);
			if (clo.option.compare("generator") == 0)
			{
				result = clo.value;
				return true;
			}
		}
		return false;
	}

	bool CommandLineHelper::parseIsInteractive(const std::string & line, bool & isInteractive)
	{
		std::string lineCopy = line;
		commandLineOption clo;

		commandLineOptionParser<std::string::iterator, boost::spirit::standard::space_type> parser;
		std::string::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard::space
			, clo);

		if (parseResult)
		{
			std::transform(clo.option.begin(), clo.option.end(), clo.option.begin(), ::tolower);
			if (clo.option.compare("interactive") == 0)
			{
				std::transform(clo.value.begin(), clo.value.end(), clo.value.begin(), ::tolower);
				if ((clo.value.compare("false") == 0) || (clo.value.compare("0") == 0))
				{
					isInteractive = false;
				}
				else
				{
					isInteractive = true;
				}
				return true;
			}
		}
		return false;
	}
}