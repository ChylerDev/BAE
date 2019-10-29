/*! ****************************************************************************
\file             Block.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __BLOCK_HPP
#define __BLOCK_HPP

// Include Files                ////////////////////////////////////////////////

#include <memory>
#include <type_traits>
#include <vector>

#include "../Engine.hpp"

#include "../Generators/GeneratorBase.hpp"
#include "../Modifiers/ModifierBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Sound
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class Block
	{
	public:

			// StereoData interactor(StereoData GeneratorSample, StereoData ModifierSample)
		using Interaction_f = std::function<StereoData(StereoData, StereoData)>;

		using GenBasePtr = Generator::GeneratorBasePtr;
		using ModBasePtr = Modifier::ModifierBasePtr;

	private:

		// Members              ///////////////////////

		GenBasePtr m_Generator;
		ModBasePtr m_Modifier;

		Interaction_f m_Interaction;

		StereoData m_Input;
		StereoData m_Output;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Block constructor.

		\param gen
			The generator used for the block.

		\param mod
			The modifier used for the block.

		\param interactor
			The function that defines how the output of the generator and the
			modifier are combined. The first argument is the sample from the
			generator, and the second argument is the sample from the modifier.
		***********************************************************************/
		Block(
			GenBasePtr const & gen,
			ModBasePtr const & mod,
			Interaction_f const & interactor
		);

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		GenBasePtr & GetGenerator();
		ModBasePtr & GetModifier();
		GenBasePtr const & GetGenerator() const;
		ModBasePtr const & GetModifier() const;

		void PrimeInput(StereoData input);
		StereoData LastOutput();

		// Functions            ///////////////////////

		void Process();

	private:

		// Functions                  ///////////////////////

	}; // class Block
	TYPEDEF_SHARED(Block);
} // namespace Sound
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __BLOCK_HPP
