/*! ****************************************************************************
\file             Sound.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Node.hpp"
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

  Sound::Sound(Math_t gain) :
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

  Sound & Sound::SetOutputGain(Math_t gain)
  {
    m_Gain = gain;
    return *this;
  }

  Sound & Sound::AddNode(
    Node_t const & node,
    uint32_t pos,
    bool targets_output
  )
  {
    m_NodeGraph[pos].push_back(node);

    if(targets_output)
    {
      node->AddOutput(m_Output);
    }

    return *this;
  }

  StereoData_t Sound::GetSample()
  {
    *m_Output = StereoData_t(0,0);

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

  void Sound::SendBlock(StereoData_t * buffer, uint64_t size)
  {
    static uint64_t i;

    for(i = 0; i < size; ++i)
    {
      *m_Output = StereoData_t(0,0);

      for(auto & nodes : m_NodeGraph)
      {
        for(auto & node : nodes.second)
        {
          node->SendSample();
        }
      }

      std::get<0>(buffer[i]) += std::get<0>(*m_Output) * m_Gain;
      std::get<1>(buffer[i]) += std::get<1>(*m_Output) * m_Gain;
    }
  }

} // namespace Core
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
