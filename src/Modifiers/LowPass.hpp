/*! ****************************************************************************
\file             LowPass.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __LOW_PASS_HPP
#define __LOW_PASS_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace Modifiers
{

  #pragma region
  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class LowPass
  {
  private:

    // Members              ///////////////////////

    double m_Cutoff;
    double m_Resonance;
    double m_Coefficients[4];
    StereoData_t m_Outputs[3];
    bool m_IsDirty;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Constructor.

    \param cutoff
      The cutoff frequency in Hz.

    \param resonance
      The resonance angle of the filter, value can be in range [0,1/6]. No
      safety checks are performed.
    ***************************************************************************/
    LowPass(float cutoff, float resonance);

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    /*! ************************************************************************
    \brief
      Sets the cutoff frequency of the filter.

    \param cutoff
      The cutoff frequency.
    ***************************************************************************/
    void SetCutoff(float cutoff);

    /*! ************************************************************************
    \brief
      Sets the resonance angle of the filter.

    \param resonance
      The resonance angle, in range [0,1/6]. No safety checks are performed.
    ***************************************************************************/
    void SetResonance(float resonance);

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Takes input sample and filters it, returning the result.

    \param input
      The input sample.

    \return
      The filtered sample.
    ***************************************************************************/
    StereoData_t FilterSample(StereoData_t input);

  private:

    // Functions                  ///////////////////////

    void Reset();

  }; // class LowPass
  #pragma endregion

} // namespace Modifiers

// Public Functions             ////////////////////////////////////////////////

#endif // __LOW_PASS_HPP
