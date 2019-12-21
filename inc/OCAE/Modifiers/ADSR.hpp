/*! ****************************************************************************
\file             ADSR.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __ADSR_HPP
#define __ADSR_HPP

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
		Attack - Decay - Sustain - Release filter.

		The most basic filter to create an envolpe over a given signal. The
		filter uses only linear slopes for the attack, decay, and release
		phases. The filter will only continue to the release phase when the
		ADSR::Release method is called.
	***************************************************************************/
	class ADSR : public ModifierBase
	{
	private:

		/*! ********************************************************************
		\brief
			Enum for tracking the current state of the ADSR envelope.
		***********************************************************************/
		enum class State : int8_t
		{
			attack,
			decay,
			sustain,
			release,
			invalid = -1,
		};

		// Members              ///////////////////////

			/// The rate of change in gain during the attack phase
		Math_t m_Attack;
			/// The rate of change in gain during the decay phase
		Math_t m_Decay;
			/// The gain level during the sustain phase
		Math_t m_Sustain;
			/// The rate of change in gain during the release phase
		Math_t m_Release;
			/// The current phase of the envelope
		State m_State;
			/// The current gain value updated during filtering
		Math_t m_Gain;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		ADSR(ADSR const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		ADSR(ADSR && other) = default;

		virtual ~ADSR() = default;	///< Default destructor.

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		ADSR & operator=(ADSR const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		ADSR & operator=(ADSR && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Will set the phase to the release phase, regardless of what the
			current phase is.
		***********************************************************************/
		void Release(void);

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

			/// Add the factory as a friend so it can construct ADSR objects
		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param attack
			Time to increase gain from 0 to 1 in samples.

		\param decay
			Time to decrease gain from 0 to sustain in samples.

		\param sustain
			The gain level of the sustain phase.

		\param release
			Time to decrease from sustain to 0 in samples.
		***********************************************************************/
		ADSR(uint64_t attack, uint64_t decay, Math_t sustain, uint64_t release);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();
	}; // class ADSR

		/// Alias for a std::shared_ptr instantiated with the ADSR class
	OCAE_TYPEDEF_SHARED(ADSR);
} // namespace Modifier
} // namespace OCAE

// Public FunctOCAE      ////////////////////////////////////////////////

#endif // __ADSR_HPP
