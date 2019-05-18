/*! ****************************************************************************
\file             Node.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

#ifndef __NODE_HPP
#define __NODE_HPP

// Include Files                ////////////////////////////////////////////////

#include <memory>
#include <type_traits>
#include <vector>

#include "../Engine.hpp"

#include "../Generators/Base.hpp"
#include "../Modifiers/Base.hpp"

// Public Macros                ////////////////////////////////////////////////

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

  using pNode_t = std::shared_ptr<class Node>;

  /*! **************************************************************************
  \brief
  *****************************************************************************/
  class Node
  {
  public:

      // StereoData_t interactor(StereoData_t GeneratorSample, StereoData_t ModifierSample)
    using Interaction_t = std::function<StereoData_t(StereoData_t const &, StereoData_t const &)>;
    using TargetsVec_t = std::vector<std::shared_ptr<StereoData_t>>;

  private:

    // Members              ///////////////////////

    TargetsVec_t m_Targets;

    Generator::pBase_t m_Generator;
    Modifier::pBase_t m_Modifier;

    Interaction_t m_Interaction;

    std::shared_ptr<StereoData_t> m_Input;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Node constructor.

      The default interactor will be used, which is defined as forwarding the
      generator's output.

    \param gen
      The generator used for the node.
    ***************************************************************************/
    Node(Generator::pBase_t const & gen);

    /*! ************************************************************************
    \brief
      Node constructor.

      The default interactor will be used, which is defined forwarding the
      modifier's output.

    \param mod
      The modifier used for the node.
    ***************************************************************************/
    Node(Modifier::pBase_t const & mod);

    /*! ************************************************************************
    \brief
      Node constructor.

      The default interactor will be used, which is defined as multiplying the
      generated sample by the filtered sample.

    \param gen
      The generator used for the node.

    \param mod
      The modifier used for the node.
    ***************************************************************************/
    Node(Generator::pBase_t const & gen, Modifier::pBase_t const & mod);

    /*! ************************************************************************
    \brief
      Node constructor.

    \param gen
      The generator used for the node.

    \param mod
      The modifier used for the node.

    \param interactor
      The function that defines how the output of the generator and the modifier
      are combined. The first argument is the sample from the generator, and the
      second argument is the sample from the modifier.
    ***************************************************************************/
    Node(
      Generator::pBase_t const & gen,
      Modifier::pBase_t const & mod,
      Interaction_t const & interactor
    );

    // Operators            ///////////////////////

    // Accossors/Mutators   ///////////////////////

    Generator::pBase_t & GetGenerator();
    Modifier::pBase_t & GetModifier();
    Generator::pBase_t const & GetGenerator() const;
    Modifier::pBase_t const & GetModifier() const;

    // Functions            ///////////////////////

    void SetInteractor(Interaction_t const & interactor);

    void AddTarget(Node const & target);
    void AddOutput(std::shared_ptr<StereoData_t> const & output);

    void SendSample(void);

    template<typename ...Args>
    static pNode_t Create(Args &&... params)
    {
      return std::make_shared<Node>(params);
    };

  private:

    // Functions                  ///////////////////////

  }; // class Node

} // namespace Core
} // namespace AudioEngine

// Public Functions             ////////////////////////////////////////////////

#endif // __NODE_HPP
