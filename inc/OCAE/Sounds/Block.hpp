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
		This class defines a way of holding a Generator, Modifier and a method
		of combining the outputs of both of them to produce a single output
		sample.
	***************************************************************************/
	class Block
	{
	public:

			/// Alias for a function that returns a sample, and takes in a generator sample as the first parameter and a modifier sample as the second parameter
		using Interaction_f = std::function<StereoData(StereoData, StereoData)>;

			/// Alias for GeneratorBasePtr
		using GenBasePtr = Generator::GeneratorBasePtr;
			/// Alias for ModifierBasePtr
		using ModBasePtr = Modifier::ModifierBasePtr;

	private:

		// Members              ///////////////////////

			/// The generator managed by this Block
		GenBasePtr m_Generator;
			/// The modifier managed by this Block
		ModBasePtr m_Modifier;
			/// The interactor used by this Block
		Interaction_f m_Interaction;

			/// The input sample
		StereoData m_Input;
			/// The output sample
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

		/*! ********************************************************************
		\brief
			Returns a reference to the managed generator.

		\return
			The managed generator.
		***********************************************************************/
		GenBasePtr & GetGenerator();

		/*! ********************************************************************
		\brief
			Returns a reference to the managed modifier.

		\return
			The managed modifier.
		***********************************************************************/
		ModBasePtr & GetModifier();

		/*! ********************************************************************
		\brief
			Returns a reference to the managed generator.

		\return
			The managed generator.
		***********************************************************************/
		GenBasePtr const & GetGenerator() const;

		/*! ********************************************************************
		\brief
			Returns a reference to the managed modifier.

		\return
			The managed modifier.
		***********************************************************************/
		ModBasePtr const & GetModifier() const;

		/*! ********************************************************************
		\brief
			Primes the input for the next Process loop.

		\param input
			The input.
		***********************************************************************/
		void PrimeInput(StereoData input);

		/*! ********************************************************************
		\brief
			Returns the output of the last Process loop.

		\return
			The most recent output.
		***********************************************************************/
		StereoData LastOutput();

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Processes the managed objects.
		***********************************************************************/
		void Process();

	private:

		// Functions                  ///////////////////////

	}; // class Block

		/// Alias for std::shared_ptr instantiated with Block.
	TYPEDEF_SHARED(Block);
} // namespace Sound
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __BLOCK_HPP
