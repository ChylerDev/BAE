/*! ****************************************************************************
\file             GeneratorFactory.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_GENERATORBASEY_HPP
#define __OCAE_GENERATORBASEY_HPP

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
	OCAE_TYPEDEF_SHARED(GeneratorBase);
	class Noise;
	OCAE_TYPEDEF_SHARED(Noise);
	class Sawtooth;
	OCAE_TYPEDEF_SHARED(Sawtooth);
	class Sine;
	OCAE_TYPEDEF_SHARED(Sine);
	class Square;
	OCAE_TYPEDEF_SHARED(Square);
	class Triangle;
	OCAE_TYPEDEF_SHARED(Triangle);
	class WAV;
	OCAE_TYPEDEF_SHARED(WAV);
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
		static NoisePtr CreateNoise();

		/*! ********************************************************************
		\brief
			Creates a Sawtooth object.

		\param freq
			The frequency for the sawtooth.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static SawtoothPtr CreateSawtooth(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a Sine object.

		\param freq
			The frequency for the sine.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static SinePtr CreateSine(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a Square object.

		\param freq
			The frequency for the square.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static SquarePtr CreateSquare(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a Triangle object.

		\param freq
			The frequency for the triangle.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static TrianglePtr CreateTriangle(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a WAV object with no WAV data.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static WAVPtr CreateWAV();

		/*! ********************************************************************
		\brief
			Creates a WAV object with a file name to open for reading.

		\param filepath
			The path to the file.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static WAVPtr CreateWAV(std::string const & filepath);

		/*! ********************************************************************
		\brief
			Creates a WAV object with a vector containing the audio WAV data.

		\param wav_data
			Raw WAVE data in RIFF format.

		\return
			GeneratorBasePtr containing the created object.
		***********************************************************************/
		static WAVPtr CreateWAV(std::vector<char> const & wav_data);

		~GeneratorFactory() = delete; ///< Deleted destructor, ensuring an instance of this class can never be created.

	private:

		// Functions                  ///////////////////////
	}; // class GeneratorFactory
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __OCAE_GENERATORBASEY_HPP
