/*! ****************************************************************************
\file             Driver.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_DRIVER_HPP
#define __OCAE_DRIVER_HPP

// Include Files                ////////////////////////////////////////////////

#include <functional>
#include <unordered_map>
#include <memory>

#include "../Engine.hpp"

#include "../Sounds/Sound.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace OCAE
{
namespace Core
{
	class Driver;
	OCAE_TYPEDEF_SHARED(Driver);
}
}

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
			Default copy constructor.

		\param other
			The object to copy.
		***********************************************************************/
		Driver(Driver const & other) = default;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The object to move.
		***********************************************************************/
		Driver(Driver && other) = default;

		/*! ********************************************************************
		\brief
			Destructor.
		***********************************************************************/
		~Driver();

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Default copy-assignment operator.

		\param rhs
			The object to copy.

		\return
			*this.
		***********************************************************************/
		Driver & operator=(Driver const & rhs) = default;

		/*! ********************************************************************
		\brief
			Default move-assignment operator.

		\param rhs
			The object to move.

		\return
			*this.
		***********************************************************************/
		Driver & operator=(Driver && rhs) = default;

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
		void SetGain(Math_t gain = OCAE_DEFAULT_GAIN);

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
		***********************************************************************/
		void Process();

		/*! ********************************************************************
		\brief
			Constructs an audio driver object.

		\param track_size
			The size of the output track in samples.

		\param gain
			The linear gain to be used when summing all audio values.

		\return
			The shared pointer holding the Driver object.
		***********************************************************************/
		static DriverPtr Create(uint64_t track_size, Math_t gain = OCAE_DEFAULT_GAIN)
		{
			return DriverPtr(new Driver(track_size, gain));
		};

	private:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructs an audio driver object.

		\param track_size
			The size of the output track in samples.

		\param gain
			The linear gain to be used when summing all audio values.
		***********************************************************************/
		Driver(uint64_t track_size, Math_t gain = OCAE_DEFAULT_GAIN);

		/*! ********************************************************************
		\brief
			Returns an ID value for use within the driver.

		\return
			The generated ID value.
		***********************************************************************/
		static uint64_t GetID();
	}; // class Driver

		/// Typedef for a std::shared_ptr instantiated with the Driver class
	OCAE_TYPEDEF_SHARED(Driver);
} // namespace Core
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __OCAE_DRIVER_HPP