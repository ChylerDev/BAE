/*! ****************************************************************************
\file             Noise.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __NOISE_HPP
#define __NOISE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include <random>

#include "GeneratorBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class Noise : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

		std::uniform_int_distribution<int16_t> m_Distribution;
		std::default_random_engine m_Engine;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~Noise() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		virtual StereoData SendSample(void);

		friend class GeneratorFactory;

	private:

		// Functions                  ///////////////////////

		Noise();
	}; // class Noise
	TYPEDEF_SHARED(Noise);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __NOISE_HPP
