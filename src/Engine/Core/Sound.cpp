/*! ****************************************************************************
\file             Sound.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <Trace/Trace.hpp>

#include "Sound.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

  Sound::Sound(float gain) :
    m_NodeGraph(), m_Output(std::make_shared<StereoData_t>()), m_Gain(gain)
  {
  }

  Sound::Graph_t & Sound::GetGraph()
  {
    return m_NodeGraph;
  }

  Sound::Graph_t const & Sound::GetGraph() const
  {
    return m_NodeGraph;
  }

  void Sound::SetOutputGain(float gain)
  {
    m_Gain = gain;
  }

  void Sound::AddNode(
    std::shared_ptr<Node> const & node,
    uint32_t pos,
    bool targets_output
  )
  {
    m_NodeGraph[pos].push_back(node);

    if(targets_output)
    {
      node->AddFinalOutput(m_Output);
    }
  }

  StereoData_t Sound::GetSample()
  {
    *m_Output = std::make_tuple(0.f,0.f);

    for(auto & nodes : m_NodeGraph)
    {
      for(auto & node : nodes.second)
      {
        node->SendSample();
      }
    }

    std::get<0>(*m_Output) *= m_Gain;
    std::get<1>(*m_Output) *= m_Gain;

    return *m_Output;
  }

} // namespace Core
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
