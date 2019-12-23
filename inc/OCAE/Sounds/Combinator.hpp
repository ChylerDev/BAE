/*! ****************************************************************************
\file             Combinator.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __COMBINATOR_HPP
#define __COMBINATOR_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Sound
{
	/*! ************************************************************************
	\brief
		This class allows for a modifyable way of combining a list of samples.
	***************************************************************************/
	class Combinator
	{
	public:

		/*! ********************************************************************
		\brief
			Enum defining the types of combinations that are possible.
		***********************************************************************/
		enum Combinations
		{
			Addition,
			Multiplication,
		};

	private:

		// Members              ///////////////////////

			/// The combination method used for this Combinator
		Combinations m_Combination;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Constructor.

		\param c
			The combination type for the object to use.
		***********************************************************************/
		Combinator(Combinations c = Addition);

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Processes the objects sequentially and either adds them or
			multiplies them depending on how the object was constructed.

		\tparam Iterator
			The iterator type to process. Must iterate over a container of
			StereoData values.

		\param first
			The beginning of the list.

		\param last
			The end of the list.

		\return
			The result of the Processing.
		***********************************************************************/
		template<typename Iterator>
		StereoData Process(Iterator first, Iterator last)
		{
			StereoData output(*first++);

			switch(m_Combination)
			{
				case Addition:
					while(first != last)
					{
						Left(output) += Left(*first);
						Right(output) += Right(*first);

						++first;
					}
					break;
				case Multiplication:
					while(first != last)
					{
						Left(output) *= Left(*first);
						Right(output) *= Right(*first);

						++first;
					}
					break;
				default:
					break;
			};

			return output;
		}

	private:

		// Functions                  ///////////////////////

	}; // class Combinator

		/// Alias for std::shared_ptr instantiated with Combinator.
	OCAE_TYPEDEF_SHARED(Combinator);
} // namespace Sound
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __COMBINATOR_HPP
