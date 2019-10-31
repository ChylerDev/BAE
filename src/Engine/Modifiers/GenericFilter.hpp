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

		using ZeroContainer = std::vector<std::tuple<uint32_t,Math_t>>;
		using PoleContainer = std::vector<std::tuple<uint32_t,Math_t>>;

	private:

		using SampleContainer = std::deque<StereoData>;

		// Members              ///////////////////////

			/// Vector of tuples, tuple of the x subscript and its coefficient
		ZeroContainer m_Zeros;
			/// Vector of tuples, tuple of the y subscript and its coefficient
		PoleContainer m_Poles;

		SampleContainer m_Inputs;
		SampleContainer m_Outputs;

		uint32_t m_MaxXSubscript;
		uint32_t m_MaxYSubscript;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Default destructor
		***********************************************************************/
		virtual ~GenericFilter() = default;

		// Operators            ///////////////////////

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

		virtual bool IsBase() { return false; };

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

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList();

	}; // class GenericFilter
	TYPEDEF_SHARED(GenericFilter);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERIC_FILTER_HPP
