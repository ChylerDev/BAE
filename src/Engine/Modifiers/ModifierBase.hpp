/*! ****************************************************************************
\file             ModifierBase.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __MODIFIERBASE_HPP
#define __MODIFIERBASE_HPP

// Include Files                ////////////////////////////////////////////////

#include <cstring>

#include "../Engine.hpp"

#include "../Tools/MethodTable.hpp"
#include "ModifierFactory.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class ModifierBase: public Tools::MethodTable
	{
	private:

		// Members              ///////////////////////

		bool is_base;

	public:

		// Con-/De- structors   ///////////////////////

		ModifierBase(bool b) : is_base(b) {};
		virtual ~ModifierBase() = default;

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
		virtual StereoData_t FilterSample(StereoData_t const & input) { return input; };
		virtual void FilterBlock(StereoData_t * input, StereoData_t * output, uint64_t size)
		{ std::copy_n(input, size, output); };

		bool IsBase() { return is_base; };

		friend class ModifierFactory;

	private:

		// Functions                  ///////////////////////

	}; // class ModifierBase
	TYPEDEF_SHARED(ModifierBase);
} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __MODIFIERBASE_HPP
