/*! ****************************************************************************
\file             Equalizer.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_EQUALIZER_HPP
#define __OCAE_EQUALIZER_HPP

// Include Files                ////////////////////////////////////////////////

#include <vector>

#include "../Engine.hpp"

#include "BandPass.hpp"
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
		Equalizer filter.

		This filter splits a given signal across bands, using Modifier::BandPass
		objects to do so, then amplifies each band by a given gain before
		combining the bands again for the final output.
	***************************************************************************/
	class Equalizer : public ModifierBase
	{
	private:

		// Members              ///////////////////////

			/// List of gains for each frequency band
		std::vector<Math_t> m_BandGains;
			/// List of band pass filters for each frequency band
		std::vector<BandPassPtr> m_Bands;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Equalizer(Equalizer const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Equalizer(Equalizer && other) = default;

		/*! ********************************************************************
		\brief
			Default destructor.
		***********************************************************************/
		virtual ~Equalizer() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Equalizer & operator=(Equalizer const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Equalizer & operator=(Equalizer && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		/*! ********************************************************************
		\brief
			Sets the gain for a given frequency band.

		\param band
			The frequency band to set the gain of.

		\param gain
			The new gain.
		***********************************************************************/
		void SetGain(uint32_t band, Math_t gain);

		/*! ********************************************************************
		\brief
			Gets the gain from a given frequency band.

		\param band
			The frequency band to get the gain from.

		\return

		***********************************************************************/
		Math_t GetGain(uint32_t band);

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

			/// Add the factory as a friend so it can construct Equalizer objects
		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param band_count
			The number of frequency bands for the equalizer.

		\param lower
			The lowest frequency of the lowest band pass filter (not the central
			frequency).

		\param upper
			The highest frequency of the highest band pass filter (not the
			central frequency).
		***********************************************************************/
		Equalizer(uint32_t band_count, Math_t lower, Math_t upper);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList();
	}; // class Equalizer

		/// Alias for a std::shared_ptr instantiated with the Equalizer class
	OCAE_TYPEDEF_SHARED(Equalizer);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __OCAE_EQUALIZER_HPP
