/*! ****************************************************************************
\file             Equalizer.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __EQUALIZER_HPP
#define __EQUALIZER_HPP

// Include Files                ////////////////////////////////////////////////

#include <vector>

#include "../Engine.hpp"

#include "BandPass.hpp"
#include "ModifierBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class Equalizer : public ModifierBase
	{
	private:

		// Members              ///////////////////////

		std::vector<Math_t> m_BandGains;
		std::vector<BandPassPtr> m_Bands;

	public:

		// Con-/De- structors   ///////////////////////

		Equalizer(Equalizer const & other) = delete;
		Equalizer(Equalizer && other) noexcept = default;

		virtual ~Equalizer() = default;

		// Operators            ///////////////////////

		Equalizer & operator=(Equalizer const & rhs) = delete;
		Equalizer & operator=(Equalizer && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		void SetGain(uint32_t band, Math_t gain);

		Math_t GetGain(uint32_t band);

		// Functions            ///////////////////////

		virtual StereoData FilterSample(StereoData const & input);

		virtual bool IsBase() { return false; };

		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		Equalizer(uint32_t band_count, Math_t lower, Math_t upper);

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();

	}; // class Equalizer
	TYPEDEF_SHARED(Equalizer);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __EQUALIZER_HPP
