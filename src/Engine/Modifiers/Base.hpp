/*! ****************************************************************************
\file             Base.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __MODIFIERS_BASE_HPP
#define __MODIFIERS_BASE_HPP

// Include Files                ////////////////////////////////////////////////

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Modifier
{

  using pBase_t = std::shared_ptr<class Base>;

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Base
  {
  private:

    // Members              ///////////////////////

    bool is_base;

  public:

    // Con-/De- structors   ///////////////////////

    Base(bool b) : is_base(b) {};
    virtual ~Base() = default;

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    /*! ************************************************************************
    \brief
      Takes input sample and filters it, returning the result.

    \param input
      The input sample.

    \return
      The filtered sample.
    ***************************************************************************/
    virtual StereoData_t FilterSample(StereoData_t const & input) { return input; };

    bool IsBase() { return is_base; };

    /*! ************************************************************************
    \brief
      Creates a modifier object and returns a pointer to it. This function
      checks to make sure the given type derives this base class.

    \tparam T
      The type to create a pointer to.

    \return
      The pointer to the new object.
    ***************************************************************************/
    template <class T, typename ...Args, std::enable_if_t<std::is_base_of_v<Base, T>, int> = 0>
    static pBase_t CreateModifier(Args &&... params)
    {
      return std::make_shared<T>(params...);
    };

  private:

    // Functions                  ///////////////////////

  }; // class Base

} // namespace Modifier
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __MODIFIERS_BASE_HPP
