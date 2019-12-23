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

// Public Objects                         //////////////////////////////////////

OCAE::Tools::MethodTable::MethodTable_t OCAE::Tools::MethodTable::s_Table;

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Tools
{
	MethodTable::MethodTable()
	{
	}

	MethodTable::MethodTable(MethodList_t const & list)
	{
		RegisterMethods(list);
	}

	void MethodTable::RegisterMethod(std::string const & name, Void_fn const & obj)
	{
		s_Table[this][name] = obj;
	}

	void MethodTable::RegisterMethods(MethodList_t const & list)
	{
		for(auto & m : list)
		{
			s_Table[this][std::get<0>(m)] = std::get<1>(m);
		}
	}
} // namespace Tools
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
