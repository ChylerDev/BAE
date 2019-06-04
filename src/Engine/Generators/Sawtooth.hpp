/*! ****************************************************************************
\file             Sawtooth.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __SAWTOOTH_HPP
#define __SAWTOOTH_HPP

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
  *****************************************************************************/
  class Sawtooth : public Base
  {
  private:

    // Members              ///////////////////////

    Math_t m_Irate;
    Math_t m_Inc;

    MethodTable_t m_Table;

  public:

    // Con-/De- structors   ///////////////////////

    Sawtooth(Math_t freq);
    virtual ~Sawtooth() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    void SetFrequency(Math_t freq);

    virtual StereoData_t SendSample(void);
    virtual void SendBlock(StereoData_t * buffer, uint64_t size);

    virtual MethodTable_t const & GetMethodTable() const;

  private:

    // Functions                  ///////////////////////

  }; // class Sawtooth

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SAWTOOTH_HPP
