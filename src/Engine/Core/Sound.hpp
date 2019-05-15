/*! ****************************************************************************
\file             Sound.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __SOUND_HPP
#define __SOUND_HPP

// Include Files                ////////////////////////////////////////////////

#include <cstdint>

#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "../Engine.hpp"

#include "Node.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

  class Node;

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Sound
  {
  public:

    using Graph_t =
      std::map<
        uint32_t,
        std::vector<
          std::shared_ptr<Node>
        >
      >;

  private:

    // Members              ///////////////////////

    Graph_t m_NodeGraph;

    std::shared_ptr<StereoData_t> m_Output;

  public:

    // Con-/De- structors   ///////////////////////

    Sound();

    ~Sound() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    void AddNode(
      std::shared_ptr<Node> const & node,
      uint32_t pos,
      bool targets_output = false
    );

    StereoData_t GetSample();

    Graph_t & GetGraph();
    Graph_t const & GetGraph() const;

  private:

    // Functions                  ///////////////////////

  }; // class Sound

} // namespace Core
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUND_HPP
