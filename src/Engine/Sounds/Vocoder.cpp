/*! ****************************************************************************
\file             Vocoder.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include <Trace/Trace.hpp>

#include "../Core/Sound.hpp"
#include "../Core/Block.hpp"

#include "../Generators/Square.hpp"

#include "../Modifiers/BandPass.hpp"
#include "../Modifiers/Envelope.hpp"

#include "Vocoder.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Sounds
{

  Vocoder::Vocoder(Block_t const & base_input, int N, Math_t gain) : Base(gain, false),
    m_CentralFrequencies(), m_BandCount(N), m_Mu(1.f), m_Table()
  {
    m_Table["SetOffset"] = [this](void * p){ SetOffset(*reinterpret_cast<int32_t*>(p)); };

    auto l_BP = BPSetup();
    auto l_Env = EnvSetup();
    auto l_Osc = OscSetup();

    m_Sound->AddBlock(base_input, 0);

    for(uint32_t i = 0; i < m_BandCount; ++i)
    {
      std::shared_ptr<Core::Block> bp(
        Core::Block::Create(Generator::Base::Create<Generator::Base>(true),l_BP[i])
      );
      std::shared_ptr<Core::Block> mod(
        Core::Block::Create(l_Osc[i], l_Env[i])
      );

      m_Sound->AddBlock(bp, 1).AddBlock(mod, 2, true)
              .SetTarget(base_input, 0, bp, 1)
              .SetTarget(bp, 1, mod, 2);
    }
  }

  void Vocoder::SetOffset(int32_t p)
  {
    m_Mu = std::pow(2, float(p)/1200.f);

    uint32_t counter = 0;

    auto & graph = m_Sound->GetGraph();
    for(auto & node : graph[2])
    {
      static Math_t freq;
      freq = m_CentralFrequencies[++counter]*m_Mu;
      m_Sound->GetBlock(node)->GetGenerator()->GetMethodTable().at("SetFrequency")(&freq);
    }
  }

  MethodTable_t const & Vocoder::GetMethodTable() const
  {
    return m_Table;
  }

} // namespace Sounds
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////

namespace AudioEngine
{
namespace Sounds
{

  std::vector<ModBase_t> Vocoder::BPSetup()
  {
    std::vector<double> l_Freq(m_BandCount+1, 0.0);

    l_Freq[0] = 80;
    l_Freq[m_BandCount] = 4'000;

    double delta = ((std::log(l_Freq[m_BandCount])/std::log(10)) - (std::log(l_Freq[0])/std::log(10))) / m_BandCount;

    for(uint32_t i = 1; i < m_BandCount; ++i)
    {
      l_Freq[i] = l_Freq[0] * std::pow(10, i*delta);
    }

    std::vector<ModBase_t> l_BP;

    double l_Q = std::sqrt(l_Freq[1]*l_Freq[0]) / (l_Freq[1] - l_Freq[0]);

    for(uint32_t i = 0; i < m_BandCount; ++i)
    {
      m_CentralFrequencies.push_back(std::sqrt(float(l_Freq[i] * l_Freq[i+1])));
      l_BP.push_back(
        Modifier::Base::Create<Modifier::BandPass>(m_CentralFrequencies.back(), float(l_Q))
      );
    }

    return l_BP;
  }

  std::vector<ModBase_t> Vocoder::EnvSetup()
  {
    std::vector<ModBase_t> l_Env;

    for(uint32_t i = 0; i < m_BandCount; ++i)
    {
      l_Env.push_back(Modifier::Base::Create<Modifier::EnvelopeFollower>(20.f, 20'000.f));
    }

    return l_Env;
  }

  std::vector<GenBase_t> Vocoder::OscSetup()
  {
    std::vector<GenBase_t> l_Osc;

    for(uint32_t i = 0; i < m_BandCount; ++i)
    {
      l_Osc.push_back(
        Generator::Base::Create<Carrier_t>(m_CentralFrequencies[i] * m_Mu)
      );
    }

    return l_Osc;
  }

} // namespace Sounds
} // namespace AudioEngine
