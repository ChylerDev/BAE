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

namespace AudioEngine
{
namespace Tools
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class Resampler
	{
	private:

		using Index_t = Math_t;

		// Members              ///////////////////////

		std::vector<StereoData> m_Data;

		Index_t m_Index;
		Math_t const m_IndexIncrement;
		Math_t m_PlaybackSpeed;

		uint64_t m_LoopStart, m_LoopEnd;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor for the resampler.

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
		void SendBlock(StereoData * buffer, uint64_t size);

	private:

		// Functions                  ///////////////////////

	}; // class Resampler
	TYPEDEF_SHARED(Resampler);
} // namespace Tools
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __RESAMPLER_HPP
