/*! ****************************************************************************
\file             Sine.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __SINE_HPP
#define __SINE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "GeneratorBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{
	/*! ************************************************************************
	\brief
		Generates sine data at the given frequency.
	***************************************************************************/
	class Sine : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

		Math_t irate;
		SampleType y1, y2;
		Math_t beta;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Default destructor.
		***********************************************************************/
		virtual ~Sine() = default;

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

		/*! ********************************************************************
		\brief
			Sets the frequency to a new value.

		\param freq
			The new frequency.
		***********************************************************************/
		void SetFrequency(Math_t freq);

		friend class GeneratorFactory;

	private:

		/*! ********************************************************************
		\brief
			Creates an object that outputs a simple sine wave without using
			inefficient functions like std::sin.

		\param freq
			The frequency for the sine-wav to output at.
		***********************************************************************/
		Sine(Math_t freq);

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Sets all the coefficients for calculating samples.
		***********************************************************************/
		void Reset(void);

	}; // class Sine
	TYPEDEF_SHARED(Sine);
} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SINE_HPP
