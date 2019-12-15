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

#include <deque>
#include <map>
#include <memory>

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
	TYPEDEF_SHARED(Sound);
}
namespace Core
{
	class Driver;
		/// Forwarded alias of std::shared_ptr instantiated with Driver.
	TYPEDEF_SHARED(Driver);
}
namespace Modifier
{
	class Gain;
		/// Forwarded alias of std::shared_ptr instantiated with Gain.
	TYPEDEF_SHARED(Gain);
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
		using BlockList = std::deque<BlockPtr>;
			/// Alias for the structure that represents the graph blocks that make up this Sound.
		using Graph = std::map<BlockPtr,BlockList>;

	private:

		// Members              ///////////////////////

			/// The graph of blocks
		Graph m_Graph;
		std::deque<BlockPtr> m_ProcessOrder;

			/// Input gain modifier.
		BlockPtr m_InputGain;
			/// Output gain modifier.
		BlockPtr m_OutputGain;

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
		Sound(Math_t input_gain = Math_t(1.0), Math_t output_gain = DEFAULT_GAIN);

		/*! ********************************************************************
		\brief
			Deleted copy constructor.

		\param other
			The other sound being copied.
		***********************************************************************/
		Sound(Sound const & other) = delete;

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
			Deleted copy assignment operator.

		\param rhs
			The sound being copied.

		\return
			*this.
		***********************************************************************/
		Sound & operator=(Sound const & rhs) = delete;

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

		BlockPtr const & GetInputBlock() const;

		BlockPtr const & GetOutputBlock() const;

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

		void AddConnection(BlockPtr const & from, BlockPtr const & to);

		void RemoveConnection(BlockPtr const & from, BlockPtr const & to);

		// Functions            ///////////////////////

		/*! ********************************************************************
		\brief
			Processes audio configured in the internal graph, storing the output
			internally.
		***********************************************************************/
		StereoData Process(StereoData input);

		/*! ********************************************************************
		\brief
			Effectively moves the other object into this one, with this sound
			coming before the passed sound.
		
		\param other
			Pointer to the other sound to concatenate with.
			NOTE: After this function, other's object will be empty, including
			      being unregistered. This' registration will be left unchanged.
		***********************************************************************/
		void Concat(SoundPtr && other);

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

		void ProcessOrder();

		void PrepareGraph(BlockList const &, BlockList &);
	}; // class Sound
} // namespace Sound
} // namespace OCAE

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUND_HPP
