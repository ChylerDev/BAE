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

  using pSound_t = std::shared_ptr<class Sound>;

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
        std::vector<pNode_t>
      >;

  private:

    // Members              ///////////////////////

    Graph_t m_NodeGraph;

    std::shared_ptr<StereoData_t> m_Output;

    float m_Gain;

  public:

    // Con-/De- structors   ///////////////////////

    Sound(float gain = DEFAULT_GAIN);

    ~Sound() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    Graph_t & GetGraph();
    Graph_t const & GetGraph() const;

    void SetOutputGain(float gain);

    // Functions            ///////////////////////

    void AddNode(
      pNode_t const & node,
      uint32_t pos,
      bool targets_output = false
    );

    StereoData_t GetSample();

    template <typename ...Args>
    static pSound_t Create(Args &&... params)
    {
      return std::make_shared<Sound>(params...);
    };

  private:

    // Functions                  ///////////////////////

  }; // class Sound

} // namespace Core
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUND_HPP
