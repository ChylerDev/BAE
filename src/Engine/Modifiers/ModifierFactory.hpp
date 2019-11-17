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

		using ZeroContainer = std::vector<std::tuple<uint32_t,Math_t>>;
		using PoleContainer = std::vector<std::tuple<uint32_t,Math_t>>;

		// Functions            ///////////////////////

		static ModifierBasePtr CreateBase();
		static ModifierBasePtr CreateADSR(uint64_t attack, uint64_t decay, Math_t sustain, uint64_t release);
		static ModifierBasePtr CreateBandPass(Math_t f, Math_t Q = 1);
		static ModifierBasePtr CreateDelay(uint64_t samples);
		static ModifierBasePtr CreateEcho(uint64_t sample_delay, Math_t decay_ratio);
		static ModifierBasePtr CreateEqualizer(uint32_t band_count = 2, Math_t lower = 20, Math_t upper = 20000);
		static ModifierBasePtr CreateEnvelopeFollower(Math_t fd, Math_t fu);
		static ModifierBasePtr CreateGain(Math_t gain = DEFAULT_GAIN);
		static ModifierBasePtr CreateGenericFilter(ZeroContainer const & zeros, PoleContainer const & poles);
		static ModifierBasePtr CreateLowPass(Math_t cutoff, Math_t resonance);

		~ModifierFactory() = delete;

	private:

		// Functions                  ///////////////////////

	}; // class ModifierFactory
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __MODIFIERFACTORY_HPP
