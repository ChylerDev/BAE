/*! ****************************************************************************
\file             MethodTable.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __METHODTABLE_HPP
#define __METHODTABLE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class MethodTable
  {
  private:

    // Members              ///////////////////////

    static MethodTable_t const s_Table;

  public:

    // Con-/De- structors   ///////////////////////

    virtual ~MethodTable() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    virtual MethodTable_t const & GetMethodTable() const;

  private:

    // Functions                  ///////////////////////

  }; // class MethodTable

} // namespace Tools
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __METHODTABLE_HPP
