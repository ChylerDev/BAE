/*! ****************************************************************************
\file             Sound.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __SOUND_HPP
#define __SOUND_HPP

// Include Files                ////////////////////////////////////////////////

#include <deque>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "../Engine.hpp"

#include "Combinator.hpp"
#include "Block.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Sound
{
	class Sound;
	TYPEDEF_SHARED(Sound);
}
}

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Sound
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class Sound
	{

	public:

		struct Edge
		{
			struct E_Block
			{
				union
				{
					SoundPtr sound;
					BlockPtr block;
				};
				bool const is_sound;

				E_Block(SoundPtr const & s):
					sound(s), is_sound(true)
				{};
				E_Block(BlockPtr const & b):
					block(b), is_sound(false)
				{};
				E_Block(E_Block const & o) : is_sound(o.is_sound)
				{
					if(is_sound)
					{
						sound = o.sound;
					}
					else
					{
						block = o.block;
					}
				}
				~E_Block()
				{
					is_sound ? sound.~SoundPtr() : block.~BlockPtr();
				};
			};
			TYPEDEF_SHARED(E_Block);

			std::deque<E_BlockPtr> inputs;
			Combinator combinator;
			std::deque<E_BlockPtr> outputs;

			Edge(std::deque<E_BlockPtr> const & in, Combinator const & comb, std::deque<E_BlockPtr> const & out):
				inputs(in), combinator(comb), outputs(out)
			{};

			void Process()
			{
				Track_t samples;

				for(auto & in : inputs)
				{
					if(in->is_sound)
					{
						in->sound->Process();
						samples.push_back(in->sound->LastOutput());
					}
					else
					{
						in->block->Process();
						samples.push_back(in->block->LastOutput());
					}
				}

				StereoData output = combinator.Process(samples.begin(), samples.end());

				for(auto & out : outputs)
				{
					if(out->is_sound)
					{
						out->sound->PrimeInput(output);
					}
					else
					{
						out->block->PrimeInput(output);
					}
				}
			};

			void PrimeInput(StereoData in)
			{
				for(auto & eb : inputs)
				{
					eb->is_sound ? eb->sound->PrimeInput(in) : eb->block->PrimeInput(in);
				}
			};
		};
		TYPEDEF_SHARED(Edge);

		using Graph = std::deque< EdgePtr >;

	private:

		// Members              ///////////////////////

		Graph m_Graph;

		Modifier::ModifierBasePtr m_InputGain;
		Modifier::ModifierBasePtr m_OutputGain;

		StereoData m_Input;
		StereoData m_Output;

	public:
		// Con-/De- structors   ///////////////////////

		Sound(Math_t input_gain = Math_t(1.0), Math_t output_gain = DEFAULT_GAIN);

		~Sound() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		Graph & GetGraph();
		Graph const & GetGraph() const;

		Sound & SetInputGain(Math_t gain);
		Sound & SetOutputGain(Math_t gain);

		// Functions            ///////////////////////

		void PrimeInput(StereoData);
		void Process();
		StereoData LastOutput();

		static EdgePtr CreateEdge(std::deque<Edge::E_BlockPtr> const & in, Combinator const & comb, std::deque<Edge::E_BlockPtr> const & out);
		static Edge::E_BlockPtr CreateE_Block(SoundPtr const &);
		static Edge::E_BlockPtr CreateE_Block(BlockPtr const &);

	private:

		// Functions                  ///////////////////////

	}; // class Sound
} // namespace Sound
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUND_HPP
