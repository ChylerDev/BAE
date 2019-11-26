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

namespace OCAE
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
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Sine(Sine const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Sine(Sine && other) noexcept = default;

		/*! ********************************************************************
		\brief
			Destructor.
		***********************************************************************/
		virtual ~Sine() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Sine & operator=(Sine const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Sine & operator=(Sine && rhs) noexcept = default;

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
			Sets the frequency to a new value.

		\param freq
			The new frequency.
		***********************************************************************/
		void SetFrequency(Math_t freq);

		Math_t GetFrequency() const;

		friend class GeneratorFactory;

	protected:

		/*! ********************************************************************
		\brief
			Creates an object that outputs a simple sine wave without using
			inefficient functions like std::sin.

		\param freq
			The frequency for the sine-wav to output at.
		***********************************************************************/
		Sine(Math_t freq);

		// Functions                  ///////////////////////

		virtual Tools::MethodTable::MethodList_t CreateMethodList();

		/*! ********************************************************************
		\brief
			Sets all the coefficients for calculating samples.
		***********************************************************************/
		void Reset(void);

	}; // class Sine
	TYPEDEF_SHARED(Sine);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __SINE_HPP
