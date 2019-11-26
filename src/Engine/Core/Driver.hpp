/*! ****************************************************************************
\file             Driver.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __DRIVER_HPP
#define __DRIVER_HPP

// Include Files                ////////////////////////////////////////////////

#include <functional>
#include <unordered_map>
#include <memory>

#include "../Engine.hpp"

#include "../Sounds/Sound.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Core
{
	/*! ************************************************************************
	\brief
		Handles the calculation of audio samples from different Sounds.
	***************************************************************************/
	class Driver
	{
	private:

		// Members              ///////////////////////

			/// The output track to store the results of processing
		Track_t m_OutputTrack;
			/// All the sounds this driver is responsible for
		std::unordered_map<uint64_t, Sound::SoundPtr> m_Sounds;
			/// The output gain for the output samples
		Math_t m_Gain;

			/// ID counter for generating IDs
		static uint64_t s_IDCounter;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Constructs an audio driver object.

		\param track_size
			The size of the output track in samples.

		\param gain
			The linear gain to be used when summing all audio values.
		***********************************************************************/
		Driver(uint64_t track_size, Math_t gain = DEFAULT_GAIN);

		Driver(Driver const &) = default;       ///< Default copy constructor
		Driver(Driver &&) noexcept = default;   ///< Default move constructor

		/*! ********************************************************************
		\brief
			Destructor.
		***********************************************************************/
		~Driver();

		// Operators            ///////////////////////

		Driver & operator=(Driver const &) = default;       ///< Default copy-assignment operator
		Driver & operator=(Driver &&) noexcept = default;   ///< Default move-assignment operator

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Adds the given sound to the internal list of tracked sounds.

		\param sound
			The sound to add.

		\return
			ID of the added sound.
		***********************************************************************/
		uint64_t AddSound(Sound::SoundPtr const & sound);

		/*! ********************************************************************
		\brief
			Removes a sound from the Driver's processing.

		\param id
			The ID of the sound to be removed.

		\return
			The sound that was removed.
		***********************************************************************/
		Sound::SoundPtr RemoveSound(uint64_t id);

		/*! ********************************************************************
		\brief
			Sets the gain to be used when summing all the audio values.

		\param gain
			The linear gain value to be set.
		***********************************************************************/
		void SetGain(Math_t gain = DEFAULT_GAIN);

		/*! ********************************************************************
		\brief
			Returns the track used for writing audio output after it has been
			processed.

		\return
			Track_t containing the output of the latest process call.
		***********************************************************************/
		Track_t const & GetOutputTrack() const;

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Processes audio and returns a track of the calculated samples.

		\return
			The calculated samples
		***********************************************************************/
		void Process();

	private:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Returns an ID value for use within the driver.

		\return
			The generated ID value.
		***********************************************************************/
		static uint64_t GetID();
	}; // class Driver

		/// Typedef for a std::shared_ptr instantiated with the Driver class
	TYPEDEF_SHARED(Driver);
} // namespace Core
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __DRIVER_HPP
