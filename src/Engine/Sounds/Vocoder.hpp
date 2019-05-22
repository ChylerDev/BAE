/*! ****************************************************************************
\file             Vocoder.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __VOCODER_HPP
#define __VOCODER_HPP

// Include Files                ////////////////////////////////////////////////

#include <vector>
#include <tuple>

#include "../Engine.hpp"

#include "Base.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Sounds
{

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Vocoder : public Base
  {
  private:

    using Carrier_t = Generator::Square;

    // Members              ///////////////////////

    std::vector<Math_t> m_CentralFrequencies;

    uint32_t m_BandCount;

    Math_t m_Mu;

  public:

    // Con-/De- structors   ///////////////////////

    Vocoder(Node_t const & base_input, int N=1, Math_t gain = DEFAULT_GAIN);

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

      // p is in cents
    void SetOffset(int32_t p);

    // Functions            ///////////////////////

    template<typename ...Args>
    static inline Vocoder_t Create(Args &&... params)
    {
      return std::make_shared<Vocoder>(params...);
    }

  private:

    // Functions                  ///////////////////////

    std::vector<ModBase_t> BPSetup();
    std::vector<ModBase_t> EnvSetup();
    std::vector<GenBase_t> OscSetup();

  }; // class Vocoder

} // namespace Sounds
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __VOCODER_HPP
