/*! ****************************************************************************
\file             BandPass.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __BAND_PASS_HPP
#define __BAND_PASS_HPP

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
  class BandPass : public Base
  {
  private:

    // Members              ///////////////////////

    double m_CentralFrequency;
    double m_Quality;
    double m_A0, m_B1, m_B2;
    StereoData_t m_X1, m_X2, m_Y1, m_Y2;

  public:

    // Con-/De- structors   ///////////////////////

    BandPass(float f, float Q = 1);
    virtual ~BandPass();

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    void SetFrequency(float f);

    void SetQuality(float Q);

    // Functions            ///////////////////////

    virtual StereoData_t FilterSample(StereoData_t const & x);

  private:

    // Functions                  ///////////////////////

    void Reset(void);

  }; // class BandPass

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __BAND_PASS_HPP