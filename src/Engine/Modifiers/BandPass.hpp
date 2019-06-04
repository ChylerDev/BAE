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

    Math_t m_CentralFrequency;
    Math_t m_Quality;
    Math_t m_A0, m_B1, m_B2;
    StereoData_t m_X1, m_X2, m_Y1, m_Y2;

    MethodTable_t m_Table;

  public:

    // Con-/De- structors   ///////////////////////

    BandPass(Math_t f, Math_t Q = 1);
    virtual ~BandPass();

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    void SetFrequency(Math_t f);

    void SetQuality(Math_t Q);

    // Functions            ///////////////////////

    virtual StereoData_t FilterSample(StereoData_t const & x);
    virtual void FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size);

    virtual MethodTable_t const & GetMethodTable() const;

  private:

    // Functions                  ///////////////////////

    void Reset(void);

  }; // class BandPass

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __BAND_PASS_HPP
