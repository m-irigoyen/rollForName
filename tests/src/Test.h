#pragma once


namespace rfn
{
	//! Every test result is in percent
	class Test
	{
	public :
		static float testAll(bool displayFailed = false);
		static float testRoll(bool displayFailed = false);
		static float testRange(bool displayFailed = false);
		static float testTableEntryParse(bool displayFailed = false);
		static float testTableParse(bool displayFailed = false);
		static float testInstructionParse(bool displayFailed = false);
		static float testGeneratorParse(bool displayFailed = false);
	};
}