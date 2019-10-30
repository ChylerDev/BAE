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
	class Equalizer
	{
	private:

		// Members              ///////////////////////

		std::vector<Math_t> m_BandGains;
		std::vector<BandPassPtr> m_Bands;

	public:

		// Con-/De- structors   ///////////////////////

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		virtual StereoData FilterSample(StereoData const & input);

		virtual bool IsBase() { return false; };

		void SetGain(int band, Math_t gain);

		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		Equalizer(int band_count, Math_t lower, Math_t upper);

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();

		void Reset();

	}; // class Equalizer
	TYPEDEF_SHARED(Equalizer);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __EQUALIZER_HPP
