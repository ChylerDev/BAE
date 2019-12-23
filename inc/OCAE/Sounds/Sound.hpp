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
		/// Forwarded alias of std::shared_ptr instantiated with Sound.
	OCAE_TYPEDEF_SHARED(Sound);
}
namespace Core
{
	class Driver;
		/// Forwarded alias of std::shared_ptr instantiated with Driver.
	OCAE_TYPEDEF_SHARED(Driver);
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
		way in conjunction with a Driver.
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
					/// Wrapper around the Block or Sound to abstract it away.
				std::any block;
					/// Bool that states whether the block member is a Sound or Block.
				bool const is_sound;

				/*! ************************************************************
				\brief
					Constructs an E_Block from a Sound object.

				\param s
					The Sound object.
				***************************************************************/
				E_Block(SoundPtr const & s);

				/*! ************************************************************
				\brief
					Constructs an E_Block from a Block obect.

				\param b
					The Block object.
				***************************************************************/
				E_Block(BlockPtr const & b);

				E_Block(E_Block const & other) = default;     ///< Default copy constructor. \param other The object being copied.
				E_Block(E_Block && other) = default; ///< Default move constructor. \param other The object being moved.
				~E_Block() = default;                         ///< Default destructor.

				E_Block & operator=(E_Block const & rhs) = default;     ///< Default copy assignment operator. \param rhs The object being copied. \return *this.
				E_Block & operator=(E_Block && rhs) = default; ///< Default move assignment operator. \param rhs The object being moved. \return *this.
			};
				/// Alias for std::shared_ptr instantiated with E_Block.
			OCAE_TYPEDEF_SHARED(E_Block);

				/// The input blocks for this edge
			std::deque<E_BlockPtr> inputs;
				/// The method of combining
			Combinator combinator;
				/// The output blocks for this edge
			std::deque<E_BlockPtr> outputs;

			/*! ****************************************************************
			\brief
				Constructs an Edge object from the given components.

			\param in
				The list of input blocks.

			\param comb
				The Combinator defining how the outputs of the processed inputs
				should be defined.

			\param out
				The list of output blocks, whose inputs are primed with the
				output of the Combinator.
			*******************************************************************/
			Edge(std::deque<E_BlockPtr> const & in,
			     Combinator const & comb,
			     std::deque<E_BlockPtr> const & out);

			/*! ****************************************************************
			\brief
				Processes the edge.
			*******************************************************************/
			void Process();

			/*! ****************************************************************
			\brief
				Primes the input of this edge.

			\param in
				The input sample.
			*******************************************************************/
			void PrimeInput(StereoData in);
		};
			/// Alias for std::shared_ptr instantiated with Edge.
		OCAE_TYPEDEF_SHARED(Edge);

			/// Alias for the structure that represents the graph blocks that make up this Sound.
		using Graph = std::deque< EdgePtr >;

	private:

		// Members              ///////////////////////

			/// The graph of blocks
		Graph m_Graph;

			/// Input gain modifier.
		Modifier::ModifierBasePtr m_InputGain;
			/// Output gain modifier.
		Modifier::ModifierBasePtr m_OutputGain;

			/// Input sample
		StereoData m_Input;
			/// Output sample
		StereoData m_Output;

			/// Driver the Sound is registered with
		Core::DriverPtr m_Driver;
			/// The ID of this Sound within the Driver
		uint64_t m_ID;

			/// Controls if the sound will Process
		bool m_IsPaused;

	public:

		// Con-/De- structors   ///////////////////////

		/*! ********************************************************************
		\brief
			Default constructor.

		\param input_gain
			The gain for the input samples.

		\param output_gain
			The gain for the output samples.
		***********************************************************************/
		Sound(Math_t input_gain = Math_t(1.0), Math_t output_gain = OCAE_DEFAULT_GAIN);

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

		~Sound() = default; ///< Default destructor.

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

		/*! ********************************************************************
		\brief
			Returns a reference to the internal graph for direct modification of
			the structure.

		\return
			A reference to the internal graph.
		***********************************************************************/
		Graph & GetGraph();

		/*! ********************************************************************
		\brief
			Returns a reference to the internal graph for direct modification of
			the structure.

		\return
			A reference to the internal graph.
		***********************************************************************/
		Graph const & GetGraph() const;

		/*! ********************************************************************
		\brief
			Sets the gain for the input.

		\param gain
			The new gain.
		***********************************************************************/
		void SetInputGain(Math_t gain);

		/*! ********************************************************************
		\brief
			Sets the gain for the output.

		\param gain
			The new gain.
		***********************************************************************/
		void SetOutputGain(Math_t gain);

		/*! ********************************************************************
		\brief
			Pauses the processing of this sound.
		***********************************************************************/
		void Pause();

		/*! ********************************************************************
		\brief
			Unpauses the processing of this sound.
		***********************************************************************/
		void Unpause();

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Primes the input for the next processing round.

		\param input
			The input to be processed.
		***********************************************************************/
		void PrimeInput(StereoData input);

		/*! ********************************************************************
		\brief
			Processes audio configured in the internal graph, storing the output
			internally.
		***********************************************************************/
		void Process();

		/*! ********************************************************************
		\brief
			Returns the output from the previous round of processing.

		\return
			The most recent output sample.
		***********************************************************************/
		StereoData LastOutput();

		/*! ********************************************************************
		\brief
			Generates an Edge for the graph from the given lists of input
			blocks, output blocks, and a Combinator defining how the blocks
			should be combined.

		\param in
			List of input blocks.

		\param comb
			Combinator. See Combinator documentation for more info.

		\param out
			List of output blocks.

		\return
			The generated Edge object wrapped in a std::shared_ptr.
		***********************************************************************/
		static EdgePtr CreateEdge(std::deque<Edge::E_BlockPtr> const & in,
		                          Combinator const & comb,
		                          std::deque<Edge::E_BlockPtr> const & out);

		/*! ********************************************************************
		\brief
			Creates an E_Block object from the given Sound.

		\param sound
			The Sound to be wrapped in an E_Block. Cannot be the same Sound
			object as the object the E_Block is added to or else endless
			recursion will occur.

		\return
			The new E_Block object wrapped in a std::shared_ptr.
		***********************************************************************/
		static Edge::E_BlockPtr CreateE_Block(SoundPtr const & sound);

		/*! ********************************************************************
		\brief
			Creates an E_Block object from the given Block.

		\param block
			The Block to be wrapped in the E_Block.

		\return
			The new E_Block object wrapped in a std::shared_ptr.
		***********************************************************************/
		static Edge::E_BlockPtr CreateE_Block(BlockPtr const & block);

		/*! ********************************************************************
		\brief
			Registers the given Sound object with the given Driver. If this
			Sound is already registered to a Driver, it will unregister itself
			before registering to the new Driver.

		\param self
			The Sound object to register to the given Driver.

		\param driver
			The Driver the given Sound object will be registered to.
		***********************************************************************/
		static void Register(SoundPtr const & self, Core::DriverPtr const & driver);

		/*! ********************************************************************
		\brief
			Unregisters the given Sound object from it's registered Driver.

		\param self
			The Sound object to unregister.
		***********************************************************************/
		static void Unregister(SoundPtr const & self);

	private:

		// Functions                  ///////////////////////

		/*! ********************************************************************
		\brief
			Ensures that the internal graph contains only unique objects. If an
			object is not unique, it will create a copy of the object.
		***********************************************************************/
		void MakeUnique();

	}; // class Sound
} // namespace Sound
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUND_HPP
