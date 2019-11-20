/*! ****************************************************************************
\file             BandPass.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __BAND_PASS_HPP
#define __BAND_PASS_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "ModifierBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	class ModifierFactory;
	class Equalizer;
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
	class BandPass : public ModifierBase
	{
	private:

		// Members              ///////////////////////

		Math_t m_CentralFrequency;
		Math_t m_Quality;
		Math_t m_A0, m_B1, m_B2;
		StereoData m_X1, m_X2, m_Y1, m_Y2;

	public:

		// Con-/De- structors   ///////////////////////

		BandPass(BandPass const & other) = delete;
		BandPass(BandPass && other) noexcept = default;

		virtual ~BandPass();

		// Operators            ///////////////////////

		BandPass & operator=(BandPass const & rhs) = delete;
		BandPass & operator=(BandPass && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		Math_t GetFrequency() const;
		void SetFrequency(Math_t f);

		Math_t GetQuality() const;
		void SetQuality(Math_t Q);

		// Functions            ///////////////////////

		virtual StereoData FilterSample(StereoData const & x);

		virtual bool IsBase() { return false; };

		friend class ModifierFactory;
		friend class Equalizer;

	protected:

		// Functions                  ///////////////////////

		BandPass(Math_t f, Math_t Q = 1);

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();

		void Reset(void);

	}; // class BandPass
	TYPEDEF_SHARED(BandPass);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __BAND_PASS_HPP
