/*! ****************************************************************************
\file             Envelope.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __ENVELOPE_HPP
#define __ENVELOPE_HPP

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
	class EnvelopeFollower : public ModifierBase
	{
	private:

		// Members              ///////////////////////

		Math_t m_AU, m_BU;
		Math_t m_AD, m_BD;
		StereoData m_X1, m_Y1;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~EnvelopeFollower();

		// Operators            ///////////////////////

		virtual StereoData FilterSample(StereoData const & x);
		virtual void FilterBlock(StereoData * input, StereoData * output, uint64_t size);

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

	private:

		// Functions                  ///////////////////////

		EnvelopeFollower(Math_t fd, Math_t fu);

	}; // class EnvelopeFollower
	TYPEDEF_SHARED(EnvelopeFollower);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __ENVELOPE_HPP
