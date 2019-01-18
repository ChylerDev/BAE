/*! ****************************************************************************
\file   Sine.hpp
\author Chyler Morrison
\email  contact@chyler.info
*******************************************************************************/

#ifndef __SINE_HPP
#define __SINE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace Generator
{

/*! ****************************************************************************
\brief
  Generates sine data at the given frequency.
*******************************************************************************/
class Sine
{
private:

  // Members              ///////////////////////

  double irate;
  double y1, y2;
  double beta;

public:

  // Con-/De- structors   ///////////////////////

  Sine(float freq = 440.f);
  ~Sine() = default;

  // Operators            ///////////////////////

  // Accossors/Mutators   ///////////////////////

  // Functions            ///////////////////////

  StereoData_t GetData(void);

  void SetFrequency(float freq);

private:

  // Functions                  ///////////////////////

  void Reset(void);

}; // class Sine

} // namespace Generator

// Public Functions             ////////////////////////////////////////////////

#endif // __SINE_HPP
