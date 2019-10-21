/*! ****************************************************************************
\file             Sound.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Block.hpp"
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
    m_NodeGraph(), m_Gain(gain)
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

  Block_t & Sound::GetBlock(Node_t const & node)
  {
    return node->block;
  }

  Block_t const & Sound::GetBlock(Node_t const & node) const
  {
    return node->block;
  }

  Sound & Sound::SetOutputGain(Math_t gain)
  {
    m_Gain = gain;
    return *this;
  }

  Sound & Sound::AddBlock(
    Block_t const & block,
    uint32_t pos,
    bool targets_output
  )
  {
    m_NodeGraph[pos].push_back(
      std::make_shared<Node>(
        aStereoData_t(new StereoData_t[8192]),
        block, std::deque<Node_t>(), targets_output
      )
    );

    return *this;
  }

  Sound & Sound::SetTarget(Block_t const & block, uint32_t pos_b, Block_t const & target, uint32_t pos_t)
  {
    Node_t source;
    Node_t dest;

    for(auto & node : m_NodeGraph[pos_b])
    {
      if(node->block == block)
      {
        source = node;
        break;
      }
    }
    for(auto & node : m_NodeGraph[pos_t])
    {
      if(node->block == target)
      {
        dest = node;
        break;
      }
    }

    SetTarget(source, dest);

    return *this;
  }

  Sound & Sound::SetTarget(Node_t const & source, Node_t const & dest)
  {
    source->outputs.push_back(dest);

    return *this;
  }

  void Sound::SendBlock(StereoData_t * output, uint64_t size)
  {
    static aStereoData_t buffer(new StereoData_t[size]);
    static uint64_t i;

    for(auto & nodes : m_NodeGraph)
    {
      for(auto & node : nodes.second)
      {
        std::fill(buffer.get(), buffer.get()+size, StereoData_t(SampleType_t(0), SampleType_t(0)));

        node->block->SendBlock(buffer.get(), node->input.get(), size);

        if(node->final_output)
        {
          for(i = 0; i < size; ++i)
          {
             Left(output[i]) += SampleType_t(Left(buffer[i]) * m_Gain);
            Right(output[i]) += SampleType_t(Right(buffer[i]) * m_Gain);
          }
        }

        std::fill(node->input.get(), node->input.get()+size, StereoData_t(SampleType_t(0), SampleType_t(0)));

        for(auto & o : node->outputs)
        {
          for(i = 0; i < size; ++i)
          {
            Left((o->input)[i]) +=
              Left(buffer[i]);
            Right(o->input[i]) += Right(buffer[i]);
          }
        }
      }
    }
  }

} // namespace Core
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
