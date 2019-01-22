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

void CreateOptions(int argc, char * argv[]);

std::map<int, std::string> const & GetOptions();

// Public Functions             ////////////////////////////////////////////////

#endif // __INPUT_HPP
