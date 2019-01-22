/*! ****************************************************************************
\file             Input.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __INPUT_HPP
#define __INPUT_HPP

// Include Files                ////////////////////////////////////////////////

#include <map>
#include <string>

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

/*! ****************************************************************************
\brief
  Creates a container to hold the command-line options passed into main.

\param argc
  The number of arguments.

\param argv
  Pointer to the array of arguments.
*******************************************************************************/
void CreateOptions(int argc, char * argv[]);

/*! ****************************************************************************
\brief
  Returns a const reference to the map containing the options.

  The strings are mapped to their corresponding index from the command-line, so
  index 0 will still be the program name.

  Since the map is const, it cannot be accessed through std::map::operator[],
  instead, it can be accessed through std::map::at, which takes a key just like
  the index operator.

\return
  The string mapped to the corresponding command-line index.
*******************************************************************************/
std::map<int, std::string> const & GetOptions();

// Public Functions             ////////////////////////////////////////////////

#endif // __INPUT_HPP
