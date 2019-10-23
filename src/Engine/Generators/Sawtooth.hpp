/*! ****************************************************************************
\file             Sawtooth.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __SAWTOOTH_HPP
#define __SAWTOOTH_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "GeneratorBase.hpp"

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
	class Sawtooth : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

		Math_t m_Irate;
		Math_t m_Inc;

		MethodTable_t m_Table;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~Sawtooth() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		void SetFrequency(Math_t freq);

		virtual StereoData SendSample(void);
		virtual void SendBlock(StereoData * buffer, uint64_t size);

		virtual MethodTable_t const & GetMethodTable() const;

	private:

		// Functions                  ///////////////////////

		Sawtooth(Math_t freq);
	}; // class Sawtooth
	TYPEDEF_SHARED(Sawtooth);
} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SAWTOOTH_HPP
