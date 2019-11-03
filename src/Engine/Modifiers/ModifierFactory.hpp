/*! ****************************************************************************
\file             ModifierFactory.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __MODIFIERFACTORY_HPP
#define __MODIFIERFACTORY_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "GenericFilter.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	class ModifierBase;
	TYPEDEF_SHARED(ModifierBase);
}
}

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class ModifierFactory
	{
	private:

		// Members              ///////////////////////

	public:

		using ZeroContainer = GenericFilter::ZeroContainer;
		using PoleContainer = GenericFilter::PoleContainer;

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Creates an empty modifier which will simply forward any input it
			recieves to it's output.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateBase();

		/*! ********************************************************************
		\brief
			Creates a modifier for an ADSR envelope.

		\param attack
			The length of the attack phase in seconds.

		\param decay
			The length of the decay phase in seconds.

		\param sustain
			The sustain level in dB.

		\param release
			The length of the decay phase in seconds.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateADSR(Math_t attack, Math_t decay, Math_t sustain, Math_t release);

		/*! ********************************************************************
		\brief
			Creates a bandpass filter.

		\param lower
			The lower frequency of the band.

		\param upper
			The upper frequency of the band.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateBandPass(Math_t lower, Math_t upper);
		static ModifierBasePtr CreateDelay(Math_t seconds);
		static ModifierBasePtr CreateEcho(Math_t delay_seconds, Math_t decay_ratio);
		static ModifierBasePtr CreateEnvelopeFollower(Math_t fd, Math_t fu);
		static ModifierBasePtr CreateGain(Math_t gain = DEFAULT_GAIN);
		static ModifierBasePtr CreateGenericFilter(ZeroContainer const & zeros,
												   PoleContainer const & poles);
		static ModifierBasePtr CreateLowPass(Math_t cutoff, Math_t resonance);

		~ModifierFactory() = delete;

	private:

		// Functions                  ///////////////////////

	}; // class ModifierFactory
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __MODIFIERFACTORY_HPP
