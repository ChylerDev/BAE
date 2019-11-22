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

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	class GeneratorBase;
	TYPEDEF_SHARED(GeneratorBase);
}
}

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	/*! ************************************************************************
	\brief
		Creates pointers to generators handled by std::shared_ptr to prevent
		memory leaks.
	***************************************************************************/
	class GeneratorFactory
	{
	private:

		// Members              ///////////////////////

	public:

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Creates a GeneratorBase object.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateBase();

		/*! ********************************************************************
		\brief
			Creates a Noise object.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateNoise();

		/*! ********************************************************************
		\brief
			Creates a Sawtooth object.

		\param freq
			The frequency for the sawtooth.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateSawtooth(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a Sine object.

		\param freq
			The frequency for the sine.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateSine(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a Square object.

		\param freq
			The frequency for the square.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateSquare(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a Triangle object.

		\param freq
			The frequency for the triangle.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateTriangle(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a WAV object with no WAV data.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateWAV();

		/*! ********************************************************************
		\brief
			Creates a WAV object with a file name to open for reading.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateWAV(std::string const & filepath);

		/*! ********************************************************************
		\brief
			Creates a WAV object with a vector containing the audio WAV data.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static GeneratorBasePtr CreateWAV(std::vector<char> const & wav_data);

		~GeneratorFactory() = delete; ///< Deleted destructor, ensuring an instance of this class can never be created.

	private:

		// Functions                  ///////////////////////

	}; // class GeneratorFactory
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERATORBASEY_HPP
