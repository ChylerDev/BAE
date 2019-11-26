/*! ****************************************************************************
\file             WAVWriter.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __WAVWRITER_HPP
#define __WAVWRITER_HPP

// Include Files                ////////////////////////////////////////////////

#include <RIFF-Util/RIFF.hpp>

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
		To be used in tandom with the recording system built into Core::Driver.

	\param audio
		The audio to be formatted into WAVE (RIFF) data.

	\return
		The formatted data.
	***************************************************************************/
	RIFF::vector_t WriteWAV(Track_t const & audio);
} // namespace Tools
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __WAVWRITER_HPP
