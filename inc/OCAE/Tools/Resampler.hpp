/*! ****************************************************************************
\file             Resampler.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __RESAMPLER_HPP
#define __RESAMPLER_HPP

// Include Files                ////////////////////////////////////////////////

#include <memory>
#include <vector>

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Tools
{
	/*! ************************************************************************
	\brief
		Class for taking audio data of one sampling rate and translating it to
		another sampling rate.
	***************************************************************************/
	class Resampler
	{
	private:

			/// Type used for fractional indexing
		using Index_t = Math_t;

		// Members              ///////////////////////

			/// The original audio data
		std::vector<StereoData> m_Data;

			/// The index for tracking position within the audio data
		Index_t m_Index;
			/// The value to increment the index by
		Math_t const m_IndexIncrement;
			/// The playback speed, allows speeding up and slowing down the data
		Math_t m_PlaybackSpeed;

			/// The start position of the loop in samples, if any
		uint64_t m_LoopStart;
			/// The end position of the loop in samples, if any
		uint64_t m_LoopEnd;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor for the resampler. If the resampler is set up to loop,
			the range of the looping is [LoopStart, LoopEnd).

		\param AudioData
			A const reference to the audio data.

		\param SourceSampleRate
			The sample rate of the source data.

		\param LoopStart
			The sample to start looping from. Defaults to 0.

		\param LoopEnd
			The sample at the loop point to loop back to LoopStart. Defaults to
			0, which is interpretted as no looping.
		***********************************************************************/
		Resampler(std::vector<StereoData> const & AudioData,
				  int32_t SourceSampleRate,
				  uint64_t LoopStart = 0, uint64_t LoopEnd = 0
		);

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Sets the playback speed.
			1.0 is original playback speed.
		
		\param playback_speed
			The playback speed
		***********************************************************************/
		void SetPlaybackSpeed(Math_t playback_speed = 1.0);

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Sends a single sample to Core::Driver for output to the OS.

		\return
			The stereo sample data.
		***********************************************************************/
		StereoData SendSample();

	private:

		// Functions                  ///////////////////////
	}; // class Resampler

		/// Alias for a std::shared_ptr instantiated with the Resampler class
	TYPEDEF_SHARED(Resampler);
} // namespace Tools
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __RESAMPLER_HPP
