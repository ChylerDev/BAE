/*! ****************************************************************************
\file             Block.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __BLOCK_HPP
#define __BLOCK_HPP

// Include Files                ////////////////////////////////////////////////

#include <memory>
#include <type_traits>
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

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Block
  {
  public:

      // void interactor(StereoData_t * OutputBuffer, StereoData_t * GeneratorBuffer, StereoData_t * ModifierBuffer, uint64_t buffer size)
    using Interaction_t = std::function<void(StereoData_t*, StereoData_t*, StereoData_t*, uint64_t)>;

  private:

    // Members              ///////////////////////

    GenBase_t m_Generator;
    ModBase_t m_Modifier;

    Interaction_t m_Interaction;

  public:

    template<typename ...Args>
    static inline Block_t Create(Args &&... params)
    {
      return std::make_shared<Block>(params...);
    }

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Block constructor.

      The default interactor will be used, which is defined as forwarding the
      generator's output.

    \param gen
      The generator used for the block.
    ***************************************************************************/
    Block(GenBase_t const & gen);

    /*! ************************************************************************
    \brief
      Block constructor.

      The default interactor will be used, which is defined forwarding the
      modifier's output.

    \param mod
      The modifier used for the block.
    ***************************************************************************/
    Block(ModBase_t const & mod);

    /*! ************************************************************************
    \brief
      Block constructor.

      The default interactor will be used, which is defined as multiplying the
      generated sample by the filtered sample.

    \param gen
      The generator used for the block.

    \param mod
      The modifier used for the block.
    ***************************************************************************/
    Block(GenBase_t const & gen, ModBase_t const & mod);

    /*! ************************************************************************
    \brief
      Block constructor.

    \param gen
      The generator used for the block.

    \param mod
      The modifier used for the block.

    \param interactor
      The function that defines how the output of the generator and the modifier
      are combined. The first argument is the sample from the generator, and the
      second argument is the sample from the modifier.
    ***************************************************************************/
    Block(
      GenBase_t const & gen,
      ModBase_t const & mod,
      Interaction_t const & interactor
    );

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    GenBase_t & GetGenerator();
    ModBase_t & GetModifier();
    GenBase_t const & GetGenerator() const;
    ModBase_t const & GetModifier() const;

    // Functions            ///////////////////////

    Block & SetInteractor(Interaction_t const & interactor);

    void SendBlock(StereoData_t * output, StereoData_t * input, uint64_t size);

  private:

    // Functions                  ///////////////////////

  }; // class Block

} // namespace Core
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __BLOCK_HPP
