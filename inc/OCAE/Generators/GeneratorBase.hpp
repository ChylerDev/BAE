/*! ****************************************************************************
\file             GeneratorBase.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __GENERATORBASE_HPP
#define __GENERATORBASE_HPP

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

		virtual ~GeneratorBase() = default; ///< Default destructor.

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Calculates the sample. For the base class this is simply 0.

		\return
			0.
		***********************************************************************/
		virtual StereoData SendSample(void) { return StereoData(0.f, 0.f); };

		/*! ********************************************************************
		\brief
			Returns boolean for if the object is a GeneratorBase or not.

		\return
			True if the object is a GeneratorBase
		***********************************************************************/
		virtual bool IsBase() { return true; };

		friend class GeneratorFactory;

	protected:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.
		***********************************************************************/
		GeneratorBase() : MethodTable(CreateMethodList()) {};

		virtual std::vector<std::tuple<std::string, Void_fn>> CreateMethodList() { return {}; };

	}; // class GeneratorBase

		/// Alias for a std::shared_ptr instantiated with the GeneratorBase class
	TYPEDEF_SHARED(GeneratorBase);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERATORBASE_HPP