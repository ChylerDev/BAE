/*! ****************************************************************************
\file             Delay.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __DELAY_HPP
#define __DELAY_HPP

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
	class Delay : public ModifierBase
	{
	private:

		// Members              ///////////////////////

		std::deque<StereoData> m_Delay;



	public:

		// Con-/De- structors   ///////////////////////

		virtual ~Delay() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		void SetDelay(uint64_t samples);

		virtual StereoData FilterSample(StereoData const & input);
		virtual void FilterBlock(StereoData * input, StereoData * output, uint64_t size);



		friend class ModifierFactory;

	private:

		// Functions                  ///////////////////////

		Delay(uint64_t samples);

	}; // class Delay
	TYPEDEF_SHARED(Delay);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __DELAY_HPP
