/*! ****************************************************************************
\file             Sine.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __SINE_HPP
#define __SINE_HPP

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
    Generates sine data at the given frequency.
  *****************************************************************************/
  class Sine : public Base
  {
  private:

    // Members              ///////////////////////

    double irate;
    double y1, y2;
    double beta;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Creates an object that outputs a simple sine wave without using inefficient
      functions like std::sin.

    \param freq
      The frequency for the sine-wav to output at.
    ***************************************************************************/
    Sine(float freq = 440.f);

    /*! ************************************************************************
    \brief
      Default destructor.
    ***************************************************************************/
    virtual ~Sine() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Sends a single sample to Core::Driver for output to the OS.

    \return
      The stereo sample data.
    ***************************************************************************/
    virtual StereoData_t SendSample(void);

    /*! ************************************************************************
    \brief
      Sets the frequency to a new value.

    \param freq
      The new frequency.
    ***************************************************************************/
    virtual void SetFrequency(float freq);

  private:

    // Functions                  ///////////////////////

    /*! ************************************************************************
    \brief
      Sets all the coefficients for calculating samples.
    ***************************************************************************/
    void Reset(void);

  }; // class Sine

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SINE_HPP
