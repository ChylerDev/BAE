/*! ****************************************************************************
\file             GeneratorFactory.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __GENERATORBASEY_HPP
#define __GENERATORBASEY_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "GeneratorBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{
	/*! ************************************************************************
	\brief
	*******************h********************************************************/
	class GeneratorFactory
	{
	private:

		// Members              ///////////////////////

	public:

		// Functions            ///////////////////////

		GeneratorBasePtr CreateNoise();
		GeneratorBasePtr CreateSawtooth(Math_t freq);
		GeneratorBasePtr CreateSine(Math_t freq);
		GeneratorBasePtr CreateSquare(Math_t freq);
		GeneratorBasePtr CreateTriangle(Math_t freq);
		GeneratorBasePtr CreateWAV();

	private:

		// Functions                  ///////////////////////

	}; // class GeneratorFactory
} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERATORBASEY_HPP
