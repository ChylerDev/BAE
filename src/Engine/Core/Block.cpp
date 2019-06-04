/*! ****************************************************************************
\file             Block.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  AudioEngine

\copyright        Copyright © 2018 Chyler
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include "../Generators/Base.hpp"
#include "../Modifiers/Base.hpp"

#include "Block.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Core
{

  Block::Block(GenBase_t const & gen) :
    m_Generator(gen),
    m_Modifier(Modifier::Base::Create<Modifier::Base>(true)),
    m_Interaction(
      [this](StereoData_t*o, StereoData_t*g, StereoData_t*m, uint64_t s)
      { UNREFERENCED_PARAMETER(m); static uint64_t i;
        for(i=0;i<s;++i){ LEFT(o[i])+=LEFT(g[i]), RIGHT(o[i])+=RIGHT(g[i]); } }
    )
  {
  }

  Block::Block(ModBase_t const & mod) :
    m_Generator(Generator::Base::Create<Generator::Base>(true)),
    m_Modifier(mod),
    m_Interaction(
      [this](StereoData_t*o, StereoData_t*g, StereoData_t*m, uint64_t s)
      { UNREFERENCED_PARAMETER(g); static uint64_t i;
        for(i=0;i<s;++i){ LEFT(o[i])+=LEFT(m[i]), RIGHT(o[i])+=RIGHT(m[i]); } }
    )
  {
  }

  Block::Block(
    GenBase_t const & gen,
    ModBase_t const & mod
  ) :
    m_Generator(gen),
    m_Modifier(mod),
    m_Interaction(
      [this](StereoData_t*o, StereoData_t*g, StereoData_t*m, uint64_t s)
      {
        static uint64_t i;

        if(this->m_Generator->IsBase())
        {
          for(i = 0; i < s; ++i)
          {
            LEFT(o[i]) += LEFT(m[i]),
            RIGHT(o[i]) += RIGHT(m[i]);
          }
        }
        else if(this->m_Modifier->IsBase())
        {
          for(i = 0; i < s; ++i)
          {
            LEFT(o[i]) += LEFT(g[i]),
            RIGHT(o[i]) += RIGHT(g[i]);
          }
        }
        else
        {
          for(i = 0; i < s; ++i)
          {
            LEFT(o[i]) += LEFT(g[i]) * LEFT(m[i]);
            RIGHT(o[i]) += RIGHT(g[i]) * RIGHT(m[i]);
          }
        }
      }
    )
  {
  }

  Block::Block(
    GenBase_t const & gen,
    ModBase_t const & mod,
    Interaction_t const & interactor
  ) :
    m_Generator(gen),
    m_Modifier(mod),
    m_Interaction(interactor)
  {
  }

  GenBase_t & Block::GetGenerator()
  {
    return m_Generator;
  }

  ModBase_t & Block::GetModifier()
  {
    return m_Modifier;
  }

  GenBase_t const & Block::GetGenerator() const
  {
    return m_Generator;
  }

  ModBase_t const & Block::GetModifier() const
  {
    return m_Modifier;
  }

  Block & Block::SetInteractor(Interaction_t const & interactor)
  {
    m_Interaction = interactor;
    return *this;
  }

  void Block::SendBlock(StereoData_t * output, StereoData_t * input, uint64_t size)
  {
    static aStereoData_t generator_buf(new StereoData_t[size]);
    static aStereoData_t  modifier_buf(new StereoData_t[size]);

    std::fill(generator_buf.get(), generator_buf.get()+size, StereoData_t(0,0));
    std::fill( modifier_buf.get(),  modifier_buf.get()+size, StereoData_t(0,0));

    m_Modifier->FilterBlock(input, modifier_buf.get(), size);
    m_Generator->SendBlock(generator_buf.get(),  size);

    m_Interaction(output, generator_buf.get(), modifier_buf.get(), size);
  }

} // namespace Core
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////
