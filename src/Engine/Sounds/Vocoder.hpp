/*! ****************************************************************************
\file             Vocoder.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __VOCODER_HPP
#define __VOCODER_HPP

// Include Files                ////////////////////////////////////////////////

#include <vector>
#include <tuple>

#include "../Engine.hpp"

#include "../Core/Node.hpp"
#include "../Core/Sound.hpp"
#include "../Generators/Base.hpp"
#include "../Generators/Sine.hpp"
#include "../Generators/Square.hpp"
#include "../Modifiers/Envelope.hpp"
#include "../Modifiers/BandPass.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Sounds
{

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Vocoder
  {
  private:

    using Carrier_t = Generator::Square;

    // Members              ///////////////////////

    std::vector<float> m_CentralFrequencies;

    uint32_t m_BandCount;

    float m_Mu;

    std::shared_ptr<Core::Sound> m_Sound;

  public:

    // Con-/De- structors   ///////////////////////

    Vocoder(std::shared_ptr<Core::Node> const & base_input, int N=1);

    // Operators            ///////////////////////

    operator std::shared_ptr<Core::Sound>();

    // Accossors/Mutators   ///////////////////////

      // p is in cents
    void SetOffset(float p);

    // Functions            ///////////////////////

  private:

    // Functions                  ///////////////////////

    std::vector<Modifier::pBase_t> BPSetup();
    std::vector<Modifier::pBase_t> EnvSetup();
    std::vector<std::shared_ptr<Generator::Base>> OscSetup();

  }; // class Vocoder

} // namespace Sounds
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __VOCODER_HPP
