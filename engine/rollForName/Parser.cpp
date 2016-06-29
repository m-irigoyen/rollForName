#include "Parser.h"
#include "Logger.h"

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

	// Parser that reads an affectation operator
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
}

