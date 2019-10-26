/*! ****************************************************************************
\file             BandPass.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __BAND_PASS_HPP
#define __BAND_PASS_HPP

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
	class BandPass : public ModifierBase
	{
	private:

		// Members              ///////////////////////

		Math_t m_CentralFrequency;
		Math_t m_Quality;
		Math_t m_A0, m_B1, m_B2;
		StereoData m_X1, m_X2, m_Y1, m_Y2;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~BandPass();

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		void SetFrequency(Math_t f);

		void SetQuality(Math_t Q);

		// Functions            ///////////////////////

		virtual StereoData FilterSample(StereoData const & x);

		friend class ModifierFactory;

	private:

		// Functions                  ///////////////////////

		BandPass(Math_t f, Math_t Q = 1);

		void Reset(void);

	}; // class BandPass
	TYPEDEF_SHARED(BandPass);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __BAND_PASS_HPP
