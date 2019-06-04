/*! ****************************************************************************
\file             MethodTable.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "MethodTable.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{

  MethodTable_t const MethodTable::s_Table;

} // namespace Tools
} // namespace AudioEngine

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{

  MethodTable_t const & MethodTable::GetMethodTable() const
  {
    return s_Table;
  }

} // namespace Tools
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
