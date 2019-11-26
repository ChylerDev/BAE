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

namespace OCAE
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

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Sawtooth(Sawtooth const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Sawtooth(Sawtooth && other) noexcept = default;

		virtual ~Sawtooth() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Sawtooth & operator=(Sawtooth const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Sawtooth & operator=(Sawtooth && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		void SetFrequency(Math_t freq);

		virtual StereoData SendSample(void);

		virtual bool IsBase() { return false; };

		friend class GeneratorFactory;

	private:

		// Functions                  ///////////////////////

		Sawtooth(Math_t freq);

		virtual Tools::MethodTable::MethodList_t CreateMethodList();

	}; // class Sawtooth
	TYPEDEF_SHARED(Sawtooth);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __SAWTOOTH_HPP
