/*! ****************************************************************************
\file             Sawtooth.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __SAWTOOTH_HPP
#define __SAWTOOTH_HPP

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
		Generates a sawtooth sound.
	***************************************************************************/
	class Sawtooth : public GeneratorBase
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
		Sawtooth(Sawtooth const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Sawtooth(Sawtooth && other) = default;

		/*! ********************************************************************
		\brief
			Default destructor.
		***********************************************************************/
		virtual ~Sawtooth() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Sawtooth & operator=(Sawtooth const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Sawtooth & operator=(Sawtooth && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Sets a new frequency.

		\param freq
			The new frequency.
		***********************************************************************/
		void SetFrequency(Math_t freq);

		/*! ********************************************************************
		\brief
			Gets the current frequency.

		\return
			The current frequency.
		***********************************************************************/
		Math_t GetFrequency() const;

		// Functions            ///////////////////////

		TODO("Fix this doc")
		/*! ********************************************************************
		\brief
			Calculates the sample. For the base class this is simply 0.

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

			/// Add the factory as a friend so it can construct Sawtooth objects
		friend class GeneratorFactory;

	private:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param freq
			The frequency for the generator.
		***********************************************************************/
		Sawtooth(Math_t freq);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();
	}; // class Sawtooth

		/// Alias for a std::shared_ptr instantiated with the Sawtooth class
	TYPEDEF_SHARED(Sawtooth);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __SAWTOOTH_HPP
