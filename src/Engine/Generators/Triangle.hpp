/*! ****************************************************************************
\file             Triangle.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __TRIANGLE_HPP
#define __TRIANGLE_HPP

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
	class Triangle : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

		Math_t m_Irate;
		Math_t m_Inc;

		MethodTable_t m_Table;

	public:

		// Con-/De- structors   ///////////////////////

		virtual ~Triangle() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		void SetFrequency(Math_t freq);

		virtual StereoData SendSample(void);
		virtual void SendBlock(StereoData * buffer, uint64_t size);

		virtual MethodTable_t const & GetMethodTable() const;

	private:

		Triangle(Math_t freq);

	}; // class Triangle
	TYPEDEF_SHARED(Triangle);
} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __TRIANGLE_HPP
