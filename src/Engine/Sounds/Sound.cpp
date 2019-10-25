/*! ****************************************************************************
\file             Sound.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Block.hpp"
#include "Sound.hpp"

#include "../Modifiers/ModifierFactory.hpp"
#include "SoundFactory.hpp"
#include "Combinator.hpp"
#include "Sound.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Sound
{
	Sound::Sound(Math_t input_gain, Math_t output_gain) :
		m_Graph(),
		m_InputGain(Modifier::ModifierFactory::CreateGain(input_gain)),
		m_OutputGain(Modifier::ModifierFactory::CreateGain(output_gain))
	{
		m_Graph.push_back(
			CreateEdge(
				std::deque<Edge::E_Block>(1, Edge::E_Block(SoundFactory::CreateBlock(m_InputGain))),
				Combinator(Combinator::Addition),
				std::deque<Edge::E_Block>()
			)
		);
		m_Graph.push_back(
			CreateEdge(
				std::deque<Edge::E_Block>(),
				Combinator(Combinator::Addition),
				std::deque<Edge::E_Block>(1, Edge::E_Block(SoundFactory::CreateBlock(m_OutputGain)))
			)
		);
	}

	Sound::Graph & Sound::GetGraph()
	{
		return m_Graph;
	}

	Sound::Graph const & Sound::GetGraph() const
	{
		return m_Graph;
	}

	Sound & Sound::SetInputGain(Math_t gain)
	{
		(*m_InputGain)("SetGain", &gain);
		return *this;
	}

	Sound & Sound::SetOutputGain(Math_t gain)
	{
		(*m_OutputGain)("SetGain", &gain);
		return *this;
	}

	void Sound::PrimeInput(StereoData in)
	{
		m_Graph.front()->PrimeInput(in);
	}

	void Sound::Process()
	{
		for(auto & e : m_Graph)
		{
			e->Process();
		}
	}

	StereoData Sound::LastOutput()
	{
		return m_Graph.back()->outputs.back().block->LastOutput();
	}

	Sound::EdgePtr Sound::CreateEdge(std::deque<Edge::E_Block> const & in, Combinator const & comb, std::deque<Edge::E_Block> const & out)
	{
		return std::make_shared<Sound::Edge>(in, comb, out);
	}

	Sound::Edge::E_Block Sound::CreateE_Block(SoundPtr const & s)
	{
		return Edge::E_Block(s);
	}

	Sound::Edge::E_Block Sound::CreateE_Block(BlockPtr const & b)
	{
		return Edge::E_Block(b);
	}
} // namespace Sound
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
