/*! ****************************************************************************
\file             Triangle.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __TRIANGLE_HPP
#define __TRIANGLE_HPP

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
  class Triangle : public Base
  {
  private:

    // Members              ///////////////////////

    Math_t m_Irate;
    Math_t m_Inc;

    MethodTable_t m_Table;

  public:

    // Con-/De- structors   ///////////////////////

    Triangle(Math_t freq);
    virtual ~Triangle() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    void SetFrequency(Math_t freq);

    virtual StereoData_t SendSample(void);

    virtual MethodTable_t const & GetMethodTable() const;

  private:

    // Functions                  ///////////////////////

  }; // class Triangle

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __TRIANGLE_HPP
