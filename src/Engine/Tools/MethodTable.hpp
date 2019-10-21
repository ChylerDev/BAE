/*! ****************************************************************************
\file             MethodTable.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __METHODTABLE_HPP
#define __METHODTABLE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{
	/*! ************************************************************************
	\brief
		For functions that take multiple parameters, the values should be packed
		in a tuple. If the function also returns a value, that should be the
		first element in the tuple, and thus can always be obtained by calling
		std::get<0> on the packed values.
	***************************************************************************/
	class MethodTable
	{
	protected:

		// Members              ///////////////////////

		MethodTable_t s_Table;

	public:

		// Con-/De- structors   ///////////////////////

		MethodTable();
		virtual ~MethodTable() = default;

		// Operators            ///////////////////////

		virtual Void_fn operator[](std::string const &) const;

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

	private:

		// Functions                  ///////////////////////

	}; // class MethodTable
} // namespace Tools
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __METHODTABLE_HPP
