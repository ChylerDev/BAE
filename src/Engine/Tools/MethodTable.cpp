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

namespace OCAE
{
namespace Tools
{
	MethodTable::MethodTable() :
		m_Table()
	{
	}

	MethodTable::MethodTable(std::vector<std::tuple<std::string, Void_fn>> const & list) :
		m_Table()
	{
		SetMethods(list);
	}

	void MethodTable::SetMethods(std::vector<std::tuple<std::string, Void_fn>> const & list)
	{
		for(auto & m : list)
		{
			m_Table[std::get<0>(m)] = std::get<1>(m);
		}
	}
} // namespace Tools
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
