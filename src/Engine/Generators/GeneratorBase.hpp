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

namespace AudioEngine
{
namespace Generator
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class GeneratorBase: public Tools::MethodTable
	{
	private:

		// Members              ///////////////////////

		bool is_base;

	public:

		// Con-/De- structors   ///////////////////////

		GeneratorBase(bool b) : is_base(b) {};
		virtual ~GeneratorBase() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		virtual StereoData_t SendSample(void) { return StereoData_t(0.f, 0.f); };
		virtual void SendBlock(StereoData_t * buffer, uint64_t size) { UNREFERENCED_PARAMETER(buffer); UNREFERENCED_PARAMETER(size); };

		bool IsBase() { return is_base; };

		friend class GeneratorFactory;

	private:

		// Functions                  ///////////////////////

	}; // class GeneratorBase

	TYPEDEF_SHARED(GeneratorBase);
} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERATORBASE_HPP
