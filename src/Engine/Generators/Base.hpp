/*! ****************************************************************************
\file             Base.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __GENERATORS__BASE_HPP
#define __GENERATORS__BASE_HPP

// Include Files                ////////////////////////////////////////////////

#include <functional>
#include <unordered_map>
#include <string>

#include "../Engine.hpp"

#include "../Tools/MethodTable.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Base: public Tools::MethodTable
  {
  private:

    // Members              ///////////////////////

    bool is_base;

  public:

    /*! ************************************************************************
    \brief
      Creates a generator object and returns a pointer to it. This function
      checks to make sure the given type derives this base class.

    \tparam T
      The type to create a pointer to.

    \return
      The pointer to the new object.
    ***************************************************************************/
    template <class T, typename ...Args, std::enable_if_t<std::is_base_of_v<Base, T>, int> = 0>
    static inline GenBase_t Create(Args &&... params)
    {
      return std::make_shared<T>(params...);
    }

    // Con-/De- structors   ///////////////////////

    Base(bool b) : is_base(b) {};
    virtual ~Base() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    virtual StereoData_t SendSample(void) { return StereoData_t(0.f, 0.f); };
    virtual void SendBlock(StereoData_t * buffer, uint64_t size) { UNREFERENCED_PARAMETER(buffer); UNREFERENCED_PARAMETER(size); };

    bool IsBase() { return is_base; };

  private:

    // Functions                  ///////////////////////

  }; // class Base

} // namespace Generator
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERATORS_BASE_HPP
