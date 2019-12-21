/*! ****************************************************************************
\file             Sine.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cmath>

#include "Sine.hpp"

// Private Macros                         //////////////////////////////////////

#define WAVTABLE_FREQ (10)
#define WAVTABLE_SIZE ((OCAE_SAMPLE_RATE) / (WAVTABLE_FREQ))

// Private Enums                          //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

OCAE::Math_t OCAE::Generator::Sine::s_Table[WAVTABLE_SIZE];
int OCAE::Generator::Sine::dummy = OCAE::Generator::Sine::SetupWaveTable();

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	Sine::Sine(Math_t f) : GeneratorBase(),
		m_A(f/WAVTABLE_FREQ), m_Index()
	{
		RegisterMethods(CreateMethodList());
	}

	StereoData Sine::SendSample(void)
	{
		int64_t k = int64_t(std::floor(m_Index));
		Math_t g = m_Index - k;
		SampleType y = float(
			(1-g)*s_Table[k] +
			g*s_Table[k+1]
		);

		m_Index += m_A;

		if(m_Index >= WAVTABLE_SIZE - 1)
		{
			m_Index -= WAVTABLE_SIZE;
		}

		return OCAE_MONO_TO_STEREO(y);
	}

	void Sine::SetFrequency(Math_t f)
	{
		m_A = f / WAVTABLE_FREQ;
	}

	Math_t Sine::GetFrequency() const
	{
		return m_A * WAVTABLE_FREQ;
	}

	Tools::MethodTable::MethodList_t Sine::CreateMethodList()
	{
		return {
			std::make_tuple(
				std::string("SetFrequency"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						SetFrequency(
							std::get<0>(
								*reinterpret_cast<
									std::tuple<OCAE_METHOD_PARAM_T(Math_t)>*
								>(p)
							)
						);
					}
				)
			),
			std::make_tuple(
				std::string("GetFrequency"),
				Tools::MethodTable::Void_fn(
					[this](void * p){
						std::get<0>(
							*reinterpret_cast<
								std::tuple<OCAE_METHOD_RET_T(Math_t)>*
							>(p)
						) = GetFrequency();
					}
				)
			)
		};
	}
} // namespace Generator
} // namespace OCAE

// Private Functions                      //////////////////////////////////////

namespace OCAE
{
namespace Generator
{
	int Sine::SetupWaveTable()
	{
		for(auto i = 0; i < WAVTABLE_SIZE; ++i)
		{
			s_Table[i] = std::sin(2 * OCAE_PI * OCAE_INC_RATE * WAVTABLE_FREQ * i);
		}

		return 0;
	}
} // namespace Generator
} // namespace OCAE
