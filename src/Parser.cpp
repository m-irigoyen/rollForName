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

	bool Parser::isTableName(ustring line, ustring& name)
	{
		quotedTextParser<ustring::iterator, boost::spirit::standard_wide::space_type> g;
		ustring::iterator it = line.begin();
		bool parseResult = qi::phrase_parse(it, line.end()
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

	bool Parser::parseTable(std::wifstream & stream, Table & t)
	{
		return false;
	}
}

