/*! ****************************************************************************
\file             SoundFactory.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_SOUND_FACTORY
#define __OCAE_SOUND_FACTORY

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "../Modifiers/ModifierBase.hpp"
#include "../Generators/GeneratorBase.hpp"
#include "Sound.hpp"

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
		Class containing functions that will generate Sound and Block objects
		from common inputs.
	***************************************************************************/
	class SoundFactory
	{
	private:

		// Members              ///////////////////////

	public:

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Creates a Sound object with no associated generators or modifiers.

		\return
			The generated Sound object wrapped inside a std::shared_ptr.
		***********************************************************************/
		static SoundPtr CreateEmptySound();

		/*! ********************************************************************
		\brief
			Creates a Sound object from a given generator.

		\param g
			The generator to be processed within this Sound object.

		\return
			The generated Sound object wrapped inside a std::shared_ptr.
		***********************************************************************/
		static SoundPtr CreateBasicGenerator(Generator::GeneratorBasePtr const & g);

		/*! ********************************************************************
		\brief
			Creates a Sound object from a given modifier.
			The modifier takes input from the input the Sound object is given.

		\param m
			The modifier to be processed within this Sound object.

		\return
			The generated Sound object wrapped inside a std::shared_ptr.
		***********************************************************************/
		static SoundPtr CreateBasicModifier(Modifier::ModifierBasePtr const & m);

		/*! ********************************************************************
		\brief
			Creates a Block object from a given generator.

			When processed, the output of the generator is forwarded to the
			output of the Block.

		\param g
			The generator to be held within the Block.

		\return
			The generated Block object wrapped inside a std::shared_ptr.
		***********************************************************************/
		static BlockPtr CreateBlock(Generator::GeneratorBasePtr const & g);

		/*! ********************************************************************
		\brief
			Creates a Block object from a given modifier.

			When processed, the output of the modifier is forwarded to the
			output of the Block.

		\param m
			The modifier to be held within the Block.

		\return
			The generated Block object wrapped inside a std::shared_ptr.
		***********************************************************************/
		static BlockPtr CreateBlock(Modifier::ModifierBasePtr const & m);

		/*! ********************************************************************
		\brief
			Creates a Block object from a given generator and modifier.

			When processed, the output of the generator and modifier are
			multiplied together and sent to the output of the Block.

		\param g
			The generator to be held within the Block.

		\param m
			The modifier to be held within the Block.

		\return
			The generated Block object wrapped inside a std::shared_ptr.
		***********************************************************************/
		static BlockPtr CreateBlock(Generator::GeneratorBasePtr const & g,
		                            Modifier::ModifierBasePtr const & m);

		/*! ********************************************************************
		\brief
			Creates a Block object from a given generator, modifier, and
			interactor.

			When processed, the output of the generator and modifier are
			combined together using the given interactor and sent to the output
			of the Block.

		\param g
			The generator to be held within the Block.

		\param m
			The modifier to be held within the Block.

		\param interactor
			Function that will combine outputs from the generator and modifier
			when the Block is processed.

		\return
			The generated Block object wrapped inside a std::shared_ptr.
		***********************************************************************/
		static BlockPtr CreateBlock(Generator::GeneratorBasePtr const & g,
		                            Modifier::ModifierBasePtr const & m,
		                            Block::Interaction_f const & interactor);

		~SoundFactory() = delete; ///< Deleted destructor, ensuring an instance of this class can never be created.

	private:

		// Functions                  ///////////////////////

	}; // class SoundFactory
} // namespace Sound
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __OCAE_SOUND_FACTORY
