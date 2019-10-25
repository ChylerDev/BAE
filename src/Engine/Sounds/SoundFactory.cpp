/*! ****************************************************************************
\file             SoundFactory.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include <vector>

#include "../Modifiers/ModifierFactory.hpp"
#include "SoundFactory.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Sound
{
	SoundPtr SoundFactory::CreateBasicGenerator(Generator::GeneratorBasePtr const & gen)
	{
			// Create the base sound
		SoundPtr sound(new Sound());

			// Save reference to the graph
		auto & graph = sound->GetGraph();

			// Add the generator to the graph
		graph.back()->inputs.push_back(
			Sound::Edge::E_BlockPtr(new Sound::Edge::E_Block(CreateBlock(gen)))
		);

		return sound;
	}

	SoundPtr SoundFactory::CreateBasicModifier(Modifier::ModifierBasePtr const & mod)
	{
		SoundPtr sound(new Sound());

			// Save reference to the graph
		auto & graph = sound->GetGraph();

			// Add the modifier to the graph
		graph.back()->inputs.push_back(
			Sound::Edge::E_BlockPtr(new Sound::Edge::E_Block(CreateBlock(mod)))
		);

		return sound;
	}

	SoundPtr SoundFactory::CreateEqualizer(uint32_t band_count, Math_t lower, Math_t upper)
	{
		Math_t const delta = Math_t((std::log10(upper)-std::log10(lower))/band_count);

		std::vector<Math_t> corners;
		corners.push_back(lower);
		for(uint32_t i = 1; i < band_count-1; ++i)
		{
			corners.push_back(
				corners.front()*std::pow(10, (i/band_count) * delta)
			);
		}
		corners.push_back(upper);

		std::vector<Math_t> centers;
		for(uint32_t i = 0; i < corners.size() - 1; ++i)
		{
			centers.push_back(std::sqrt(corners[i]*corners[i+1]));
		}

		Math_t const Q = centers.front()/(corners[1] * corners.front());

		std::deque<Sound::Edge::E_BlockPtr> bands;

		for(auto & f : centers)
		{
			bands.push_back(Sound::Edge::E_BlockPtr(new Sound::Edge::E_Block(
				CreateBlock(
					Modifier::ModifierFactory::CreateBandPass(f, Q)
				)
			)));
		}

		SoundPtr sound(new Sound);

		auto & graph = sound->GetGraph();

		graph.front()->outputs = bands;
		graph.back()->inputs = bands;

		return sound;
	}

	BlockPtr SoundFactory::CreateBlock(Generator::GeneratorBasePtr const & gen)
	{
		return BlockPtr(new Block(
			gen,
			Modifier::ModifierFactory::CreateBase(),
			[](StereoData g, StereoData m)->StereoData{ UNREFERENCED_PARAMETER(m); return g; }
		));
	}

	BlockPtr SoundFactory::CreateBlock(Modifier::ModifierBasePtr const & mod)
	{
		return BlockPtr(new Block(
			Generator::GeneratorFactory::CreateBase(),
			mod,
			[](StereoData g, StereoData m)->StereoData{UNREFERENCED_PARAMETER(g); return m;}
		));
	}

	BlockPtr SoundFactory::CreateBlock(Generator::GeneratorBasePtr const & gen, Modifier::ModifierBasePtr const & mod)
	{
		return BlockPtr(new Block(
			gen,
			mod,
			[](StereoData g, StereoData m)->StereoData{return StereoData(Left(g)*Left(m),Right(g)*Right(m));}
		));
	}

	BlockPtr SoundFactory::CreateBlock(Generator::GeneratorBasePtr const & gen, Modifier::ModifierBasePtr const & mod, Block::Interaction_f const & interactor)
	{
		return BlockPtr(new Block(gen, mod, interactor));
	}
} // namespace Sound
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
