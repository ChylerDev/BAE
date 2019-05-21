/*! ****************************************************************************
\file             Sine.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __SQUARE_HPP
#define __SQUARE_HPP

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
    Generates square wave data at the given frequency.
  *****************************************************************************/
  class Square : public Base
  {
  private:

    // Members              ///////////////////////

    Math_t m_Ind;
    Math_t m_Inv;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Creates an object that outputs a simple square wave without using
      inefficient functions like std::sin.

    \param freq
      The frequency for the square wav to output at.
    ***************************************************************************/
    Square(Math_t freq);

    /*! ************************************************************************
    \brief
      Default destructor.
    ***************************************************************************/
    virtual ~Square() = default;

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
    virtual void SetFrequency(Math_t freq);

  private:

    // Functions                  ///////////////////////

  }; // class Square

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SQUARE_HPP
