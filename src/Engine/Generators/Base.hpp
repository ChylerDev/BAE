/*! ****************************************************************************
\file             Base.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __GENERATORS__BASE_HPP
#define __GENERATORS__BASE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Base
  {
  private:

    // Members              ///////////////////////

  public:

    // Con-/De- structors   ///////////////////////

    Base() = default;
    virtual ~Base() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    virtual StereoData_t SendSample(void) { return StereoData_t(0.f, 0.f); };
    virtual void SetFrequency(float freq) { UNREFERENCED_PARAMETER(freq); };

  private:

    // Functions                  ///////////////////////

  }; // class Base

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERATORS_BASE_HPP
