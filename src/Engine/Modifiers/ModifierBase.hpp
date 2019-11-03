/*! ****************************************************************************
\file             ModifierBase.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  OCAE

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __MODIFIERBASE_HPP
#define __MODIFIERBASE_HPP

// Include Files                ////////////////////////////////////////////////

#include <cstring>

#include "../Engine.hpp"

#include "../Tools/MethodTable.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	class ModifierFactory;
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
	***************************************************************************/
	class ModifierBase: public Tools::MethodTable
	{
	private:

		// Members              ///////////////////////

	public:

		// Con-/De- structors   ///////////////////////

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
		virtual StereoData FilterSample(StereoData const & input) { return input; };

		virtual bool IsBase() { return true; };

		friend class ModifierFactory;

	protected:

		// Functions                  ///////////////////////

		ModifierBase() : MethodTable(CreateMethodList()) {};

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList() { return {}; };

	}; // class ModifierBase
	TYPEDEF_SHARED(ModifierBase);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __MODIFIERBASE_HPP
