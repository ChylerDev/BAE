/*! ****************************************************************************
\file             ModifierFactory.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "ModifierFactory.hpp"

#include "ADSR.hpp"
#include "BandPass.hpp"
#include "Delay.hpp"
#include "Echo.hpp"
#include "Equalizer.hpp"
#include "Envelope.hpp"
#include "Gain.hpp"
#include "GenericFilter.hpp"
#include "LowPass.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	ModifierBasePtr ModifierFactory::CreateBase()
	{
		return ModifierBasePtr(new ModifierBase());
	}

	ModifierBasePtr ModifierFactory::CreateADSR(Math_t attack, Math_t decay, Math_t sustain, Math_t release)
	{
		return ModifierBasePtr(new ADSR(
			uint64_t(attack*SAMPLE_RATE),
			uint64_t(decay*SAMPLE_RATE),
			Math_t(DB_TO_LINEAR(sustain)),
			uint64_t(release*SAMPLE_RATE)
		));
	}

	ModifierBasePtr ModifierFactory::CreateBandPass(Math_t lower, Math_t upper)
	{
		Math_t const fc(std::sqrt(lower * upper));
		return ModifierBasePtr(new BandPass(fc, fc/(upper-lower)));
	}

	ModifierBasePtr ModifierFactory::CreateDelay(Math_t seconds)
	{
		return ModifierBasePtr(new Delay(uint64_t(seconds * SAMPLE_RATE)));
	}

	ModifierBasePtr ModifierFactory::CreateEcho(Math_t delay_seconds, Math_t decay_ratio)
	{
			// Clamp value to within the accepted range
		decay_ratio = std::clamp(decay_ratio, Math_t(0), Math_t(1));
		return ModifierBasePtr(new Echo(uint64_t(delay_seconds * SAMPLE_RATE), decay_ratio));
	}

	ModifierBasePtr ModifierFactory::CreateEqualizer(uint32_t band_count, Math_t lower, Math_t upper)
	{
		return ModifierBasePtr(new Equalizer(band_count, lower, upper));
	}

	ModifierBasePtr ModifierFactory::CreateEnvelopeFollower(Math_t fd, Math_t fu)
	{
		return ModifierBasePtr(new EnvelopeFollower(fd, fu));
	}

	ModifierBasePtr ModifierFactory::CreateGain(Math_t gain)
	{
		return ModifierBasePtr(new Gain(gain));
	}

	ModifierBasePtr ModifierFactory::CreateGenericFilter(ZeroContainer const & zeros, PoleContainer const & poles)
	{
		return ModifierBasePtr(new GenericFilter(zeros, poles));
	}

	ModifierBasePtr ModifierFactory::CreateLowPass(Math_t cutoff, Math_t resonance)
	{
			// Clamp value to within the accepted range
		resonance = std::clamp(resonance, Math_t(0), Math_t(1)) / Math_t(6);
		return ModifierBasePtr(new LowPass(cutoff, resonance));
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
