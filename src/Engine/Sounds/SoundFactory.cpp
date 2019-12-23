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

namespace OCAE
{
namespace Sound
{
	SoundPtr SoundFactory::CreateEmptySound()
	{
		return SoundPtr(new Sound());
	}

	SoundPtr SoundFactory::CreateBasicGenerator(Generator::GeneratorBasePtr const & gen)
	{
			// Create the base sound
		SoundPtr sound(new Sound());

		BlockPtr block = CreateBlock(gen);

		sound->AddConnection(block, sound->GetOutputBlock());

		return sound;
	}

	SoundPtr SoundFactory::CreateBasicModifier(Modifier::ModifierBasePtr const & mod)
	{
		SoundPtr sound(new Sound());

		BlockPtr block = CreateBlock(mod);

		sound->AddConnection(sound->GetInputBlock(), block);
		sound->AddConnection(block, sound->GetOutputBlock());

		return sound;
	}

	BlockPtr SoundFactory::CreateBlock(Generator::GeneratorBasePtr const & gen)
	{
		return BlockPtr(new Block(
			gen,
			Modifier::ModifierFactory::CreateBase(),
			[](StereoData g, StereoData m)->StereoData{ OCAE_UNREFERENCED_PARAMETER(m); return g; }
		));
	}

	BlockPtr SoundFactory::CreateBlock(Modifier::ModifierBasePtr const & mod)
	{
		return BlockPtr(new Block(
			Generator::GeneratorFactory::CreateBase(),
			mod,
			[](StereoData g, StereoData m)->StereoData{OCAE_UNREFERENCED_PARAMETER(g); return m;}
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
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
