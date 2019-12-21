/*! ****************************************************************************
\file             Sine.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_SINE_HPP
#define __OCAE_SINE_HPP

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

			/// Value storing the non-integer index increment value
		Math_t m_A;
			/// The current index in the wave table to access
		Math_t m_Index;

		static Math_t s_Table[OCAE_SAMPLE_RATE/10];
		static int dummy;

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
		Sine(Sine && other) = default;

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
		Sine & operator=(Sine && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Sets the frequency to a new value.

		\param freq
			The new frequency.
		***********************************************************************/
		void SetFrequency(Math_t freq);

		/*! ********************************************************************
		\brief
			Gets the current frequency.

		\return
			The frequency of the generator.
		***********************************************************************/
		Math_t GetFrequency() const;

		// Functions            ///////////////////////

		OCAE_TODO("Fix this doc")
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

			/// Add the factory as a friend so it can construct Sine objects
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

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();

	private:

		// Functions                  ///////////////////////

		static int SetupWaveTable();
	}; // class Sine

		/// Alias for a std::shared_ptr instantiated with the Sine class
	OCAE_TYPEDEF_SHARED(Sine);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __OCAE_SINE_HPP
