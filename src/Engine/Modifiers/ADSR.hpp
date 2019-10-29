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

namespace OCAE
{
namespace Modifier
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class ADSR : public ModifierBase
	{
	private:

		enum State : int8_t
		{
			attack,
			decay,
			sustain,
			release,
			invalid = -1,
		};

		// Members              ///////////////////////

		Math_t m_Attack;
		Math_t m_Decay;
		Math_t m_Sustain;
		Math_t m_Release;

		State m_State;

		Math_t m_Gain;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~ADSR() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		void Release(void);

		virtual StereoData FilterSample(StereoData const & input);

		friend class ModifierFactory;

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

		/// Alias for a std::shared_ptr instantiated with the ADSR class
	TYPEDEF_SHARED(ADSR);
} // namespace Modifier
} // namespace OCAE

// Public FunctOCAE      ////////////////////////////////////////////////

#endif // __ADSR_HPP
