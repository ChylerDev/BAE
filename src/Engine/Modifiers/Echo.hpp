/*! ****************************************************************************
\file             Echo.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __ECHO_HPP
#define __ECHO_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include <deque>

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
		Echo IIR filter. Uses output sample for echoing instead of input,
		creating an infinite impulse responce (IIR).
	***************************************************************************/
	class Echo : public ModifierBase
	{
	private:

		// Members              ///////////////////////

			/// Filtered samples for continuous echo
		std::deque<StereoData> m_Echo;
			/// Decay ratio for the echo
		Math_t m_Ratio;

	public:

		// Con-/De- structors   ///////////////////////

		Echo(Echo const & other) = delete;
		Echo(Echo && other) noexcept = default;

		virtual ~Echo() = default;

		// Operators            ///////////////////////

		Echo & operator=(Echo const & rhs) = delete;
		Echo & operator=(Echo && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		void SetDecayRatio(Math_t decay_ratio);
		Math_t GetDecayRatio() const;

		// Functions            ///////////////////////

		virtual StereoData FilterSample(StereoData const & sample);

		virtual bool IsBase() { return false; };

		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		Echo(uint64_t sample_delay, Math_t decay_ratio);

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();

	}; // class Echo
	TYPEDEF_SHARED(Echo);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __ECHO_HPP
