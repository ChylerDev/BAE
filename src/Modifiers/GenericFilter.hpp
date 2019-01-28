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

#include "../Engine.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace Modifiers
{

  #pragma region
  /*! **************************************************************************
  \brief
    Generic audio filter with simple poles.
  *****************************************************************************/
  class GenericFilter
  {
  public:

    using ZeroContainer = std::vector<std::tuple<uint32_t,float>>;
    using PoleContainer = std::vector<std::tuple<uint32_t,float>>;

  private:

    // Members              ///////////////////////

      /// Vector of tuples, tuple of the x subscript and its coefficient
    ZeroContainer m_Zeros;
      /// Vector of tuples, tuple of the y subscript and its coefficient
    PoleContainer m_Poles;

    std::vector<StereoData_t> m_Inputs;
    std::vector<StereoData_t> m_Outputs;

    uint32_t m_MaxXSubscript;
    uint32_t m_MaxYSubscript;

  public:

    // Con-/De- structors   ///////////////////////

    GenericFilter(ZeroContainer const & zeros, PoleContainer const & poles);

    ~GenericFilter();

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    // Functions            ///////////////////////

    StereoData_t FilterSample(StereoData_t x);

  private:

    // Functions                  ///////////////////////

  }; // class GenericFilter
  #pragma endregion

} // namespace Modifiers

// Public Functions             ////////////////////////////////////////////////

#endif // __GENERIC_FILTER_HPP
