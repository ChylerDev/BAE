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

	ModifierBasePtr ModifierFactory::CreateADSR(uint64_t attack, uint64_t decay, Math_t sustain, uint64_t release)
	{
		return ModifierBasePtr(new ADSR(attack, decay, sustain, release));
	}

	ModifierBasePtr ModifierFactory::CreateBandPass(Math_t f, Math_t Q)
	{
		return ModifierBasePtr(new BandPass(f, Q));
	}

	ModifierBasePtr ModifierFactory::CreateDelay(uint64_t samples)
	{
		return ModifierBasePtr(new Delay(samples));
	}

	ModifierBasePtr ModifierFactory::CreateEcho(uint64_t sample_delay, Math_t decay_ratio)
	{
		return ModifierBasePtr(new Echo(sample_delay, decay_ratio));
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
		return ModifierBasePtr(new LowPass(cutoff, resonance));
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
