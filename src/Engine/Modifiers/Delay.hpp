/*! ****************************************************************************
\file             Delay.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __DELAY_HPP
#define __DELAY_HPP

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
	***************************************************************************/
	class Delay : public ModifierBase
	{
	private:

		// Members              ///////////////////////

		std::deque<StereoData> m_Delay;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~Delay() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		virtual StereoData FilterSample(StereoData const & input);

		virtual bool IsBase() { return false; };

		void SetDelay(uint64_t samples);

		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		Delay(uint64_t samples);

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();

	}; // class Delay
	TYPEDEF_SHARED(Delay);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __DELAY_HPP
