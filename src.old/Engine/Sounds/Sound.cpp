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
		for(auto & b : m_Graph[from])
		{
			if(b.get() == to.get())
				return;
		}

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

		StereoData out;

		for(auto & b : m_ProcessOrder)
		{
			out = b->Process();

			for(auto & t : m_Graph[b])
			{
				t->PrimeInput(out);
			}
		}

		return out;
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

			// Generate the order of processing through the graph
		for(auto & l : m_Graph)
		{
				// Don't worry about output gain, it'll be taken care of at the end of the function
			if(l.first == m_OutputGain)
			{
				continue;
			}
				// This could add duplicates, so we'll need to check for that later
			m_ProcessOrder.push_back(l.first);
			PrepareGraph(l.second, m_ProcessOrder);
		}

			// Remove duplicates
		for(uint64_t i = 0; i < m_ProcessOrder.size(); ++i)
		{
			for(uint64_t j = i+1; j < m_ProcessOrder.size(); ++j)
			{
				if(m_ProcessOrder[i] == m_ProcessOrder[j])
				{
					m_ProcessOrder.erase(m_ProcessOrder.begin()+int64_t(j));
					--j;
				}
			}
		}

			// Push the output gain block to the back as it will always be the last block processed
		m_ProcessOrder.push_back(m_OutputGain);
	}

	void Sound::PrepareGraph(BlockList const & list, BlockList & out)
	{
		for(auto & b : list)
		{
			bool shouldProcess = true;
				// If the current block is the output block, don't insert it, it will be inserted after the recursive call stack is complete
			if(b == m_OutputGain)
			{
				shouldProcess = false;
			}

				// If the item is already in the output list, do not insert it. Prevents getting stuck parsing a cycle
			for(auto & i : out)
			{
				if(i == b)
				{
					shouldProcess = false;
				}
			}

			if (shouldProcess)
			{
				out.push_back(b);
				PrepareGraph(m_Graph[b], out);
			}
		}
	}
} // namespace Sound
} // namespace OCAE
