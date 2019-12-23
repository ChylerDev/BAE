/*! ****************************************************************************
\file             BandPass.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_BAND_PASS_HPP
#define __OCAE_BAND_PASS_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "ModifierBase.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	class ModifierFactory;
	class Equalizer;
}
}

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	/*! ************************************************************************
	\brief
		Bandpass filter.
	***************************************************************************/
	class BandPass : public ModifierBase
	{
	private:

		// Members              ///////////////////////

			/// The central frequency
		Math_t m_CentralFrequency;
			/// The quality
		Math_t m_Quality;
			/// The xn and xn-2 coefficient
		Math_t m_A0;
			/// The yn-1 coefficient
		Math_t m_B1;
			/// The yn-2 coefficient
		Math_t m_B2;
			/// The xn-1 sample
		StereoData m_X1;
			/// The xn-2 sample
		StereoData m_X2;
			/// The yn-1 sample
		StereoData m_Y1;
			/// The yn-2 sample
		StereoData m_Y2;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		BandPass(BandPass const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		BandPass(BandPass && other) = default;

		virtual ~BandPass();	///< Default destructor.

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		BandPass & operator=(BandPass const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		BandPass & operator=(BandPass && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Returns the central frequency of the filter.

		\return
			The central frequency.
		***********************************************************************/
		Math_t GetFrequency() const;

		/*! ********************************************************************
		\brief
			Sets the central frequency of the filter.

		\param f
			The new central frequency.
		***********************************************************************/
		void SetFrequency(Math_t f);

		/*! ********************************************************************
		\brief
			Returns the quality of the filter.

		\return
			The quality.
		***********************************************************************/
		Math_t GetQuality() const;

		/*! ********************************************************************
		\brief
			Sets the quality of the filter.

		\param Q
			The new quality.
		***********************************************************************/
		void SetQuality(Math_t Q);

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Takes input sample and filters it, returning the result.

		\param input
			The input sample.

		\return
			The filtered sample.
		***********************************************************************/
		virtual StereoData Process(StereoData const & input);

		/*! ********************************************************************
		\brief
			Returns boolean for if the object calling this function is a
			ModifierBase or not.

		\return
			False.
		***********************************************************************/
		virtual bool IsBase() { return false; };

			/// Add the factory as a friend so it can construct BandPass objects
		friend class ModifierFactory;

			/// Add the Equalizer filter as a friend so it can construct BandPass objects
		friend class Equalizer;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param f
			The central frequency.

		\param Q
			The filter quality.
		***********************************************************************/
		BandPass(Math_t f, Math_t Q = 1);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();

		/*! ********************************************************************
		\brief
			Resets the filters values in response to a change in the object's
			parameters.
		***********************************************************************/
		void Reset(void);
	}; // class BandPass

		/// Alias for a std::shared_ptr instantiated with the BandPass class
	OCAE_TYPEDEF_SHARED(BandPass);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __OCAE_BAND_PASS_HPP
