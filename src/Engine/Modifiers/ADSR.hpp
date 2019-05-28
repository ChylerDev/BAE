/*! ****************************************************************************
\file             ADSR.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __ADSR_HPP
#define __ADSR_HPP

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
  class ADSR : public Base
  {
  private:

    enum state : int8_t
    {
      attack,
      decay,
      sustain,
      release,
      invalid = -1,
    };

    // Members              ///////////////////////

    Math::FixedPoint<7,24> m_Attack;
    Math::FixedPoint<7,24> m_Decay;
    Math::FixedPoint<7,24> m_Sustain;
    Math::FixedPoint<7,24> m_Release;

    state m_State;

    Math::FixedPoint<7,24> m_Gain;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Simple linear ADSR envelope filter.

    \param attack
      Time to increase gain from 0 to 1 in samples.

    \param decay
      Time to decrease gain from 0 to sustain in samples.

    \param sustain
      The gain level of the sustain phase.

    \param release
      Time to decrease from sustain to 0 in samples.
    ***************************************************************************/
    ADSR(uint64_t attack, uint64_t decay, Math_t sustain, uint64_t release);
    virtual ~ADSR() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    void Release(void);

    virtual StereoData_t FilterSample(StereoData_t const & input);

  private:

    // Functions                  ///////////////////////

  }; // class ADSR

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __ADSR_HPP
