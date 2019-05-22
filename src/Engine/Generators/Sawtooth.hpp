/*! ****************************************************************************
\file             Sawtooth.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __SAWTOOTH_HPP
#define __SAWTOOTH_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "Base.hpp"

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
  class Sawtooth : public Base
  {
  private:

    // Members              ///////////////////////

    Math_t m_Irate;

    Math_t m_Inc;

  public:

    // Con-/De- structors   ///////////////////////

    Sawtooth(Math_t freq);
    virtual ~Sawtooth() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    virtual void SetFrequency(Math_t freq);

    virtual StereoData_t SendSample(void);

  private:

    // Functions                  ///////////////////////

  }; // class Sawtooth

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SAWTOOTH_HPP
