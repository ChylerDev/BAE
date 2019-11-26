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
		Factory class for constructing audio filters (Modifiers).
	***************************************************************************/
	class ModifierFactory
	{
	private:

		// Members              ///////////////////////

	public:

			/// Container used for coefficients of zeros of a filter in GenericFilter
		using ZeroContainer = GenericFilter::ZeroContainer;
			/// Container used for coefficients of poles of a filter in GenericFilter
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

		/*! ********************************************************************
		\brief
			Creates a delay filter.

		\param seconds
			The amount of time in seconds to delay for.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateDelay(Math_t seconds);

		/*! ********************************************************************
		\brief
			Creates an echo filter.

		\param delay_seconds
			The amount of time between echos in seconds.

		\param decay_ratio
			The decay factor of the echo. Value should be in range of [0,1), if
			it's >= 1 or < 0 it will be clamped to the range.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateEcho(Math_t delay_seconds, Math_t decay_ratio);

		/*! ********************************************************************
		\brief
			Creates an equalizer filter.

		\param band_count
			The number of bands in the equalizer. Defaults to 2.

		\param lower
			The lowest frequency of the equalizer. Defaults to 20Hz.

		\param upper
			The highest frequency of the equalizer. Defaults to 20kHz.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateEqualizer(uint32_t band_count = 2, Math_t lower = 20, Math_t upper = 20000);

		/*! ********************************************************************
		\brief
			Creates an envelope follower filter.

		\param lower
			The lower end of frequencies to follow. Defaults to 20Hz for normal
			human hearing range.

		\param upper
			The upper end of frequencies to follow. Defaults to 20kHz for normal
			human hearing range.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateEnvelopeFollower(Math_t lower = Math_t(20), Math_t upper = Math_t(20000));

		/*! ********************************************************************
		\brief
			Creates a gain filter.

		\param gain
			The gain to amplify the signal by. Value may be negative.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateGain(Math_t gain = DEFAULT_GAIN);

		/*! ********************************************************************
		\brief
			Creates a generic filter.

		\param zeros
			The list of coefficients for the zeros of the filter.

		\param poles
			The list of coefficients for the poles of the filter.

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateGenericFilter(ZeroContainer const & zeros,
												   PoleContainer const & poles);

		/*! ********************************************************************
		\brief
			Creates a low pass filter.

		\param cutoff
			The cutoff frequency of the filter.

		\param resonance
			The resonance of the filter at the cutoff frequency. Should be in
			the range of [0, 1/6], if the value is outside of this, it will be
			clamped to the range. Defaults to 0 for no resonance

		\return
			The generated modifier object.
		***********************************************************************/
		static ModifierBasePtr CreateLowPass(Math_t cutoff, Math_t resonance = 0);

		/*! ********************************************************************
		\brief
			Destructor. Deleted to ensure that an object can never be created.
		***********************************************************************/
		~ModifierFactory() = delete;

	}; // class ModifierFactory
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __MODIFIERFACTORY_HPP
