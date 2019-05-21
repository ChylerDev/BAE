/*! ****************************************************************************
\file             Gain.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __GAIN_HPP
#define __GAIN_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "Base.hpp"

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
  class Gain : public Base
  {
  private:

    // Members              ///////////////////////

    Math_t m_Gain;

  public:

    // Con-/De- structors   ///////////////////////

    Gain(Math_t gain = DEFAULT_GAIN);
    virtual ~Gain() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    void SetGain(Math_t gain);
    Math_t GetGain() const;

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Takes input sample and filters it, returning the result.

    \param input
      The input sample.

    \return
      The filtered sample.
    ***************************************************************************/
    virtual StereoData_t FilterSample(StereoData_t const & input);

  private:

    // Functions                  ///////////////////////

  }; // class Gain

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __GAIN_HPP
