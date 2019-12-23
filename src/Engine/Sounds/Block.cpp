/*! ****************************************************************************
\file             Block.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "../Generators/GeneratorBase.hpp"
#include "../Modifiers/ModifierBase.hpp"

#include "Block.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Sound
{
	Block::Block(
		GenBasePtr const & gen,
		ModBasePtr const & mod,
		Interaction_f const & interactor
	) :
		m_Generator(gen),
		m_Modifier(mod),
		m_Interaction(interactor),
		m_Input()
	{
	}

	Block::GenBasePtr & Block::GetGenerator()
	{
		return m_Generator;
	}

	Block::ModBasePtr & Block::GetModifier()
	{
		return m_Modifier;
	}

	Block::GenBasePtr const & Block::GetGenerator() const
	{
		return m_Generator;
	}

	Block::ModBasePtr const & Block::GetModifier() const
	{
		return m_Modifier;
	}

	StereoData Block::Process()
	{
		StereoData out = m_Interaction(m_Generator->Process(), m_Modifier->Process(m_Input));
		m_Input = StereoData();

		return out;
	}
} // namespace Sound
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
