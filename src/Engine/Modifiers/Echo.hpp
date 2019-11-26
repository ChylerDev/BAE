/*! ****************************************************************************
\file             Echo.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __ECHO_HPP
#define __ECHO_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include <deque>

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
		Echo IIR filter. Uses output sample for echoing instead of input,
		creating an infinite impulse responce (IIR).

		The delay value between echos is a whole number for simple whole sample
		calculations.
	***************************************************************************/
	class Echo : public ModifierBase
	{
	private:

		// Members              ///////////////////////

			/// Filtered samples for continuous echo
		std::deque<StereoData> m_Echo;
			/// Decay ratio for the echo
		Math_t m_Ratio;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Echo(Echo const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Echo(Echo && other) noexcept = default;

		virtual ~Echo() = default; ///< Default destructor.

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Echo & operator=(Echo const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Echo & operator=(Echo && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Sets the decay ratio of the echo samples.

		\param decay_ratio
			The new decay ratio.
		***********************************************************************/
		void SetDecayRatio(Math_t decay_ratio);

		/*! ********************************************************************
		\brief
			Gets the decay ratio of the echo samples.

		\return
			The decay ratio.
		***********************************************************************/
		Math_t GetDecayRatio() const;

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Takes input sample and filters it, returning the result.

		\param input
			The input sample.

		\return
			The filtered sample.
		***********************************************************************/
		virtual StereoData FilterSample(StereoData const & sample);

		/*! ********************************************************************
		\brief
			Returns boolean for if the object calling this function is a
			ModifierBase or not.

		\return
			False.
		***********************************************************************/
		virtual bool IsBase() { return false; };

			/// Add the factory as a friend so it can construct Echo objects
		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param sample_delay
			The delay in samples between the input signal and it's first echo.

		\param decay_ratio
			The decay ratio of the echo samples.
		***********************************************************************/
		Echo(uint64_t sample_delay, Math_t decay_ratio);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();
	}; // class Echo

		/// Alias for a std::shared_ptr instantiated with the Echo class
	TYPEDEF_SHARED(Echo);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __ECHO_HPP
