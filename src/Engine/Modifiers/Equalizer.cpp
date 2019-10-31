/*! ****************************************************************************
\file             Equalizer.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Equalizer.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Modifier
{
	Equalizer::Equalizer(int band_count, Math_t lower, Math_t upper)
	{
		SetMethods(CreateMethodList());
	}
} // namespace Modifier
} // namespace OCAE

// Private Functions                      //////////////////////////////////////
