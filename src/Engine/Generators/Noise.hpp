/*! ****************************************************************************
\file             Noise.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __NOISE_HPP
#define __NOISE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include <random>

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
  class Noise : public Base
  {
  private:

    // Members              ///////////////////////

    std::uniform_int_distribution<int16_t> m_Distribution;
    std::default_random_engine m_Engine;

  public:

    // Con-/De- structors   ///////////////////////

    Noise();
    virtual ~Noise() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    virtual StereoData_t SendSample(void);
    virtual void SendBlock(StereoData_t * buffer, uint64_t size);

  private:

    // Functions                  ///////////////////////

  }; // class Noise

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __NOISE_HPP
