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
			Edge(
				std::deque<BlockPtr>(1, SoundFactory::CreateBlock(m_InputGain)),
				Combinator(Combinator::Addition),
				std::deque<BlockPtr>()
			)
		);
		m_Graph.push_back(
			Edge(
				std::deque<BlockPtr>(),
				Combinator(Combinator::Addition),
				std::deque<BlockPtr>(1, SoundFactory::CreateBlock(m_OutputGain))
			)
		);
	}

	Sound & Sound::AddEdge(Edge const & e, bool takes_input = false, bool sends_output = false)
	{
		if(takes_input)
		{
			auto & out = std::get<2>(m_Graph.front());
			out.insert(out.end(), std::get<0>(e).begin(), std::get<0>(e).end());
		}
		else if(sends_output)
		{
			auto & in = std::get<0>(m_Graph.back());
			in.insert(in.end(), std::get<2>(e).begin(), std::get<2>(e).end());
		}
		else
		{
			m_Graph.insert(m_Graph.end()-1, e);
		}

		return *this;
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

	StereoData Sound::Process(StereoData input)
	{
		std::get<0>(m_Graph.front()).front()->PrimeInput(input);

		for(auto & e : m_Graph)
		{
			std::deque<StereoData> outputs;

			for(auto & in : std::get<0>(e))
			{
				in->Process();
				outputs.push_back(in->LastOutput());
			}

			input = std::get<1>(e).Process(outputs.begin(), outputs.end());

			for(auto & out : std::get<2>(e))
			{
				out->PrimeInput(input);
			}
		}

		return std::get<2>(m_Graph.back()).back()->LastOutput();
	}
} // namespace Sound
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
