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

#include "Envelope.hpp"
#include "BandPass.hpp"
#include "../Generators/Base.hpp"
#include "../Generators/Sine.hpp"
#include "../Generators/Square.hpp"
#include "../Core/Node.hpp"
#include "../Core/Sound.hpp"

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
  class Vocoder
  {
  private:

    using Channel_t = std::tuple<BandPass, EnvelopeFollower, Generator::Square>;

    // Members              ///////////////////////

    std::vector<Channel_t> m_Channels;
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

    std::vector<BandPass> BPSetup();
    std::vector<EnvelopeFollower> EnvSetup();
    std::vector<Generator::Square> OscSetup();

  }; // class Vocoder

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __VOCODER_HPP
