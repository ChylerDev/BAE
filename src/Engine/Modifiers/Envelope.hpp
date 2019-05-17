/*! ****************************************************************************
\file             Envelope.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __STUB_HPP
#define __STUB_HPP

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

  using pEnvelopeFollower_t = std::shared_ptr<class EnvelopeFollower>;

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class EnvelopeFollower : public Base
  {
  private:

    // Members              ///////////////////////

    double m_AU, m_BU;
    double m_AD, m_BD;
    StereoData_t m_X1, m_Y1;

  public:

    // Con-/De- structors   ///////////////////////

    EnvelopeFollower(float fd, float fu);
    virtual ~EnvelopeFollower();

    // Operators            ///////////////////////

    virtual StereoData_t FilterSample(StereoData_t const & x);

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

  private:

    // Functions                  ///////////////////////

  }; // class EnvelopeFollower

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __STUB_HPP
