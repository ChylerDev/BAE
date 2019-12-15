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
	Sound::Sound(Math_t input_gain, Math_t output_gain) :
		m_Graph(), m_ProcessOrder(),
		m_InputGain(SoundFactory::CreateBlock(Modifier::ModifierFactory::CreateGain(input_gain))),
		m_OutputGain(SoundFactory::CreateBlock(Modifier::ModifierFactory::CreateGain(output_gain))),
		m_Driver(), m_ID(static_cast<uint64_t>(-1)), m_IsPaused(false)
	{
		m_Graph[m_InputGain];
	}

	Sound::Sound(Sound const & other) :
		m_Graph(other.m_Graph), m_ProcessOrder(other.m_ProcessOrder),
		m_InputGain(other.m_InputGain), m_OutputGain(other.m_OutputGain),
		m_Driver(), m_ID(static_cast<uint64_t>(-1)), m_IsPaused(other.m_IsPaused)
	{
		MakeUnique();
	}

	Sound::Sound(Sound && other) noexcept :
		m_Graph(std::move(other.m_Graph)), m_ProcessOrder(std::move(other.m_ProcessOrder)),
		m_InputGain(std::move(other.m_InputGain)), m_OutputGain(std::move(other.m_OutputGain)),
		m_Driver(), m_ID(static_cast<uint64_t>(-1)), m_IsPaused(std::move(other.m_IsPaused))
	{
	}

	Sound & Sound::operator=(Sound const & rhs)
	{
		m_Graph        = rhs.m_Graph;
		m_ProcessOrder = rhs.m_ProcessOrder;
		m_IsPaused     = rhs.m_IsPaused;
		Math_t gain;
		rhs.m_InputGain->GetModifier->CallMethod("GetGain", METHOD_RET(gain));
		m_InputGain->GetModifier->CallMethod(
			"SetGain", METHOD_PARAM(gain)
		);
		rhs.m_OutputGain->GetModifier->CallMethod("GetGain", METHOD_RET(gain));
		m_OutputGain->GetModifier->CallMethod(
			"SetGain", METHOD_PARAM(gain)
		);

		MakeUnique();

		return *this;
	}

	Sound & Sound::operator=(Sound && rhs) noexcept
	{
		m_Graph        = std::move(rhs.m_Graph);
		m_ProcessOrder = std::move(rhs.m_ProcessOrder);
		m_InputGain    = std::move(rhs.m_InputGain);
		m_OutputGain   = std::move(rhs.m_OutputGain);
		m_IsPaused     = std::move(rhs.m_IsPaused);

		return *this;
	}

	BlockPtr const & Sound::GetInputGain() const
	{
		return m_InputGain;
	}

	BlockPtr const & Sound::GetOutputGain() const
	{
		return m_OutputGain;
	}

	void Sound::SetInputGain(Math_t gain)
	{
		m_InputGain->GetModifier()->CallMethod("SetGain", gain);
	}

	void Sound::SetOutputGain(Math_t gain)
	{
		m_OutputGain->GetModifier()->CallMethod("SetGain", gain);
	}

	void Sound::Pause()
	{
		m_IsPaused = true;
	}

	void Sound::Unpause()
	{
		m_IsPaused = false;
	}

	void Sound::AddConnection(BlockPtr const & from, BlockPtr const & to)
	{
		m_Graph[from].push_back(to);

		ProcessOrder();
	}

	void Sound::RemoveConnection(BlockPtr const & from, BlockPtr const & to)
	{
		for(auto it = m_Graph[from].begin(); it < m_Graph[from].end(); ++it)
		{
			if(*it == to)
			{
				m_Graph[from].erase(it);
				break;
			}
		}

		ProcessOrder();
	}

	StereoData Sound::Process(StereoData input)
	{
		if(m_IsPaused)
		{
			return StereoData();
		}

		m_InputGain->PrimeInput(input);

		for(auto & b : m_ProcessOrder)
		{
			StereoData out;

			b->Process();
			out = b->LastOutput();

			for(auto & t : m_Graph[b])
			{
				t->PrimeInput(out);
			}
		}

		return m_OutputGain->LastOutput();
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
} // namespace Sound
} // namespace OCAE

// Private Functions                      //////////////////////////////////////

namespace OCAE
{
namespace Sound
{
	void Sound::ProcessOrder()
	{
		m_ProcessOrder.clear();
		m_ProcessOrder.push_front(m_InputGain);

		for(auto & l : m_Graph)
		{
			PrepareGraph(l.second, m_ProcessOrder);
		}

		std::deque<BlockPtr> scratch;
		for(auto i = 0; i < m_ProcessOrder.size(); ++i)
		{
			bool should_remove = false;
			for(auto & b : scratch)
			{
				if(m_ProcessOrder[i] == b)
				{
					should_remove = true;
				}
			}

			if(should_remove)
			{
				m_ProcessOrder.erase(m_ProcessOrder.begin() + i);
				--i;
			}
		}
	}

	void Sound::PrepareGraph(std::deque<BlockPtr> const & list, std::deque<BlockPtr> & out)
	{
		for(auto & b : list)
		{
			out.push_back(b);
			PrepareGraph(m_Graph[b], out);
		}
	}

	void Sound::MakeUnique()
	{
		Graph unique_graph;

		for(auto & a : m_Graph)
		{
			auto new_a = std::make_shared<Block>(*(a.first));

			for(auto & b : a.second)
			{
				unique_graph[new_a].push_back(
					std::make_shared<Block>(*b)
				);
			}
		}

		m_Graph.clear();
		m_Graph = std::move(unique_graph);
	}
} // namespace Sound
} // namespace OCAE
