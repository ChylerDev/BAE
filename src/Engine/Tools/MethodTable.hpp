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

#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Tools
{
	/*! ************************************************************************
	\brief
		The purpose of this class is to create a simple interface for calling
		methods from an object of an unknown type. For example, within OCAE
		you have a Sine object currently represented by a GeneratorBasePtr
		object. To call the Sine method to set the frequency you would utilize
		this class in the following manner:

		```c++
		GeneratorBasePtr obj = CreateSine(440);

		auto new_freq = 880;
		(*obj)("SetFrequency", &new_freq);
		```
	***************************************************************************/
	class MethodTable
	{
	public:

		using MethodTable_t = std::unordered_map<std::string, Void_fn>;

	protected:

		// Members              ///////////////////////

			/// Object mapping a string to a function
		MethodTable_t m_Table;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Default constructor
		***********************************************************************/
		MethodTable();

		/*! ********************************************************************
		\brief
			Consturctor.

		\param list
			List of tuples for mapping a string to a function to initialize the
			internal method table.
		***********************************************************************/
		MethodTable(std::vector<std::tuple<std::string, Void_fn>> const & list);

		virtual ~MethodTable() = default; ///< Default destructor.

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Calls a method. If the provided function name does not
			exist within the map an exception will be thrown by
			std::unordered_map. If the method is to return a value, the first
			parameter must be a reference to a value that will store the
			returned value.

		\tparam Args
			The arguments' types of the given method.

		\param fn
			The name of the method.

		\param args
			The parameters for the method.
		***********************************************************************/
		template<typename... Args>
		void CallMethod(std::string const & fn, Args... args)
		{
			using tuple = std::tuple<typename std::remove_reference_t<Args>...>;
			tuple params(args...);
			m_Table.at(fn)(reinterpret_cast<void*>(&params));
		}

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

	protected:

		// Functions            ///////////////////////

		void SetMethods(std::vector<std::tuple<std::string, Void_fn>> const & list);

	}; // class MethodTable
} // namespace Tools
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __METHODTABLE_HPP
