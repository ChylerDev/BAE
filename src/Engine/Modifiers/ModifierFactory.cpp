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
#include "Envelope.hpp"
#include "Gain.hpp"
#include "GenericFilter.hpp"
#include "LowPass.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{
	ModifierBasePtr ModifierFactory::CreateBase()
	{
		return std::make_shared<ModifierBase>();
	}

	ModifierBasePtr ModifierFactory::CreateADSR(uint64_t attack, uint64_t decay, Math_t sustain, uint64_t release)
	{
		return std::make_shared<ADSR>(attack, decay, sustain, release);
	}

	ModifierBasePtr ModifierFactory::CreateBandPass(Math_t f, Math_t Q = 1)
	{
		return std::make_shared<BandPass>(f, Q);
	}

	ModifierBasePtr ModifierFactory::CreateDelay(uint64_t samples)
	{
		return std::make_shared<Delay>(samples);
	}

	ModifierBasePtr ModifierFactory::CreateEcho(uint64_t sample_delay, Math_t decay_ratio)
	{
		return std::make_shared<Echo>(sample_delay, decay_ratio);
	}

	ModifierBasePtr ModifierFactory::CreateEnvelopeFollower(Math_t fd, Math_t fu)
	{
		return std::make_shared<EnvelopeFollower>(fd, fu);
	}

	ModifierBasePtr ModifierFactory::CreateGain(Math_t gain = DEFAULT_GAIN)
	{
		return std::make_shared<Gain>(gain);
	}

	ModifierBasePtr ModifierFactory::CreateGenericFilter(ZeroContainer const & zeros, PoleContainer const & poles)
	{
		return std::make_shared<GenericFilter>(zeros, poles);
	}

	ModifierBasePtr ModifierFactory::CreateLowPass(Math_t cutoff, Math_t resonance)
	{
		return std::make_shared<LowPass>(cutoff, resonance);
	}
} // namespace Modifier
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
