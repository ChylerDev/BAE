/*! ****************************************************************************
\file             Delay.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __DELAY_HPP
#define __DELAY_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include <deque>

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
  class Delay : public Base
  {
  private:

    // Members              ///////////////////////

    std::deque<StereoData_t> m_Delay;

    MethodTable_t m_Table;

  public:

    // Con-/De- structors   ///////////////////////

    Delay(uint64_t samples);
    virtual ~Delay() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    void SetDelay(uint64_t samples);

    virtual StereoData_t FilterSample(StereoData_t const & input);

    virtual MethodTable_t const & GetMethodTable() const;

  private:

    // Functions                  ///////////////////////

  }; // class Delay

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __DELAY_HPP
