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
#include <memory>
#include <vector>

#include "../Engine.hpp"

#include "../Sounds/Sound.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Core
{
	/*! ************************************************************************
	\brief
		Initiates Port Audio and handles all related calls.
	***************************************************************************/
	class Driver
	{
	private:

		// Members              ///////////////////////

		Track_t m_OutputTrack;

		std::vector<Sound::SoundPtr> m_Sounds;

		Math_t m_Gain;

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

		Driver(Driver const &) = default;
		Driver(Driver &&) noexcept = default;

		/*! ********************************************************************
		\brief
			Destructor.
		***********************************************************************/
		~Driver();

		// Operators            ///////////////////////

		Driver & operator=(Driver const &) = default;
		Driver & operator=(Driver &&) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Adds the given sound to the internal list of tracked sounds.

		\param sound
			The sound to add.
		***********************************************************************/
		void AddSound(Sound::SoundPtr const & sound);

		/*! ********************************************************************
		\brief
			Sets the gain to be used when summing all the audio values.

		\param gain
			The linear gain value to be set.
		***********************************************************************/
		void SetGain(Math_t gain = DEFAULT_GAIN);

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

	}; // class Driver
	TYPEDEF_SHARED(Driver);
} // namespace Core
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __DRIVER_HPP
