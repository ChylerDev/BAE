/*! ****************************************************************************
\file             Input.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __INPUT_HPP
#define __INPUT_HPP

// Include Files                ////////////////////////////////////////////////

#include <string>
#include <vector>

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
		Creates a container to hold the command-line options passed into main.

	\param argc
		The number of arguments.

	\param argv
		Pointer to the array of arguments.
	***************************************************************************/
	void InitOptions(int argc, char * argv[]);

	/*! ************************************************************************
	\brief
		Returns a const reference to string at the given index.

	\return
		The string at the given index.
	***************************************************************************/
	std::string const & GetOption(int index);
} // namespace Tools
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __INPUT_HPP
