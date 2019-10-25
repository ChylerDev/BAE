/*! ****************************************************************************
\file             Equalizer.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __EQUALIZER_HPP
#define __EQUALIZER_HPP

// Include Files                ////////////////////////////////////////////////

#include <tuple>
#include <vector>

#include "../Engine.hpp"

#include "ModifierBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class Equalizer
	{
	private:

		// Members              ///////////////////////

	public:

		// Con-/De- structors   ///////////////////////

		Equalizer(int band_count, Math_t lower, Math_t upper, Math_t Q);

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		void SetGain(int band, Math_t gain);

		friend class ModifierFactory;

	private:

		// Functions                  ///////////////////////

	}; // class Equalizer
	TYPEDEF_SHARED(Equalizer);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __EQUALIZER_HPP
