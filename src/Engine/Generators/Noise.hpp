/*! ****************************************************************************
\file             Noise.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __NOISE_HPP
#define __NOISE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include <random>

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
	class Noise : public GeneratorBase
	{
	private:

		// Members              ///////////////////////

		std::uniform_int_distribution<int16_t> m_Distribution;
		std::default_random_engine m_Engine;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Copy constructor. Deleted.

		\param other
			The other object to be copied.
		***********************************************************************/
		Noise(Noise const & other) = delete;

		/*! ********************************************************************
		\brief
			Default move constructor.

		\param other
			The other object to be moved.
		***********************************************************************/
		Noise(Noise && other) noexcept = default;

		virtual ~Noise() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator. Deleted.

		\param rhs
			The object to be copied.

		\return
			*this.
		***********************************************************************/
		Noise & operator=(Noise const & rhs) = delete;

		/*! ********************************************************************
		\brief
			Default move assignment operator.

		\param rhs
			The object to be moved.

		\return
			*this.
		***********************************************************************/
		Noise & operator=(Noise && rhs) noexcept = default;

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		virtual StereoData SendSample(void);

		virtual bool IsBase() { return false; };

		friend class GeneratorFactory;

	protected:

		// Functions                  ///////////////////////

		Noise();

		virtual Tools::MethodTable::MethodList_t CreateMethodList() { return {}; };

	}; // class Noise

		/// Alias for a std::shared_ptr instantiated with the Noise class
	TYPEDEF_SHARED(Noise);
} // namespace Generator
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __NOISE_HPP
