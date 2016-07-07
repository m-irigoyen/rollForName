#include <rollForName/Parser.h>
#include <rollForName/Logger.h>
#include <rollForName/StringHelpers.h>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
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

// RollAndMod
BOOST_FUSION_ADAPT_STRUCT(
	rfn::RollAndMod,
	(std::vector<rfn::Roll>, rolls)
	(rfn::ustring, mod)
)

// Table
BOOST_FUSION_ADAPT_STRUCT(
	rfn::Table,
	(rfn::ustring, name)
	(rfn::ustringVector, requiredTables)
	(rfn::ustring, roll)
	(rfn::entryVector, entries)
)

// Instruction
BOOST_FUSION_ADAPT_STRUCT(
	rfn::Instruction,
	(bool, generator)
	(rfn::ustring, name)
)

//Action
BOOST_FUSION_ADAPT_STRUCT(
	rfn::Action,
	(rfn::ustring, variableName)
	(int, value)
)

// Generator
BOOST_FUSION_ADAPT_STRUCT(
	rfn::Generator,
	(rfn::ustring, name)
	(std::vector<rfn::ustring>, requiredGenerators)
	(std::vector<rfn::ustring>, requiredTables)
	(std::vector<rfn::Instruction>, instructions)
)

namespace rfn
{
	// Parser that reads a ustring between quotes
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

	// Parser that reads several quoted texts separated by commas
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct quotedTextVectorParser : public qi::grammar<Iterator, std::vector<ustring>(), Skipper> // return type is Table
	{
	public:
		quotedTextVectorParser() : quotedTextVectorParser::base_type(start)
		{
			start %= quotedText >> *(quotedText >> qi::lit(","));
		}

		quotedTextParser<Iterator, Skipper> quotedText;
		qi::rule<Iterator, std::vector<ustring>(), Skipper> start;
	};

	// Parser that extracts a file name, without extension, from a filepath (ustring form)
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct ustringFileNameParser : public qi::grammar<Iterator, ustring(), Skipper> // return type is Table
	{
	public:
		ustringFileNameParser() : ustringFileNameParser::base_type(start)
		{
			path = qi::lexeme[-((qi::lit("/") || qi::lit("\\")))
				>> *(qi::char_ - (qi::lit("/") || qi::lit("\\")))
				>> ((qi::lit("/") || qi::lit("\\")))
			];

			start %= *path >> *(qi::char_ - ".") >> qi::lit(".rfn");
		}

		qi::rule<Iterator, Skipper> path;
		qi::rule<Iterator, ustring(), Skipper> start;
	};

	// Parser that extracts a file name, without extension, from a filepath (std::string form)
	template <typename Iterator, typename Skipper = boost::spirit::standard::space_type>
	struct fileNameParser : public qi::grammar<Iterator, std::string(), Skipper> // return type is Table
	{
	public:
		fileNameParser() : fileNameParser::base_type(start)
		{
			path = qi::lexeme[-((qi::lit("/") || qi::lit("\\")))
				>> *(qi::char_ - (qi::lit("/") || qi::lit("\\")))
				>> ((qi::lit("/") || qi::lit("\\")))
			];

			start %= *path >> *(qi::char_ - ".") >> qi::lit(".rfn");
		}

		qi::rule<Iterator, Skipper> path;
		qi::rule<Iterator, std::string(), Skipper> start;
	};

	// Parser that reads a table name
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
			range %= qi::lit("<")
				>> qi::int_
				>> qi::lit("_")
				>> qi::int_
				>> qi::lit(">");

			nb = qi::lit("<")
				>> qi::int_[phoenix::bind(&Range::setNumber, qi::_val, qi::_1)]
				>> qi::lit(">");

