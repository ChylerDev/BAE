/*! ****************************************************************************
\file             Delay.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __DELAY_HPP
#define __DELAY_HPP

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
		Delay filter.

		The delay value is a whole number for simple whole sample calculations.
	***************************************************************************/
	class Delay : public ModifierBase
	{
	private:

		// Members              ///////////////////////

			/// Delayed sample storage
		std::deque<StereoData> m_Delay;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Delay(Delay const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Delay(Delay && other) noexcept = default;

		virtual ~Delay() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Delay & operator=(Delay const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Delay & operator=(Delay && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Sets a new delay length.
			
			If the new delay is larger than the previous delay, 0 samples are
			inserted to the front of the delayed sample list.

		\param samples
			New delay length in samples.
		*******************************************************************************/
		void SetDelay(uint64_t samples);

		/*! ********************************************************************
		\brief
			Gets the current delay length.

		\return
			The delay length in samples.
		***********************************************************************/
		uint64_t GetDelay() const;

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
			False.
		***********************************************************************/
		virtual bool IsBase() { return false; };

			/// Add the factory as a friend so it can construct Delay objects
		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param samples
			The delay amount in samples.
		***********************************************************************/
		Delay(uint64_t samples);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();
	}; // class Delay

		/// Alias for a std::shared_ptr instantiated with the Delay class
	TYPEDEF_SHARED(Delay);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __DELAY_HPP
