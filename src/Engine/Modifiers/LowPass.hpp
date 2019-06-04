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
  class LowPass : public Base
  {
  private:

    // Members              ///////////////////////

    Math_t m_Cutoff;
    Math_t m_Resonance;
    Math_t m_Coefficients[4];
    StereoData_t m_Outputs[3];

    MethodTable_t m_Table;

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
    LowPass(Math_t cutoff, Math_t resonance);

    /*! ************************************************************************
    \brief
      Default destructor.
    ***************************************************************************/
    virtual ~LowPass() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    /*! ************************************************************************
    \brief
      Sets the cutoff frequency of the filter.

    \param cutoff
      The cutoff frequency.
    ***************************************************************************/
    void SetCutoff(Math_t cutoff);

    /*! ************************************************************************
    \brief
      Sets the resonance angle of the filter.

    \param resonance
      The resonance angle, in range [0,1/6]. No safety checks are performed.
    ***************************************************************************/
    void SetResonance(Math_t resonance);

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
    virtual void FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size);

    virtual MethodTable_t const & GetMethodTable() const;

  private:

    // Functions                  ///////////////////////

    void Reset();

  }; // class LowPass

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __LOW_PASS_HPP
