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
#include <tuple>
#include <vector>

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Core
{
	/*! ************************************************************************
	\brief
	***************************************************************************/
	class Sound
	{

	public:

		using Graph_t =
			std::map<
				uint32_t,
				std::vector<Node_t>
			>;

	private:

		// Members              ///////////////////////

		Graph_t m_NodeGraph;

		Math_t m_Gain;

	public:

		template <typename ...Args>
		static inline Sound_t Create(Args &&... params)
		{
			return std::make_shared<Sound>(params...);
		}

		// Con-/De- structors   ///////////////////////

		Sound(Math_t gain = DEFAULT_GAIN);

		~Sound() = default;

		// Operators            ///////////////////////

		// Accossors/Mutators   ///////////////////////

		Graph_t & GetGraph();
		Graph_t const & GetGraph() const;

		Block_t & GetBlock(Node_t const &);
		Block_t const & GetBlock(Node_t const &) const;

		Sound & SetOutputGain(Math_t gain);

		// Functions            ///////////////////////

		Sound & AddBlock(
			Block_t const & block,
			uint32_t pos,
			bool targets_output = false
		);

		Sound & SetTarget(Block_t const & block, uint32_t pos_b, Block_t const & target, uint32_t pos_t);
		Sound & SetTarget(Node_t const & source, Node_t const & dest);

		void SendBlock(StereoData_t * output, uint64_t size);

	private:

		// Functions                  ///////////////////////

	}; // class Sound
} // namespace Core
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUND_HPP
