/*! ****************************************************************************
\file             GeneratorFactory.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "GeneratorFactory.hpp"

#include "Noise.hpp"
#include "Sawtooth.hpp"
#include "Sine.hpp"
#include "Square.hpp"
#include "Triangle.hpp"
#include "WAV.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{
	GeneratorBasePtr GeneratorFactory::CreateNoise()
	{
		return std::make_shared<Noise>();
	}

	GeneratorBasePtr GeneratorFactory::CreateSawtooth(Math_t freq)
	{
		return std::make_shared<Sawtooth>(freq);
	}

	GeneratorBasePtr GeneratorFactory::CreateSine(Math_t freq)
	{
		return std::make_shared<Sine>(freq);
	}

	GeneratorBasePtr GeneratorFactory::CreateSquare(Math_t freq)
	{
		return std::make_shared<Square>(freq);
	}

	GeneratorBasePtr GeneratorFactory::CreateTriangle(Math_t freq)
	{
		return std::make_shared<Triangle>(freq);
	}

	GeneratorBasePtr GeneratorFactory::CreateWAV()
	{

	}
} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
