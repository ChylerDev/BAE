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

namespace OCAE
{
namespace Generator
{
	GeneratorBasePtr GeneratorFactory::CreateBase()
	{
		return GeneratorBasePtr(new GeneratorBase());
	}

	GeneratorBasePtr GeneratorFactory::CreateNoise()
	{
		return GeneratorBasePtr(new Noise());
	}

	GeneratorBasePtr GeneratorFactory::CreateSawtooth(Math_t freq)
	{
		return GeneratorBasePtr(new Sawtooth(freq));
	}

	GeneratorBasePtr GeneratorFactory::CreateSine(Math_t freq)
	{
		return GeneratorBasePtr(new Sine(freq));
	}

	GeneratorBasePtr GeneratorFactory::CreateSquare(Math_t freq)
	{
		return GeneratorBasePtr(new Square(freq));
	}

	GeneratorBasePtr GeneratorFactory::CreateTriangle(Math_t freq)
	{
		return GeneratorBasePtr(new Triangle(freq));
	}

	GeneratorBasePtr GeneratorFactory::CreateWAV()
	{
		return GeneratorBasePtr(new WAV());
	}

	GeneratorBasePtr GeneratorFactory::CreateWAV(std::string const & filepath)
	{
		return GeneratorBasePtr(new WAV(filepath));
	}

	GeneratorBasePtr GeneratorFactory::CreateWAV(std::vector<char> const & wav_data)
	{
		return GeneratorBasePtr(new WAV(wav_data));
	}

	GeneratorBasePtr GeneratorFactory::CreateWAV(int argc)
	{
		return GeneratorBasePtr(new WAV(argc));
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
