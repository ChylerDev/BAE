/*! ****************************************************************************
\file             Sound.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __SOUND_HPP
#define __SOUND_HPP

// Include Files                ////////////////////////////////////////////////

#include <any>
#include <deque>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "../Engine.hpp"

#include "Combinator.hpp"
#include "Block.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

namespace OCAE
{
namespace Sound
{
	class Sound;
	TYPEDEF_SHARED(Sound);
}
namespace Core
{
	class Driver;
	TYPEDEF_SHARED(Driver);
}
}

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace OCAE
{
namespace Sound
{
	/*! ************************************************************************
	\brief
		Class for handling Generator and Modifier objects in a more abstract
		way.
	***************************************************************************/
	class Sound
	{
	public:
		/*! ********************************************************************
		\brief
			Structure representing the edges of the graph that defines a Sound.
		***********************************************************************/
		struct Edge
		{
			/*! ****************************************************************
			\brief
				Structure to abstract away the node of the Sound graph, allowing
				for sounds and blocks to make up a sound.
			*******************************************************************/
			struct E_Block
			{
				std::any block;
				bool const is_sound;

				E_Block(SoundPtr const & s);
				E_Block(BlockPtr const & b);
				E_Block(E_Block const &) = default;
				E_Block(E_Block &&) noexcept = default;
				~E_Block() = default;
			};
			TYPEDEF_SHARED(E_Block);

			std::deque<E_BlockPtr> inputs;
			Combinator combinator;
			std::deque<E_BlockPtr> outputs;

			Edge(std::deque<E_BlockPtr> const & in,
			     Combinator const & comb,
			     std::deque<E_BlockPtr> const & out);

			void Process();
			void PrimeInput(StereoData in);
		};
		TYPEDEF_SHARED(Edge);

		using Graph = std::deque< EdgePtr >;

	private:

		// Members              ///////////////////////

		Graph m_Graph;

		Modifier::ModifierBasePtr m_InputGain;
		Modifier::ModifierBasePtr m_OutputGain;

		StereoData m_Input;
		StereoData m_Output;

		Core::DriverPtr m_Driver;
		uint64_t m_ID;
		bool m_IsPaused;

	public:

		// Con-/De- structors   ///////////////////////

		Sound(Math_t input_gain = Math_t(1.0), Math_t output_gain = DEFAULT_GAIN);

		/*! ********************************************************************
		\brief
			Copy constructor.
			NOTE: The contsructed sound will not be registered to a driver, even
				  if the sound being copied is.

		\param other
			The other sound being copied
		***********************************************************************/
		Sound(Sound const & other);

		/*! ********************************************************************
		\brief
			Move constructor.
			NOTE: The constructed sound will not be registered to a driver, even
				  if the sound being moved is.

		\param other
			The other sound being moved.
		***********************************************************************/
		Sound(Sound && other) noexcept;

		~Sound() = default;

		// Operators            ///////////////////////

		/*! ********************************************************************
		\brief
			Copy assignment operator.
			NOTE: The copied sound will not change it's registration. If it
				  needs to be registered to a different driver, you must handle
				  that yourself.

		\param rhs
			The sound being copied.

		\return
			*this.
		***********************************************************************/
		Sound & operator=(Sound const & rhs);

		/*! ********************************************************************
		\brief
			Move assignment operator.
			NOTE: The moved sound will not change it's registration. If it
				  needs to be registered to a different driver, you must handle
				  that yourself.

		\param rhs
			The sound being moved.

		\return
			*this.
		***********************************************************************/
		Sound & operator=(Sound && rhs) noexcept;

		// Accossors/Mutators   ///////////////////////

		Graph & GetGraph();
		Graph const & GetGraph() const;

		Sound & SetInputGain(Math_t gain);
		Sound & SetOutputGain(Math_t gain);

		void Pause();
		void Unpause();

		// Functions            ///////////////////////

		void PrimeInput(StereoData);
		void Process();
		StereoData LastOutput();

		static EdgePtr CreateEdge(std::deque<Edge::E_BlockPtr> const & in,
		                          Combinator const & comb,
		                          std::deque<Edge::E_BlockPtr> const & out);
		static Edge::E_BlockPtr CreateE_Block(SoundPtr const &);
		static Edge::E_BlockPtr CreateE_Block(BlockPtr const &);

		static void Register(SoundPtr const & self, Core::DriverPtr const & driver);
		static void Unregister(SoundPtr const & self);

	private:

		// Functions                  ///////////////////////

		void MakeUnique();

	}; // class Sound
} // namespace Sound
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUND_HPP
