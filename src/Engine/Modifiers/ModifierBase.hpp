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
		The base Modifier class that all modifiers should inherit from.

		There are a few functions that should be overridden by derived classes,
		but are also implemented here for default behavior:
			* FilterSample
			* IsBase (This function will likely be removed in the future)
			* CreateMethodList

		See their individual documentation for more info.
	***************************************************************************/
	class ModifierBase: public Tools::MethodTable
	{
	private:

		// Members              ///////////////////////

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Default destructor, virtual for inheritence uses.
		***********************************************************************/
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

		/*! ********************************************************************
		\brief
			Returns boolean for if the object calling this function is a
			ModifierBase or not.

		\return
			True for this class, false for any derived class.
		***********************************************************************/
		virtual bool IsBase() { return true; };

			/// Add the factory as a friend so it can construct ModifierBase objects
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
