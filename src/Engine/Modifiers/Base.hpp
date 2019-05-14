/*! ****************************************************************************
\file             Base.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __MODIFIERS_BASE_HPP
#define __MODIFIERS_BASE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Base
  {
  private:

    // Members              ///////////////////////

    bool is_base;

  public:

    // Con-/De- structors   ///////////////////////

    Base(bool b) : is_base(b) {};
    virtual ~Base() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Takes input sample and filters it, returning the result.

    \param input
      The input sample.

    \return
      The filtered sample.
    ***************************************************************************/
    virtual StereoData_t FilterSample(StereoData_t const & input) { return input; };

    bool IsBase() { return is_base; };

  private:

    // Functions                  ///////////////////////

  }; // class Base

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __MODIFIERS_BASE_HPP
