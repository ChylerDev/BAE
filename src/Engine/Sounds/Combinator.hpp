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

namespace AudioEngine
{
namespace Sound
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class Combinator
	{
	public:

		enum Combinations
		{
			Addition,
			Multiplication,
		};

	private:

		// Members              ///////////////////////

		Combinations m_Combination;

	public:

		// Con-/De- structors   ///////////////////////

		Combinator(Combinations c = Addition);

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		// Functions            ///////////////////////

		template<typename Iterator>
		StereoData Process(Iterator first, Iterator last)
		{
			StereoData output;

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
			}

			return output;
		}

	private:

		// Functions                  ///////////////////////

	}; // class Combinator
	TYPEDEF_SHARED(Combinator);
} // namespace Sound
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __COMBINATOR_HPP
