/*! ****************************************************************************
\file             stub.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

#ifndef __STUB_HPP
#define __STUB_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

  class Recorder
  {
    private:

      // Members              ///////////////////////

      Track_t m_Track;

      std::deque<Track_t> m_NewSamples;

    public:

      template<typename ...Args>
      static inline Recorder_t Create(Args &&... params)
      {
        return std::make_shared<Recorder>(params...);
      }

      // Con-/De- structors   ///////////////////////

      Recorder();
      ~Recorder();

      // Operators            ///////////////////////

      // Accossors/Mutators   ///////////////////////

      // Functions            ///////////////////////

      void SendSamples(Track_t const & samples);

      Track_t GetRecording();

  };

  //static AudioEngine::Modifier::Vocoder v(4);

} // namespace Core
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __STUB_HPP
