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

	NoisePtr GeneratorFactory::CreateNoise()
	{
		return NoisePtr(new Noise());
	}

	SawtoothPtr GeneratorFactory::CreateSawtooth(Math_t freq)
	{
		return SawtoothPtr(new Sawtooth(freq));
	}

	SinePtr GeneratorFactory::CreateSine(Math_t freq)
	{
		return SinePtr(new Sine(freq));
	}

	SquarePtr GeneratorFactory::CreateSquare(Math_t freq)
	{
		return SquarePtr(new Square(freq));
	}

	TrianglePtr GeneratorFactory::CreateTriangle(Math_t freq)
	{
		return TrianglePtr(new Triangle(freq));
	}

	WAVPtr GeneratorFactory::CreateWAV()
	{
		return WAVPtr(new WAV());
	}

	WAVPtr GeneratorFactory::CreateWAV(std::string const & filepath)
	{
		return WAVPtr(new WAV(filepath));
	}

	WAVPtr GeneratorFactory::CreateWAV(std::vector<char> const & wav_data)
	{
		return WAVPtr(new WAV(wav_data));
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
