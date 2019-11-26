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

namespace OCAE
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

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Triangle(Triangle const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Triangle(Triangle && other) noexcept = default;

		virtual ~Triangle() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Triangle & operator=(Triangle const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Triangle & operator=(Triangle && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		virtual StereoData SendSample(void);

		virtual bool IsBase() { return false; };

		void SetFrequency(Math_t freq);

		friend class GeneratorFactory;

	private:

		Triangle(Math_t freq);

		virtual Tools::MethodTable::MethodList_t CreateMethodList();

	}; // class Triangle
	TYPEDEF_SHARED(Triangle);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __TRIANGLE_HPP
