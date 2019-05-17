/*! ****************************************************************************
\file             Node.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "Node.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

  Node::Node(std::shared_ptr<Generator::Base> const & gen) :
    m_Targets(),
    m_Generator(gen),
    m_Modifier(Modifier::Base::CreateModifier<Modifier::Base>(true)),
    m_Interaction(
      [this](StereoData_t const & g, StereoData_t const & m)
      { UNREFERENCED_PARAMETER(m); return g; }
    ),
    m_Input(std::make_shared<StereoData_t>(0.f,0.f))
  {
  }

  Node::Node(Modifier::pBase_t const & mod) :
    m_Targets(),
    m_Generator(std::make_shared<Generator::Base>(true)),
    m_Modifier(mod),
    m_Interaction(
      [this](StereoData_t const & g, StereoData_t const & m)
      { UNREFERENCED_PARAMETER(g); return m; }
    ),
    m_Input(std::make_shared<StereoData_t>(0.f,0.f))
  {
  }

  Node::Node(
    std::shared_ptr<Generator::Base> const & gen,
    Modifier::pBase_t const & mod
  ) :
    m_Targets(),
    m_Generator(gen),
    m_Modifier(mod),
    m_Interaction(
      [this](StereoData_t const & g, StereoData_t const & m)
      {
        return this->m_Generator->IsBase() ?
          m :
          (this->m_Modifier->IsBase() ?
            g :
            StereoData_t(
              std::get<0>(g) * std::get<0>(m),
              std::get<1>(g) * std::get<1>(m)
            )
          );
      }
    ),
    m_Input(std::make_shared<StereoData_t>(0.f,0.f))
  {
  }

  Node::Node(
    std::shared_ptr<Generator::Base> const & gen,
    Modifier::pBase_t const & mod,
    Interaction_t const & interactor
  ) :
    m_Targets(),
    m_Generator(gen),
    m_Modifier(mod),
    m_Interaction(interactor),
    m_Input(std::make_shared<StereoData_t>(0.f, 0.f))
  {
  }

  std::shared_ptr<Generator::Base> & Node::GetGenerator()
  {
    return m_Generator;
  }

  Modifier::pBase_t & Node::GetModifier()
  {
    return m_Modifier;
  }

  std::shared_ptr<Generator::Base> const & Node::GetGenerator() const
  {
    return m_Generator;
  }

  Modifier::pBase_t const & Node::GetModifier() const
  {
    return m_Modifier;
  }

  void Node::SetInteractor(Interaction_t const & interactor)
  {
    m_Interaction = interactor;
  }

  void Node::AddTarget(Node const & target)
  {
    m_Targets.push_back(target.m_Input);
  }

  void Node::AddFinalOutput(std::shared_ptr<StereoData_t> const & output)
  {
    m_Targets.push_back(output);
  }

  void Node::SendSample(void)
  {
    StereoData_t output = m_Interaction(
      m_Generator->SendSample(), m_Modifier->FilterSample(*m_Input)
    );

      // Set to 0 before setting the targets in case this node itself is one of the targets
    *m_Input = StereoData_t(0.f,0.f);

    for(auto & target : m_Targets)
    {
      std::get<0>(*target) += std::get<0>(output);
      std::get<1>(*target) += std::get<1>(output);
    }
  }

} // namespace Core
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
