/*! ****************************************************************************
\file             MethodTable.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "MethodTable.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{
	MethodTable::MethodTable() :
		m_Table()
	{
	}

	void MethodTable::operator()(std::string const & name, void * val)
	{
		m_Table.at(name)(val);
	}
} // namespace Tools
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
