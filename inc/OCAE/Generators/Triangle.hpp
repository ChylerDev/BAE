/*! ****************************************************************************
\file             Triangle.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __TRIANGLE_HPP
#define __TRIANGLE_HPP

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
		Triangle wave generator.
	***************************************************************************/
	class Triangle : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

			/// Combination of the sampling rate and desired frequency
		Math_t m_Irate;
			/// Sample to sample increment value
		Math_t m_Inc;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Triangle(Triangle const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Triangle(Triangle && other) = default;

		/*! ********************************************************************
		\brief
			Default destructor.
		***********************************************************************/
		virtual ~Triangle() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Triangle & operator=(Triangle const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Triangle & operator=(Triangle && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Sets a new frequency for the generator.

		\param freq
			The new frequency.
		***********************************************************************/
		void SetFrequency(Math_t freq);

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Calculates the sample. For the base class this is simply 0.

		\return
			The stereo sample data.1
		***********************************************************************/
		virtual StereoData SendSample(void);

		/*! ********************************************************************
		\brief
			Returns boolean for if the object is a GeneratorBase or not.

		\return
			False.
		***********************************************************************/
		virtual bool IsBase() { return false; };

			/// Add the factory as a friend so it can construct Triangle objects
		friend class GeneratorFactory;

	private:

		/*! ********************************************************************
		\brief
			Constructor.

		\param freq
			The frequency for the generator.
		***********************************************************************/
		Triangle(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();
	}; // class Triangle

		/// Alias for a std::shared_ptr instantiated with the Triangle class
	OCAE_TYPEDEF_SHARED(Triangle);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __TRIANGLE_HPP
