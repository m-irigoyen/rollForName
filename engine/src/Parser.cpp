#include <rollForName/Parser.h>
#include <rollForName/Logger.h>
#include <rollForName/StringHelpers.h>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#define ERRORTAG_PARSER_L L"Parser"
#define ERRORTAG_PARSER "Parser"

// Using directives
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

	// Boost::fusion adapting structures
	// Range
BOOST_FUSION_ADAPT_STRUCT(
	rfn::Range,
	(int, min)
	(int, max)
	)

	// TableEntry
BOOST_FUSION_ADAPT_STRUCT(
	rfn::TableEntry,
	(rfn::Range, range)
	(rfn::ustring, name)
	(rfn::ustring, description)
)

// Roll
BOOST_FUSION_ADAPT_STRUCT(
	rfn::Roll,
	(unsigned int, nbDices)
	(unsigned int, nbFaces)
	(std::vector<int>, modifiers)
)

// Table
BOOST_FUSION_ADAPT_STRUCT(
	rfn::Table,
	(rfn::ustring, name)
	(rfn::ustring, requiredTables)
	(rfn::ustring, roll)
	(rfn::entryVector, entries)
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
			start %= qi::lexeme[qi::lit("\"") >> *(qi::char_ - "\"") >> qi::lit("\"")];
		}

		qi::rule<Iterator, ustring(), Skipper> start;
	};

	// Parser that reads an affectation operator
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct tableNameParser : public qi::grammar<Iterator, ustring(), Skipper> // return type is Table
	{
	public:
		tableNameParser() : tableNameParser::base_type(start)
		{
			start = (*(qi::char_ - "\"") >> quotedText)[qi::_val = qi::_2];
		}

		quotedTextParser<Iterator, Skipper> quotedText;
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
				>> qi::lit("_")
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

	// Parser that reads a dice roll, and returns it in ustring form
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct rollStringParser : public qi::grammar<Iterator, ustring(), Skipper>
	{
	public:
		rollStringParser() : rollStringParser::base_type(start)
		{
			start %= qi::no_case[
				qi::char_("[")
				>> *(qi::char_ - "]")
				>> qi::char_("]")
			];
		}
		qi::rule<Iterator, ustring(), Skipper> start;
	};

	// Parser that reads a Table Entry
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct tableEntryParser : public qi::grammar<Iterator, TableEntry(), Skipper>
	{
	public:
		tableEntryParser() : tableEntryParser::base_type(start)
		{
			start %= (qi::lit("*") | qi::lit("-"))
				>> range
				>> quotedText
				>> qi::lit(":")
				>> quotedText;
		}

		rangeParser<Iterator, Skipper> range;
		quotedTextParser<Iterator, Skipper> quotedText;

		qi::rule<Iterator, TableEntry(), Skipper> start;
	};

	// Parser that reads a Table
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct tableParser : public qi::grammar<Iterator, Table(), Skipper>
	{
	public:
		tableParser() : tableParser::base_type(start)
		{
			start %= tableName
				>> ((qi::no_case[qi::lit("required")] >> quotedText) | qi::attr("") )
				>> rollString
				>> qi::lit("{")
				>> +(tableEntry)
				>> qi::lit("}");
		}

		tableNameParser<Iterator, Skipper>  tableName;
		quotedTextParser<Iterator, Skipper> quotedText;
		rollStringParser<Iterator, Skipper> rollString;
		tableEntryParser<Iterator, Skipper> tableEntry;

		qi::rule<Iterator, Table(), Skipper> start;
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

	bool rfn::Parser::parseTableEntry(const ustring & line, TableEntry & t)
	{
		ustring lineCopy = line;

		tableEntryParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, t);

		if (parseResult)
		{
			t.range.makeValid();
			return true;
		}
		else
		{
			Logger::errlogs(L"Failed to parse line \n"
				+ line
				+ L". The following wasn't parsed : "
				+ ustring(lineCopy, it - lineCopy.begin(), ustring::npos)
				, ERRORTAG_PARSER_L
				, L"parseTableEntry");
		}
	}

	bool Parser::parseTable(const ustring & line, Table & t)
	{
		ustring lineCopy = line;

		tableParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, t);

		if (parseResult)
		{
			makeValidId(t.name);
			// Making valid ranges
			for (TableEntry te : t.entries)
			{
				te.range.makeValid();
			}

			return true;
		}
		else
		{
			Logger::errlogs(L"Failed to parse line \n"
				+ line + L". The following wasn't parsed : "
				+ ustring(lineCopy, it - lineCopy.begin(), ustring::npos)
				, ERRORTAG_PARSER_L
				, L"parseTable");
		}
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
			result.makeValid();
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

