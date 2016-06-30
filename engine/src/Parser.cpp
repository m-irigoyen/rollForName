#include <rollForName/Parser.h>
#include <rollForName/Logger.h>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

// Using directives
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

// Boost::fusion adapting structures
BOOST_FUSION_ADAPT_STRUCT(
	rfn::Range,
	(int, min)
	(int, max)
	)

BOOST_FUSION_ADAPT_STRUCT(
	rfn::Roll,
	(unsigned int, nbDices)
	(unsigned int, nbFaces)
	(std::vector<int>, modifiers)
	)

namespace rfn
{
	// Parser that reads an affectation operator
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct quotedTextParser : public qi::grammar<Iterator, ustring(), Skipper> // return type is Table
	{
	public:
		quotedTextParser() : quotedTextParser::base_type(start)
		{
			start %= qi::lit("\"") >> *(qi::char_ - "\"") >> qi::lit("\"");
		}

		qi::rule<Iterator, ustring(), Skipper> start;
	};

	// Parser that reads a range
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct rangeParser : public qi::grammar<Iterator, Range(), Skipper>
	{
	public:
		rangeParser() : rangeParser::base_type(start)
		{
			start %= qi::lit("<")
				>> qi::int_
				>> qi::lit("-")
				>> qi::int_
				>> qi::lit(">");
		}

		qi::rule<Iterator, Range(), Skipper> start;
	};

	// Parser that reads a dice roll
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct rollParser : public qi::grammar<Iterator, std::vector<Roll>(), Skipper>
	{
	public:
		rollParser() : rollParser::base_type(start)
		{
			roll %= qi::int_
				>> qi::no_case[qi::lit("d")]
				>> qi::int_
				>> *(qi::int_
					>> (&qi::lit("+") | &qi::lit("-") | &qi::lit("]")));

			start %= qi::lit("[")
				>> *roll
				>> qi::lit("]");
		}

		qi::rule<Iterator, Roll, Skipper> roll;
		qi::rule<Iterator, std::vector<Roll>(), Skipper> start;
	};


		// FUNCTIONS

	bool Parser::isTableName(const ustring& line, ustring& name)
	{
		ustring lineCopy = line;

		quotedTextParser<ustring::iterator, boost::spirit::standard_wide::space_type> g;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, g
			, boost::spirit::standard_wide::space
			, name);

		if (parseResult)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Parser::parseTable(const ustring & line, Table & t)
	{
		return false;
	}

	bool Parser::parseRange(const ustring& line, Range& result)
	{
		ustring lineCopy = line;

		rangeParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		if (parseResult)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool Parser::parseRoll(const ustring & line, std::vector<Roll> & result)
	{
		ustring lineCopy = line;

		rollParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		if (parseResult)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

