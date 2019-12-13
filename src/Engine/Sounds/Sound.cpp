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

#include "../Core/Driver.hpp"
#include "../Modifiers/Gain.hpp"
#include "../Modifiers/ModifierFactory.hpp"
#include "../Tools/MethodTable.hpp"
#include "SoundFactory.hpp"
#include "Combinator.hpp"
#include "Sound.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Sound
{
//////////////////////////////// Sound Functions ///////////////////////////////

	Sound::Sound(Math_t input_gain, Math_t output_gain) :
		m_Graph(),
		m_InputGain(Modifier::ModifierFactory::CreateGain(input_gain)),
		m_OutputGain(Modifier::ModifierFactory::CreateGain(output_gain)),
		m_Input(), m_Output(),
		m_Driver(), m_ID(static_cast<uint64_t>(-1)), m_IsPaused(false)
	{
		m_Graph.push_back(
			CreateEdge(
				std::deque<Edge::E_BlockPtr>(1, Edge::E_BlockPtr(
					new Edge::E_Block(SoundFactory::CreateBlock(m_InputGain))
				)),
				Combinator(Combinator::Addition),
				std::deque<Edge::E_BlockPtr>()
			)
		);
		m_Graph.push_back(
			CreateEdge(
				std::deque<Edge::E_BlockPtr>(),
				Combinator(Combinator::Addition),
				std::deque<Edge::E_BlockPtr>(1, Edge::E_BlockPtr(
					new Edge::E_Block(SoundFactory::CreateBlock(m_OutputGain))
				))
			)
		);
	}

	Sound::Sound(Sound const & other) :
		m_Graph(other.m_Graph),
		m_InputGain(other.m_InputGain), m_OutputGain(other.m_OutputGain),
		m_Input(), m_Output(),
		m_Driver(), m_ID(static_cast<uint64_t>(-1)), m_IsPaused(other.m_IsPaused)
	{
		MakeUnique();
	}

	Sound::Sound(Sound && other) noexcept :
		m_Graph(std::move(other.m_Graph)),
		m_InputGain(std::move(other.m_InputGain)), m_OutputGain(std::move(other.m_OutputGain)),
		m_Input(std::move(other.m_Input)), m_Output(std::move(other.m_Output)),
		m_Driver(), m_ID(static_cast<uint64_t>(-1)), m_IsPaused(std::move(other.m_IsPaused))
	{
	}

	Sound & Sound::operator=(Sound const & rhs)
	{
		m_Graph    = rhs.m_Graph;
		m_Input    = rhs.m_Input;
		m_Output   = rhs.m_Output;
		m_IsPaused = rhs.m_IsPaused;

		MakeUnique();

		return *this;
	}

	Sound & Sound::operator=(Sound && rhs) noexcept
	{
		m_Graph      = std::move(rhs.m_Graph);
		m_InputGain  = std::move(rhs.m_InputGain);
		m_OutputGain = std::move(rhs.m_OutputGain);
		m_Input      = std::move(rhs.m_Input);
		m_Output     = std::move(rhs.m_Output);
		m_IsPaused   = std::move(rhs.m_IsPaused);

		return *this;
	}

	Sound::Graph & Sound::GetGraph()
	{
		return m_Graph;
	}

	Sound::Graph const & Sound::GetGraph() const
	{
		return m_Graph;
	}

	void Sound::SetInputGain(Math_t gain)
	{
		m_InputGain->CallMethod("SetGain", gain);
	}

	void Sound::SetOutputGain(Math_t gain)
	{
		m_OutputGain->CallMethod("SetGain", gain);
	}

	void Sound::Pause()
	{
		m_IsPaused = true;
	}

	void Sound::Unpause()
	{
		m_IsPaused = false;
	}

	void Sound::PrimeInput(StereoData in)
	{
		m_Graph.front()->PrimeInput(in);
	}

	void Sound::Process()
	{
		if(m_IsPaused)
		{
			m_Output = StereoData();
			return;
		}

		for(auto & e : m_Graph)
		{
			e->Process();
		}

		std::any_cast<BlockPtr>(m_Graph.back()->outputs.back()->block)->Process();
		m_Output = std::any_cast<BlockPtr>(m_Graph.back()->outputs.back()->block)->LastOutput();
	}

	StereoData Sound::LastOutput()
	{
		return std::any_cast<BlockPtr>(m_Graph.back()->outputs.back()->block)->LastOutput();
	}

	Sound::EdgePtr Sound::CreateEdge(std::deque<Edge::E_BlockPtr> const & in,
									 Combinator const & comb,
									 std::deque<Edge::E_BlockPtr> const & out)
	{
		return std::make_shared<Sound::Edge>(in, comb, out);
	}

	Sound::Edge::E_BlockPtr Sound::CreateE_Block(SoundPtr const & s)
	{
		return Edge::E_BlockPtr(new Edge::E_Block(s));
	}

	Sound::Edge::E_BlockPtr Sound::CreateE_Block(BlockPtr const & b)
	{
		return Edge::E_BlockPtr(new Edge::E_Block(b));
	}

	void Sound::Register(SoundPtr const & self, Core::DriverPtr const & driver)
	{
		if(self->m_ID != static_cast<uint64_t>(-1)) Unregister(self);

		self->m_Driver = driver;
		self->m_ID = self->m_Driver->AddSound(self);
	}

	void Sound::Unregister(SoundPtr const & self)
	{
		if(self->m_ID == static_cast<uint64_t>(-1)) return;

		self->m_Driver->RemoveSound(self->m_ID);
		self->m_ID = static_cast<uint64_t>(-1);
	}

//////////////////////////////// Edge Functions ////////////////////////////////

	Sound::Edge::Edge(std::deque<E_BlockPtr> const & in, Combinator const & comb, std::deque<E_BlockPtr> const & out):
		inputs(in), combinator(comb), outputs(out)
	{
	}

	void Sound::Edge::Process()
	{
		Track_t samples;

		for(auto & in : inputs)
		{
			if(in->is_sound)
			{
				std::any_cast<SoundPtr>(in->block)->Process();
				samples.push_back(std::any_cast<SoundPtr>(in->block)->LastOutput());
			}
			else
			{
				std::any_cast<BlockPtr>(in->block)->Process();
				samples.push_back(std::any_cast<BlockPtr>(in->block)->LastOutput());
			}
		}

		StereoData output = combinator.Process(samples.begin(), samples.end());

		for(auto & out : outputs)
		{
			out->is_sound ? std::any_cast<SoundPtr>(out->block)->PrimeInput(output)
			              : std::any_cast<BlockPtr>(out->block)->PrimeInput(output);
		}
	}

	void Sound::Edge::PrimeInput(StereoData in)
	{
		for(auto & eb : inputs)
		{
			eb->is_sound ? std::any_cast<SoundPtr>(eb->block)->PrimeInput(in)
			             : std::any_cast<BlockPtr>(eb->block)->PrimeInput(in);
		}
	}

////////////////////////////// E_Block Functions ///////////////////////////////

	Sound::Edge::E_Block::E_Block(SoundPtr const & s):
		block(s), is_sound(true)
	{
	}

	Sound::Edge::E_Block::E_Block(BlockPtr const & b):
		block(b), is_sound(false)
	{
	}
} // namespace Sound
} // namespace OCAE

// Private Functions                      //////////////////////////////////////

namespace OCAE
{
namespace Sound
{
	void Sound::MakeUnique()
	{
			// Make all edges in the graph unique
		for(auto & edge : m_Graph)
		{
			for(auto & in : edge->inputs)
			{
				if(in.use_count() <= 1)
				{
					continue;
				}

				if(in->is_sound)
				{
					in->block = std::make_shared<Sound>(*std::any_cast<SoundPtr>(in->block));
				}
				else
				{
					in->block = std::make_shared<Block>(*std::any_cast<BlockPtr>(in->block));
				}
			}

			for(auto & out : edge->outputs)
			{
				if(out.use_count() <= 1)
				{
					continue;
				}

				if(out->is_sound)
				{
					out->block = std::make_shared<Sound>(*std::any_cast<SoundPtr>(out->block));
				}
				else
				{
					out->block = std::make_shared<Block>(*std::any_cast<BlockPtr>(out->block));
				}
			}
		}

		m_InputGain = std::any_cast<BlockPtr>(m_Graph.front()->inputs.front())->GetModifier();
		m_OutputGain = std::any_cast<BlockPtr>(m_Graph.back()->outputs.back())->GetModifier();
	}
} // namespace Sound
} // namespace OCAE
