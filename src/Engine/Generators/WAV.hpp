/*! ****************************************************************************
\file             WAV.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __WAV_HPP
#define __WAV_HPP

// Include Files                ////////////////////////////////////////////////

#include <string>
#include <memory>
#include <vector>

#include "../Engine.hpp"

#include "GeneratorBase.hpp"
#include "../Tools/MethodTable.hpp"
#include "../Tools/Resampler.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	/*! ************************************************************************
	\brief
		Plays audio from a WAV file.

		Supported formats: 8-bit, 16-bit, and 24-bit audio. 48kHz only for now,
		resampler will be added soon.
	***************************************************************************/
	class WAV : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

		Tools::ResamplerPtr m_Resampler;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Default destructor.
		***********************************************************************/
		virtual ~WAV() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Sends a single sample to Core::Driver for output to the OS.

		\return
			The stereo sample data.
		***********************************************************************/
		virtual StereoData SendSample(void);

		virtual bool IsBase() { return false; };

		/*! ********************************************************************
		\brief
			Reads a file from the disk and parses it for the WAV data.

		\param path
			The path to the file.
		***********************************************************************/
		void ReadFile(std::string const & path);

		/*! ********************************************************************
		\brief
			Loads the supplied WAV data and sets up the object to play the audio
			data.

		\param wav_data
			The WAV data
		***********************************************************************/
		void LoadWAV(std::vector<char> const & wav_data);

		friend class GeneratorFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Default constructor. If no data is provided in calling
			WAV::ReadFile, then WAV::SendSample will only output 0 data.
		***********************************************************************/
		WAV();

		/*! ********************************************************************
		\brief
			Path to a WAV file.

		\param path
			The path.
		***********************************************************************/
		WAV(std::string const & path);

		/*! ********************************************************************
		\brief
			std::vector with the contents of a WAV file.

		\param wav_data
			The WAV data
		***********************************************************************/
		WAV(std::vector<char> const & wav_data);

		/*! ********************************************************************
		\brief
			Integer argc parameter passed into main. Uses the functions in
			Input.*pp to access the command-line parameters.

		\param argc
			Parameter passed into main.
		***********************************************************************/
		WAV(int argc);

		void ParseWAV(char const * array, int size);

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();

	}; // class WAV
	TYPEDEF_SHARED(WAV);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __WAV_HPP
