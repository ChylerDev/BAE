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

		std::deque<StereoData_t> m_Echo;
		Math_t m_Ratio;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~Echo() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		virtual StereoData_t FilterSample(StereoData_t const & sample);
		virtual void FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size);

	private:

		// Functions                  ///////////////////////

		Echo(uint64_t sample_delay, Math_t decay_ratio);

	}; // class Echo
	TYPEDEF_SHARED(Echo);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __ECHO_HPP
