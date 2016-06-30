#pragma once

#include "TableManager.h"

namespace rfn
{
	class Generator
	{
		Generator();

		//TODO : console command arguments
		bool init();



	private:
		TableManager manager_;

		bool ready_;
	};
}