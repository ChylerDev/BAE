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

#include <type_traits>

#include "../Engine.hpp"

#include "../Tools/MethodTable.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
		/// Forward reference for friending
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
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		ModifierBase(ModifierBase const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		ModifierBase(ModifierBase && other) noexcept = default;

		/*! ********************************************************************
		\brief
			Default destructor.
		***********************************************************************/
		virtual ~ModifierBase() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		ModifierBase & operator=(ModifierBase const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		ModifierBase & operator=(ModifierBase && rhs) noexcept = default;

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

		/*! ********************************************************************
		\brief
			Default constructor.
		***********************************************************************/
		ModifierBase() : MethodTable() { RegisterMethods(CreateMethodList()); };

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList() { return {}; };
	}; // class ModifierBase

		/// Alias for a std::shared_ptr instantiated with the ModifierBase class
	TYPEDEF_SHARED(ModifierBase);
} // namespace Modifier
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __MODIFIERBASE_HPP
