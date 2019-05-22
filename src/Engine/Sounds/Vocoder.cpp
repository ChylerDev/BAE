/*! ****************************************************************************
\file             Vocoder.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <Trace/Trace.hpp>

#include "../Core/Sound.hpp"
#include "../Core/Node.hpp"

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

  Vocoder::Vocoder(pNode_t const & base_input, int N, Math_t gain) : Base(gain, false),
    m_CentralFrequencies(), m_BandCount(N), m_Mu(1.f)
  {
    auto l_BP = BPSetup();
    auto l_Env = EnvSetup();
    auto l_Osc = OscSetup();

    m_Sound->AddNode(base_input, 0);

    for(uint32_t i = 0; i < m_BandCount; ++i)
    {
      std::shared_ptr<Core::Node> bp(
        Node_t::Create(GenBase_t::Create<GenBase_t>(true),l_BP[i])
      );
      std::shared_ptr<Core::Node> mod(
        Node_t::Create(l_Osc[i], l_Env[i])
      );

      base_input->AddTarget(*bp);
      bp->AddTarget(*mod);

      m_Sound->AddNode(bp, 1);
      m_Sound->AddNode(mod, 2, true);
    }
  }

  void Vocoder::SetOffset(int32_t p)
  {
    m_Mu = std::powf(2, p/1200.f);

    uint32_t counter = 0;

    auto & graph = m_Sound->GetGraph();
    for(auto & node : graph[2])
    {
      dynamic_cast<Carrier_t *>(node->GetGenerator().get())->
        SetFrequency(m_CentralFrequencies[++counter]*m_Mu);
    }
  }

} // namespace Sounds
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////

namespace AudioEngine
{
namespace Sounds
{

  std::vector<pModBase_t> Vocoder::BPSetup()
  {
    std::vector<double> l_Freq(m_BandCount+1, 0.0);

    l_Freq[0] = 80;
    l_Freq[m_BandCount] = 4'000;

    double delta = ((std::log(l_Freq[m_BandCount])/std::log(10)) - (std::log(l_Freq[0])/std::log(10))) / m_BandCount;

    for(uint32_t i = 1; i < m_BandCount; ++i)
    {
      l_Freq[i] = l_Freq[0] * std::pow(10, i*delta);
    }

    std::vector<pModBase_t> l_BP;

    double l_Q = std::sqrt(l_Freq[1]*l_Freq[0]) / (l_Freq[1] - l_Freq[0]);

    for(uint32_t i = 0; i < m_BandCount; ++i)
    {
      m_CentralFrequencies.push_back(std::sqrt(float(l_Freq[i] * l_Freq[i+1])));
      l_BP.push_back(
        ModBase_t::Create<BandPass_t>(m_CentralFrequencies.back(), float(l_Q))
      );
    }

    return l_BP;
  }

  std::vector<pModBase_t> Vocoder::EnvSetup()
  {
    std::vector<pModBase_t> l_Env;

    for(uint32_t i = 0; i < m_BandCount; ++i)
    {
      l_Env.push_back(ModBase_t::Create<Envelope_t>(20.f, 20'000.f));
    }

    return l_Env;
  }

  std::vector<pGenBase_t> Vocoder::OscSetup()
  {
    std::vector<pGenBase_t> l_Osc;

    for(uint32_t i = 0; i < m_BandCount; ++i)
    {
      l_Osc.push_back(
        GenBase_t::Create<Carrier_t>(m_CentralFrequencies[i] * m_Mu)
      );
    }

    return l_Osc;
  }

} // namespace Sounds
} // namespace AudioEngine
