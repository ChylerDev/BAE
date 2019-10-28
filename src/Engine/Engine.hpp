/*! ****************************************************************************
\file             Engine.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __ENGINE_HPP
#define __ENGINE_HPP

// Public Macros                ////////////////////////////////////////////////

// Include Files                ////////////////////////////////////////////////

#include "Macro.hpp"
#include "Types.hpp"
#include "Util.hpp"

// Kitchen Sink
#ifndef AE_BUILD
	#include "Core.hpp"
	#include "Generators.hpp"
	#include "Modifiers.hpp"
	#include "Sounds.hpp"
	#include "Tools.hpp"
#endif

#endif // __ENGINE_HPP
