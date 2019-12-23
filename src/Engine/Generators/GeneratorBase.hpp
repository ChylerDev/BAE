/*! ****************************************************************************
\file             GeneratorBase.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __OCAE_GENERATORBASE_HPP
#define __OCAE_GENERATORBASE_HPP

// Include Files                ////////////////////////////////////////////////

#include <functional>
#include <unordered_map>
#include <string>

#include "../Engine.hpp"

#include "../Tools/MethodTable.hpp"
#include "GeneratorFactory.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	/*! ************************************************************************
	\brief
		General base class for all generator (sounds) to inherit from. Any
		derived classes with extra methods that may need to be acquired can be
		accessed through their setup of the Tools::MethodTable.
	***************************************************************************/
	class GeneratorBase: public Tools::MethodTable
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
		GeneratorBase(GeneratorBase const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		GeneratorBase(GeneratorBase && other) = default;

		/*! ********************************************************************
		\brief
			Default destructor.
		***********************************************************************/
		virtual ~GeneratorBase() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		GeneratorBase & operator=(GeneratorBase const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		GeneratorBase & operator=(GeneratorBase && rhs) = default;

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Calculates the sample. For the base class this is simply 0.

		\return
			The stereo sample data.
		***********************************************************************/
		virtual inline StereoData Process(void) { return StereoData(0.f, 0.f); };

		/*! ********************************************************************
		\brief
			Returns boolean for if the object is a GeneratorBase or not.

		\return
			True for this class, false for any derived class.
		***********************************************************************/
		virtual bool IsBase() { return true; };

			/// Add the factory as a friend so it can construct GeneratorBase objects
		friend class GeneratorFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.
		***********************************************************************/
		GeneratorBase() : MethodTable() { RegisterMethods(CreateMethodList()); };

		/*! ********************************************************************
		\brief
			Creates a vector containing the names of functions, and the callable
			functions themselves.

			See Tools::MethodTable documentation on more info about this system.

		\return
			The vector containing callable functions and their names as strings.
		***********************************************************************/
		virtual Tools::MethodTable::MethodList_t CreateMethodList() { return {}; };

	}; // class GeneratorBase

		/// Alias for a std::shared_ptr instantiated with the GeneratorBase class
	OCAE_TYPEDEF_SHARED(GeneratorBase);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __OCAE_GENERATORBASE_HPP
