/*! ****************************************************************************
\file             Echo.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __ECHO_HPP
#define __ECHO_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include <deque>

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
	class Echo : public ModifierBase
	{
	private:

		// Members              ///////////////////////

		std::deque<StereoData> m_Echo;
		Math_t m_Ratio;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~Echo() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		virtual StereoData FilterSample(StereoData const & sample);

		friend class ModifierFactory;

	private:

		// Functions                  ///////////////////////

		Echo(uint64_t sample_delay, Math_t decay_ratio);

	}; // class Echo
	TYPEDEF_SHARED(Echo);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __ECHO_HPP
