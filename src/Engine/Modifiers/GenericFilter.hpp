/*! ****************************************************************************
\file             GenericFilter.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __GENERIC_FILTER_HPP
#define __GENERIC_FILTER_HPP

// Include Files                ////////////////////////////////////////////////

#include <tuple>
#include <vector>
#include <deque>

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
		Generic audio filter with simple poles.
	***************************************************************************/
	class GenericFilter : public ModifierBase
	{
	public:

			/// Container used for coefficients of zeros of a filter
		using ZeroContainer = std::vector<std::tuple<uint32_t,Math_t>>;
			/// Container used for coefficients of poles of a filter
		using PoleContainer = std::vector<std::tuple<uint32_t,Math_t>>;

	private:

			/// Container used for the previous outputs and inputs of the filter
		using SampleContainer = std::deque<StereoData>;

		// Members              ///////////////////////

			/// Vector of tuples, tuple of the x subscript and its coefficient
		ZeroContainer m_Zeros;
			/// Vector of tuples, tuple of the y subscript and its coefficient
		PoleContainer m_Poles;

			/// Previous inputs to the filter
		SampleContainer m_Inputs;
			/// Previous outputs to the filter
		SampleContainer m_Outputs;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		GenericFilter(GenericFilter const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		GenericFilter(GenericFilter && other) noexcept = default;

		/*! ********************************************************************
		\brief
			Destructor
		***********************************************************************/
		virtual ~GenericFilter() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Assignment operator. Deleted.

		\param rhs
			The object to copy.

		\return
			*this.
		***********************************************************************/
		GenericFilter & operator=(GenericFilter const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		GenericFilter & operator=(GenericFilter && rhs) noexcept = default;

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
			False.
		***********************************************************************/
		virtual bool IsBase() { return false; };

			/// Add the factory as a friend so it can construct GenericFilter objects
		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param zeros
			Container a tuple of the x subscript and its coefficient.
			Expected to be ordered lowest to highest by subscript.

		\param poles
			Container of a tuple of the the y subscript and its coefficient.
			Expected to be ordered lowest to highest by subscript.
		***********************************************************************/
		GenericFilter(ZeroContainer const & zeros, PoleContainer const & poles);

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();
	}; // class GenericFilter

		/// Alias for a std::shared_ptr instantiated with the GenericFilter class
	TYPEDEF_SHARED(GenericFilter);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERIC_FILTER_HPP
