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

namespace OCAE
{
namespace Tools
{
	static std::vector<std::string> s_Options;
} // namespace Tools
} // namespace OCAE

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
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
		return s_Options[size_t(i)];
	}
} // namespace Tools
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
