/*! ****************************************************************************
\file             GenericFilter.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __GENERIC_FILTER_HPP
#define __GENERIC_FILTER_HPP

// Include Files                ////////////////////////////////////////////////

#include <tuple>
#include <vector>
#include <deque>

#include "../Engine.hpp"

#include "Base.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Modifiers
{

  #pragma region
  /*! **************************************************************************
  \brief
    Generic audio filter with simple poles.
  *****************************************************************************/
  class GenericFilter : public Base
  {
  public:

    using ZeroContainer = std::vector<std::tuple<uint32_t,float>>;
    using PoleContainer = std::vector<std::tuple<uint32_t,float>>;

  private:
    
    using SampleContainer = std::deque<StereoData_t>;

    // Members              ///////////////////////

      /// Vector of tuples, tuple of the x subscript and its coefficient
    ZeroContainer m_Zeros;
      /// Vector of tuples, tuple of the y subscript and its coefficient
    PoleContainer m_Poles;

    SampleContainer m_Inputs;
    SampleContainer m_Outputs;

    uint32_t m_MaxXSubscript;
    uint32_t m_MaxYSubscript;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Constructor.

    \param zeros
      Container a tuple of the x subscript and its coefficient.
      Expected to be ordered lowest to highest by subscript.

    \param poles
      Container of a tuple of the the y subscript and its coefficient.
      Expected to be ordered lowest to highest by subscript.
    ***************************************************************************/
    GenericFilter(ZeroContainer const & zeros, PoleContainer const & poles);

    /*! ************************************************************************
    \brief
      Default destructor
    ***************************************************************************/
    virtual ~GenericFilter() = default;

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
    virtual StereoData_t FilterSample(StereoData_t input);

  private:

    // Functions                  ///////////////////////

  }; // class GenericFilter
  #pragma endregion

} // namespace Modifiers
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERIC_FILTER_HPP
