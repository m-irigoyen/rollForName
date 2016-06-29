#pragma once

#include <fstream>


namespace rfn
{
	typedef std::wifstream wifstream;

	class FileOpener
	{
	public:
		//! Returns true if the file was correctly open
		static bool open(std::string path, wifstream& stream);
	};
}