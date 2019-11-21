/*! ****************************************************************************
\file             LowPass.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __LOW_PASS_HPP
#define __LOW_PASS_HPP

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
		3rd Order Butterworth Low Pass filter with resonance.
	***************************************************************************/
	class LowPass : public ModifierBase
	{
	private:

		// Members              ///////////////////////

			/// Cutoff frequency
		Math_t m_Cutoff;
			/// Resonance
		Math_t m_Resonance;
			/// List of coefficients for the filter
		Math_t m_Coefficients[4];
			/// Previous outputs for future calculations
		StereoData m_Outputs[3];

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/

		LowPass(LowPass const & other) = delete;
		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		LowPass(LowPass && other) noexcept = default;

		/*! ********************************************************************
		\brief
			Destructor.
		***********************************************************************/
		virtual ~LowPass() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		LowPass & operator=(LowPass const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		LowPass & operator=(LowPass && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Sets the cutoff frequency of the filter.

		\param cutoff
			The cutoff frequency.
		***********************************************************************/
		void SetCutoff(Math_t cutoff);

		/*! ********************************************************************
		\brief
			Sets the resonance angle of the filter.

		\param resonance
			The resonance angle, in range [0,1/6]. No safety checks are
			performed.
		***********************************************************************/
		void SetResonance(Math_t resonance);

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

			/// Add the factory as a friend so it can construct LowPass objects
		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param cutoff
			The cutoff frequency in Hz.

		\param resonance
			The resonance angle of the filter, value can be in range [0,1/6]. No
			safety checks are performed.
		***********************************************************************/
		LowPass(Math_t cutoff, Math_t resonance);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();

		/*! ********************************************************************
		\brief
			Resets the values of the object. Called during construction,
			SetCutoff, and SetResonance.
		***********************************************************************/
		void Reset();
	}; // class LowPass

		/// Alias for a std::shared_ptr instantiated with the LowPass class
	TYPEDEF_SHARED(LowPass);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __LOW_PASS_HPP