			start %= nb | range;
		}

		qi::rule<Iterator, Range(), Skipper> nb;
		qi::rule<Iterator, Range(), Skipper> range;

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
				>> qi::char_("]")]
				>> -(qi::char_("+")
				>> qi::lexeme[
					qi::string("($")
					>> (*qi::char_ - (')'))
					>> qi::char_(")")
					]);
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
			start %= (qi::lit("*") | qi::lit("-") | qi::lit("!"))
				>> (range | qi::attr(Range()))
				>> quotedText
				>> (
					(qi::lit(":") >> quotedText)
					| qi::attr("")
					);
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
				>> ((qi::no_case[qi::lit("required")] >> -qi::lit(":") >> quotedTextVector)
					| qi::attr(ustringVector()) )
				>> (rollString || qi::attr(""))
				>> qi::lit("{")
				>> +(tableEntry)
				>> qi::lit("}");
		}

		tableNameParser<Iterator, Skipper>  tableName;
		quotedTextVectorParser<Iterator, Skipper> quotedTextVector;
		rollStringParser<Iterator, Skipper> rollString;
		tableEntryParser<Iterator, Skipper> tableEntry;

		qi::rule<Iterator, Table(), Skipper> start;
	};

	// Parser that reads an Instruction
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct instructionParser : public qi::grammar<Iterator, Instruction(), Skipper>
	{
	public:
		instructionParser() : instructionParser::base_type(start)
		{
			start %= (
				(qi::no_case[qi::lit("t") >> qi::lit(":")] >> qi::attr(false))
				| (qi::no_case[qi::lit("g") >> qi::lit(":")] >> qi::attr(true))
				| qi::attr(true)
				)
				>> quotedText;
		}

		quotedTextParser<Iterator, Skipper> quotedText;

		qi::rule<Iterator, Instruction(), Skipper> start;
	};

	// Parser that reads a Generator
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct generatorParser : public qi::grammar<Iterator, Generator(), Skipper>
	{
	public:
		generatorParser() : generatorParser::base_type(start)
		{
			start %= quotedText	// name
				>> ( (qi::no_case[qi::lit("required generators")]
						>> -qi::lit(":")
						>> quotedTextVector)
					| qi::attr(ustringVector()) )
				>> ( ((qi::no_case[qi::lit("required tables")]
						>> -qi::lit(":")
						>> *quotedTextVector))
					| qi::attr(ustringVector()) )
				>> qi::lit("{")
				>> +instruction
				>> qi::lit("}");
		}

		instructionParser<Iterator, Skipper> instruction;
		quotedTextParser<Iterator, Skipper> quotedText;
		quotedTextVectorParser<Iterator, Skipper> quotedTextVector;

		qi::rule<Iterator, Generator(), Skipper> start;
	};

	// Parser that reads a goto description, and returns the name
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct gotoDescriptionParser : public qi::grammar<Iterator, ustring(), Skipper>
	{
	public:
		gotoDescriptionParser() : gotoDescriptionParser::base_type(start)
		{
			start %= qi::lit("->") >> qi::lexeme[*qi::char_];
		}

		qi::rule<Iterator, ustring(), Skipper> start;
	};

	// Parser that reads an action and returns it
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct actionParser : public qi::grammar<Iterator, Action(), Skipper>
	{
	public:
		actionParser() : actionParser::base_type(start)
		{
			start %= qi::lit("(")
				>> qi::lexeme[qi::lit("$")
				>> *(qi::char_ - '=')]
				>> qi::lit("=")
				>> qi::int_
				>> qi::lit(")");
		}

		qi::rule<Iterator, Action(), Skipper> start;
	};

	// Parser that reads a lone variable and returns it
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct loneVariableParser : public qi::grammar<Iterator, ustring(), Skipper>
	{
	public:
		loneVariableParser() : loneVariableParser::base_type(start)
		{
			ignore = *(qi::char_ - '(');

			start %= ignore
				>> qi::lexeme[qi::lit("(")
				>> qi::lit("$")
				>> *(qi::char_ - ')')
				>> qi::lit(")")
				];
		}

		qi::rule<Iterator, Skipper> ignore;
		qi::rule<Iterator, ustring(), Skipper> start;
	};

	// Parser that reads a dice roll + modifier
	template <typename Iterator, typename Skipper = boost::spirit::standard_wide::space_type>
	struct rollAndModParser : public qi::grammar<Iterator, RollAndMod(), Skipper>
	{
	public:
		rollAndModParser() : rollAndModParser::base_type(start)
		{
			roll %= qi::int_
				>> qi::no_case[qi::lit("d")]
				>> qi::int_
				>> *(qi::int_
					>> (&qi::lit("+") | &qi::lit("-") | &qi::lit("]")));

			start %= qi::lit("[")
				>> *roll
				>> qi::lit("]")
				>>
				(
				(qi::lit("+") >> loneVariable)
				| qi::attr("")
					);
		}

		loneVariableParser<Iterator, Skipper> loneVariable;

		qi::rule<Iterator, Roll, Skipper> roll;
		qi::rule<Iterator, RollAndMod(), Skipper> start;
	};



		//----------
		// FUNCTIONS
		//----------

	bool rfn::Parser::findNextBracketEnd(ustring::iterator& begin, ustring::iterator end, ustring& skipped)
	{
		return qi::phrase_parse(begin
			, end
			, (*qi::char_ - '}') >> qi::char_('}')
			, boost::spirit::standard_wide::space
			, skipped);
	}

	bool Parser::isTableName(const ustring& line, ustring& name)
	{
		ustring lineCopy = line;

		quotedTextParser<ustring::iterator, boost::spirit::standard_wide::space_type> g;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, g
			, boost::spirit::standard_wide::space
			, name);

		return parseResult;
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
			return false;
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

		if (parseResult && t.isValid())
		{
			if (t.isValid())
			{
				return true;
			}
			else
			{
				Logger::errlogs(L"Parsed invalid table \n"
					+ line + L"."
					, ERRORTAG_PARSER_L
					, L"parseTable");
			}
		}
		else
		{
			Logger::errlogs(L"Failed to parse table \n"
				+ line + L". The following wasn't parsed : "
				+ ustring(lineCopy, it - lineCopy.begin(), ustring::npos)
				, ERRORTAG_PARSER_L
				, L"parseTable");
		}
		return false;
	}

	bool rfn::Parser::parseTable(ustring::iterator& begin, ustring::iterator end, Table & t)
	{
		tableParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		bool parseResult = qi::phrase_parse(begin, end
			, parser
			, boost::spirit::standard_wide::space
			, t);

		if (parseResult && t.isValid())
		{
			return true;
		}
		else
		{
			return false;
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
	bool Parser::parseRoll(const ustring & line, RollAndMod & result)
	{
		ustring lineCopy = line;

		rollAndModParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}


	bool Parser::parseNameFromFilepath(const std::string& line, std::string& result)
	{
		std::string lineCopy = line;

		fileNameParser<std::string::iterator, boost::spirit::standard::space_type> parser;
		std::string::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard::space
			, result);

		return parseResult;
	}

	bool Parser::parseNameFromFilepath(const ustring& line, ustring& result)
	{
		ustring lineCopy = line;

		ustringFileNameParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}

	bool rfn::Parser::parseLoneVariable(const ustring & line, ustring & result)
	{
		ustring lineCopy = line;

		loneVariableParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}

	bool rfn::Parser::parseRollString(const ustring & line, ustring & result)
	{
		ustring lineCopy = line;

		rollStringParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		bool parseResult = qi::phrase_parse(lineCopy.begin(), lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}

	bool rfn::Parser::parseGeneratorInstruction(const ustring & line, Instruction & result)
	{
		ustring lineCopy = line;

		instructionParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}
	bool rfn::Parser::parseGenerator(const ustring & line, Generator & result)
	{
		ustring lineCopy = line;

		generatorParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}
	bool rfn::Parser::parseGenerator(ustring::iterator& begin
		, ustring::iterator end
		, Generator& result)
	{
		generatorParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		bool parseResult = qi::phrase_parse(begin, end
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}
	bool rfn::Parser::parseGoto(const ustring & line, ustring & result)
	{
		ustring lineCopy = line;

		gotoDescriptionParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}

	bool rfn::Parser::parseAction(const ustring & line, Action & result)
	{
		ustring lineCopy = line;

		actionParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		ustring::iterator it = lineCopy.begin();
		bool parseResult = qi::phrase_parse(it, lineCopy.end()
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}

	bool rfn::Parser::parseLoneVariable(ustring::iterator begin, const ustring::iterator end, ustring & result)
	{
		loneVariableParser<ustring::iterator, boost::spirit::standard_wide::space_type> parser;
		bool parseResult = qi::phrase_parse(begin, end
			, parser
			, boost::spirit::standard_wide::space
			, result);

		return parseResult;
	}
}

