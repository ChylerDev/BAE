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

#define NULL_ID (static_cast<uint64_t>(-1))

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
		m_Driver(), m_ID(NULL_ID), m_IsPaused(false)
	{
		m_Graph[m_InputGain];
	}

	Sound::Sound(Sound && other) noexcept :
		m_Graph(std::move(other.m_Graph)), m_ProcessOrder(std::move(other.m_ProcessOrder)),
		m_InputGain(std::move(other.m_InputGain)), m_OutputGain(std::move(other.m_OutputGain)),
		m_Driver(), m_ID(NULL_ID), m_IsPaused(std::move(other.m_IsPaused))
	{
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

	BlockPtr const & Sound::GetInputBlock() const
	{
		return m_InputGain;
	}

	BlockPtr const & Sound::GetOutputBlock() const
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
			b->Process();
			StereoData out = b->LastOutput();

			for(auto & t : m_Graph[b])
			{
				t->PrimeInput(out);
			}
		}

		return m_OutputGain->LastOutput();
	}

	void Sound::Concat(SoundPtr && other)
	{
			// Calculate the new gain
		Math_t this_g, other_g;
		m_OutputGain->GetModifier()->CallMethod("GetGain", METHOD_RET(this_g));
		other->m_InputGain->GetModifier()->CallMethod("GetGain", METHOD_RET(other_g));
		this_g *= other_g;

			// Set the new gain
		m_OutputGain->GetModifier()->CallMethod("SetGain", METHOD_PARAM(this_g));

			// Remove the other's input gain from the process order
		other->m_ProcessOrder.pop_front();

			// Move the other's input's connections to this' current output list in the graph
		m_Graph[m_OutputGain] = std::move(other->m_Graph[other->m_InputGain]);

			// Remove the other's input from the graph
		other->m_Graph.erase(other->m_InputGain);

			// Move the edges from other's graph to this' graph
		for(auto & p : other->m_Graph)
		{
			m_Graph[p.first] = std::move(p.second);
		}

			// Set the new output gain block
		m_OutputGain = other->m_OutputGain;

			// Get the new process order
		ProcessOrder();

			// Unregister other
		Unregister(other);
	}

	void Sound::Register(SoundPtr const & self, Core::DriverPtr const & driver)
	{
		if(self->m_ID != NULL_ID) Unregister(self);

		self->m_Driver = driver;
		self->m_ID = self->m_Driver->AddSound(self);
	}

	void Sound::Unregister(SoundPtr const & self)
	{
		if(self->m_ID == NULL_ID) return;

		self->m_Driver->RemoveSound(self->m_ID);
		self->m_ID = NULL_ID;
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
			// Clear any current order
		m_ProcessOrder.clear();
			// Push the input gain block to the front as it will always be the first block processed
		m_ProcessOrder.push_front(m_InputGain);

			// Generate the graph of the 
		for(auto & l : m_Graph)
		{
			PrepareGraph(l.second, m_ProcessOrder);
		}

			// Push the output gain block to the back as it will always be the last block processed
		m_ProcessOrder.push_back(m_OutputGain);
	}

	void Sound::PrepareGraph(std::deque<BlockPtr> const & list, std::deque<BlockPtr> & out)
	{
		for(auto & b : list)
		{
				// If the current block is the output block, don't insert it, it will be inserted after the recursive call stack is complete
			if(b == m_OutputGain)
			{
				continue;
			}

				// If the item is already in the output list, do not insert it. Prevents getting stuck parsing a cycle
			for(auto & i : out)
			{
				if(i == b)
				{
					continue;
				}
			}
			out.push_back(b);
			PrepareGraph(m_Graph[b], out);
		}
	}
} // namespace Sound
} // namespace OCAE
