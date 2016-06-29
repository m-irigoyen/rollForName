#pragma once

#include "typedefs.h"
#include <iostream>

namespace rfn
{

	//! ustringhe Logger is custom log system that can write messages to different outputs
	class Logger
	{
	private:
		typedef std::string string;

	public :
		//! Writes a message to the standard console output
		static void logs(ustring message);
		static void logs(ustring message, ustring author);
		static void logs(ustring message, ustring author, ustring section);
		//! Writes a message to the error console output
		static void errlogs(ustring message);
		static void errlogs(ustring message, ustring author);
		static void errlogs(ustring message, ustring author, ustring section);

		//! Writes a message to the standard console output
		static void logs(string message);
		static void logs(string message, string author);
		static void logs(string message, string author, string section);
		//! Writes a message to the error console output
		static void errlogs(string message);
		static void errlogs(string message, string author);
		static void errlogs(string message, string author, string section);

		static void logNewLine();
		static void errlogNewLine();
	};


}
