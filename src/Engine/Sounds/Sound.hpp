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

		//                       inputs                  combinator  outputs
		using Edge = std::tuple< std::deque< BlockPtr >, Combinator, std::deque< BlockPtr > >;
		using Graph = std::deque< Edge >;

	private:

		// Members              ///////////////////////

		Graph m_Graph;

		Modifier::ModifierBasePtr m_InputGain;
		Modifier::ModifierBasePtr m_OutputGain;

	public:
		// Con-/De- structors   ///////////////////////

		Sound(Math_t input_gain = Math_t(1.0), Math_t output_gain = DEFAULT_GAIN);

		~Sound() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		Sound & AddEdge(Edge const & e, bool takes_input = false, bool sends_output = false);

		Sound & SetInputGain(Math_t gain);
		Sound & SetOutputGain(Math_t gain);

		// Functions            ///////////////////////

		StereoData Process(StereoData input);
	private:

		// Functions                  ///////////////////////

	}; // class Sound
	TYPEDEF_SHARED(Sound);
} // namespace Sound
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUND_HPP
