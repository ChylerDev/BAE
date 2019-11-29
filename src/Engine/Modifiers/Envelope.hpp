/*! ****************************************************************************
\file             Envelope.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __ENVELOPE_HPP
#define __ENVELOPE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "ModifierBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	/*! ************************************************************************
	\brief
		Envelope follower filter. Calculates the gain of the input signal over
		time.
	***************************************************************************/
	class EnvelopeFollower : public ModifierBase
	{
	private:

		// Members              ///////////////////////

			/// Tracking variable
		Math_t m_AU;
			/// Tracking variable
		Math_t m_BU;
			/// Tracking variables
		Math_t m_AD;
			/// Tracking variable
		Math_t m_BD;
			/// Previous sample
		StereoData m_X1;
			/// Previous sample
		StereoData m_Y1;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		EnvelopeFollower(EnvelopeFollower const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		EnvelopeFollower(EnvelopeFollower && other) = default;

		/*! ********************************************************************
		\brief
			Deconstructor.
		***********************************************************************/
		virtual ~EnvelopeFollower();

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		EnvelopeFollower & operator=(EnvelopeFollower const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		EnvelopeFollower & operator=(EnvelopeFollower && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Takes input sample and filters it, returning the result.

		\param input
			The input sample.

		\return
			The filtered sample.
		***********************************************************************/
		virtual StereoData FilterSample(StereoData const & input);

		/*! ********************************************************************
		\brief
			Returns boolean for if the object calling this function is a
			ModifierBase or not.

		\return
			True for this class, false for any derived class.
		***********************************************************************/
		virtual bool IsBase() { return false; };

			/// Add the factory as a friend so it can construct EnvelopeFollower objects
		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor. Creates a follower with upper and lower bounds to what
			frequencies it should follow.

		\param lower
			The lower bound of frequencies to follow.

		\param upper
			The upper bound of frequencies to follow.
		***********************************************************************/
		EnvelopeFollower(Math_t lower, Math_t upper);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();
	}; // class EnvelopeFollower

		/// Alias for a std::shared_ptr instantiated with the ModifierBase class
	TYPEDEF_SHARED(EnvelopeFollower);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __ENVELOPE_HPP
