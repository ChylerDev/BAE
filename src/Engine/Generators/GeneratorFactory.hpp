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

#include <string>
#include <vector>

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

		static GeneratorBasePtr CreateBase();
		static GeneratorBasePtr CreateNoise();
		static GeneratorBasePtr CreateSawtooth(Math_t freq);
		static GeneratorBasePtr CreateSine(Math_t freq);
		static GeneratorBasePtr CreateSquare(Math_t freq);
		static GeneratorBasePtr CreateTriangle(Math_t freq);
		static GeneratorBasePtr CreateWAV();
		static GeneratorBasePtr CreateWAV(std::string const & filepath);
		static GeneratorBasePtr CreateWAV(std::vector<char> const & wav_data);
		static GeneratorBasePtr CreateWAV(int argc);

		~GeneratorFactory() = delete;

	private:

		// Functions                  ///////////////////////

	}; // class GeneratorFactory
} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERATORBASEY_HPP
