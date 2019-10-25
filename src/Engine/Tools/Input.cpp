/*! ****************************************************************************
\file             Input.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Input.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{
	static std::vector<std::string> s_Options;
} // namespace Tools
} // namespace AudioEngine

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{
	void InitOptions(int argc, char * argv[])
	{
		for(int i = 0; i < argc; ++i)
		{
			s_Options.push_back(argv[i]);
		}
	}

	std::string const & GetOption(int i)
	{
		return s_Options[i];
	}
} // namespace Tools
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
