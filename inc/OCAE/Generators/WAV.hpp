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
		Plays audio from WAVE data.

		Supported formats: 8-bit, 16-bit, and 24-bit audio.
	***************************************************************************/
	class WAV : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

			/// Resampler used for resampling input WAV data to the OCAE's sampling rate
		Tools::ResamplerPtr m_Resampler;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		WAV(WAV const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		WAV(WAV && other) = default;

		/*! ********************************************************************
		\brief
			Default destructor.
		***********************************************************************/
		virtual ~WAV() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		WAV & operator=(WAV const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		WAV & operator=(WAV && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Sends a single sample to Core::Driver for output to the OS.

		\return
			The stereo sample data.
		***********************************************************************/
		virtual StereoData SendSample(void);

		/*! ********************************************************************
		\brief
			Returns boolean for if the object is a GeneratorBase or not.

		\return
			False.
		***********************************************************************/
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

			/// Add the factory as a friend so it can construct GeneratorBase objects
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

		/*! ********************************************************************
		\brief
			Parses WAVE data from the given raw data.

			NOTE: The data in the array should be the fully RIFF-structured
			      data.

		\param array
			The raw WAVE data to be parsed.

		\param size
			The size of the WAVE data.
		***********************************************************************/
		void ParseWAV(char const * array, int size);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();
	}; // class WAV

		/// Alias for a std::shared_ptr instantiated with the WAV class
	TYPEDEF_SHARED(WAV);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __WAV_HPP
