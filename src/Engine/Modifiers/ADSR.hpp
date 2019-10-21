/*! ****************************************************************************
\file             ADSR.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __ADSR_HPP
#define __ADSR_HPP

// Include Files                ////////////////////////////////////////////////

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
	class ADSR : public ModifierBase
	{
	private:

		enum state : int8_t
		{
			attack,
			decay,
			sustain,
			release,
			invalid = -1,
		};

		// Members              ///////////////////////

		FixedPoint::FixedPoint<7,24> m_Attack;
		FixedPoint::FixedPoint<7,24> m_Decay;
		FixedPoint::FixedPoint<7,24> m_Sustain;
		FixedPoint::FixedPoint<7,24> m_Release;

		state m_State;

		FixedPoint::FixedPoint<7,24> m_Gain;

		MethodTable_t m_Table;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~ADSR() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		void Release(void);

		virtual StereoData_t FilterSample(StereoData_t const & input);
		virtual void FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size);

		virtual MethodTable_t const & GetMethodTable() const;

	private:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Simple linear ADSR envelope filter.

		\param attack
			Time to increase gain from 0 to 1 in samples.

		\param decay
			Time to decrease gain from 0 to sustain in samples.

		\param sustain
			The gain level of the sustain phase.

		\param release
			Time to decrease from sustain to 0 in samples.
		***********************************************************************/
		ADSR(uint64_t attack, uint64_t decay, Math_t sustain, uint64_t release);
	}; // class ADSR
	TYPEDEF_SHARED(ADSR);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __ADSR_HPP
