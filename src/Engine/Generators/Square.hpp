/*! ****************************************************************************
\file             Square.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __SQUARE_HPP
#define __SQUARE_HPP

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
		Generates square wave data at the given frequency.
	***************************************************************************/
	class Square : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

			/// Current time value
		Math_t m_Ind;
			/// Point of inversion
		Math_t m_Inv;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Square(Square const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Square(Square && other) noexcept = default;

		/*! ********************************************************************
		\brief
			Destructor.
		***********************************************************************/
		virtual ~Square() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Square & operator=(Square const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Square & operator=(Square && rhs) noexcept = default;

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
			Sets the frequency to a new value.

		\param freq
			The new frequency.
		***********************************************************************/
		void SetFrequency(Math_t freq);

			/// Add the factory as a friend so it can construct Square objects
		friend class GeneratorFactory;

	private:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Creates an object that outputs a simple square wave without using
			inefficient functions like std::sin.

		\param freq
			The frequency for the square wav to output at.
		***********************************************************************/
		Square(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();
	}; // class Square

		/// Alias for a std::shared_ptr instantiated with the Square class
	TYPEDEF_SHARED(Square);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __SQUARE_HPP
