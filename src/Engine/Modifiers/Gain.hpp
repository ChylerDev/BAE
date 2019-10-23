/*! ****************************************************************************
\file             Gain.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __GAIN_HPP
#define __GAIN_HPP

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
	class Gain : public ModifierBase
	{
	private:

		// Members              ///////////////////////

		Math_t m_Gain;

		MethodTable_t m_Table;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~Gain() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		void SetGain(Math_t gain);
		Math_t GetGain() const;

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Takes input sample and filters it, returning the result.

		\param input
			The input sample.

		\return
			The filtered sample.
		***********************************************************************/
		virtual StereoData FilterSample(StereoData const & input);
		virtual void FilterBlock(StereoData * input, StereoData * output, uint64_t size);

		virtual MethodTable_t const & GetMethodTable() const;

	private:

		// Functions                  ///////////////////////

		Gain(Math_t gain = DEFAULT_GAIN);

	}; // class Gain
	TYPEDEF_SHARED(Gain);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __GAIN_HPP
