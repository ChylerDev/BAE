/*! ****************************************************************************
\file             ModifierFactory.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <algorithm>

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

	ADSRPtr ModifierFactory::CreateADSR(Math_t attack, Math_t decay, Math_t sustain, Math_t release)
	{
		return ADSRPtr(new ADSR(
			uint64_t(attack*SAMPLE_RATE),
			uint64_t(decay*SAMPLE_RATE),
			Math_t(DB_TO_LINEAR(sustain)),
			uint64_t(release*SAMPLE_RATE)
		));
	}

	BandPassPtr ModifierFactory::CreateBandPass(Math_t lower, Math_t upper)
	{
		Math_t const fc(std::sqrt(lower * upper));
		return BandPassPtr(new BandPass(fc, fc/(upper-lower)));
	}

	DelayPtr ModifierFactory::CreateDelay(Math_t seconds)
	{
		return DelayPtr(new Delay(uint64_t(seconds * SAMPLE_RATE)));
	}

	EchoPtr ModifierFactory::CreateEcho(Math_t delay_seconds, Math_t decay_ratio)
	{
			// Clamp value to within the accepted range
		decay_ratio = std::clamp(decay_ratio, Math_t(0), Math_t(1));
		return EchoPtr(new Echo(uint64_t(delay_seconds * SAMPLE_RATE), decay_ratio));
	}

	EqualizerPtr ModifierFactory::CreateEqualizer(uint32_t band_count, Math_t lower, Math_t upper)
	{
		return EqualizerPtr(new Equalizer(band_count, lower, upper));
	}

	EnvelopeFollowerPtr ModifierFactory::CreateEnvelopeFollower(Math_t fd, Math_t fu)
	{
		return EnvelopeFollowerPtr(new EnvelopeFollower(fd, fu));
	}

	GainPtr ModifierFactory::CreateGain(Math_t gain)
	{
		return GainPtr(new Gain(gain));
	}

	GenericFilterPtr ModifierFactory::CreateGenericFilter(ZeroContainer const & zeros, PoleContainer const & poles)
	{
		return GenericFilterPtr(new GenericFilter(zeros, poles));
	}

	LowPassPtr ModifierFactory::CreateLowPass(Math_t cutoff, Math_t resonance)
	{
			// Clamp value to within the accepted range
		resonance = std::clamp(resonance, Math_t(0), Math_t(1)) / Math_t(6);
		return LowPassPtr(new LowPass(cutoff, resonance));
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
