/*! ****************************************************************************
\file             Base.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __SOUNDS_BASE_HPP
#define __SOUNDS_BASE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

#include "../Core/Sound.hpp"

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
  class Base
  {
  private:

    // Members              ///////////////////////

    bool m_IsBase;

  protected:

    // Members              ///////////////////////

    Sound_t m_Sound;

  public:

    template<typename T, typename ...Args,
      std::enable_if_t<std::is_base_of_v<Base, T>, int> = 0
    >
    static inline SoundsBase_t Create(Args &&... params)
    {
      return std::make_shared<T>(params...);
    }

    // Con-/De- structors   ///////////////////////

    Base(Math_t gain, bool b) : m_IsBase(b), m_Sound(Core::Sound::Create(gain)) {};
    virtual ~Base() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    bool IsBase() const { return m_IsBase; };

    virtual Sound_t & ToSound() { return m_Sound; };

  private:

    // Functions                  ///////////////////////

  }; // class stub

} // namespace Sounds
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __SOUNDS_BASE_HPP
