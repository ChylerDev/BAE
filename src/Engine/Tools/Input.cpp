/*! ****************************************************************************
\file             Input.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Input.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{
  static std::map<int, std::string> s_Options;
} // namespace Tools
} // namespace AudioEngine

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Tools
{

  void CreateOptions(int argc, char * argv[])
  {
    for(int i = 0; i < argc; ++i)
    {
      s_Options[i] = argv[i];
    }
  }

  std::map<int, std::string> const & GetOptions()
  {
    return s_Options;
  }

} // namespace Tools
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
