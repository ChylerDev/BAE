/*! ****************************************************************************
\file             WAV.cpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  Audio Engine

\copyright        Copyright © 2019 Chyler Morrison
*******************************************************************************/

// Include Files                          //////////////////////////////////////

#include <cstring>

#include <fstream>
#include <sstream>

#include <RIFF-Util/RIFF.hpp>

#include "../Tools/Input.hpp"
#include "../Tools/Resampler.hpp"

#include "WAV.hpp"

// Private Macros                         //////////////////////////////////////

// Private Enums                          //////////////////////////////////////

// Private Objects                        //////////////////////////////////////

// Private Function Declarations          //////////////////////////////////////

// Public Functions                       //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{
	WAV::WAV() : GeneratorBase(false),
		m_Resampler(), m_Table()
	{
		SetupMethodTable();
	}

	WAV::WAV(std::string const & path) : GeneratorBase(false),
		m_Resampler(), m_Table()
	{
		SetupMethodTable();

		ReadFile(path);
	}

	WAV::WAV(std::vector<char> const & data) : GeneratorBase(false),
		m_Resampler(), m_Table()
	{
		SetupMethodTable();

		ParseWAV(data.data(), int(data.size()));
	}

	WAV::WAV(int argc) : GeneratorBase(false),
		m_Resampler(), m_Table()
	{
		SetupMethodTable();

		ReadFile(Tools::GetOptions().at(argc));
	}

	void WAV::ReadFile(std::string const & path)
	{
		std::ifstream l_file(path, std::ios::binary);

		if(!l_file)
		{
			std::cerr << "WAV file " << path << " couldn't be opened.\n";
		}
		else
		{
			std::cout << "WAV file " << path << " opened for reading\n";

			std::vector<char> temp;

			l_file.seekg(0, std::ios::end);
			temp.reserve(l_file.tellg());
			l_file.seekg(0, std::ios::beg);

			std::cout << "Loading file...\n";

			temp.assign((std::istreambuf_iterator<char>(l_file)), std::istreambuf_iterator<char>());

			std::cout << "File successfully loaded\n";

			ParseWAV(temp.data(), int(temp.size()));
		}
	}

	void WAV::LoadWAV(std::vector<char> const & wav_data)
	{
		ParseWAV(wav_data.data(), int(wav_data.size()));
	}

	StereoData_t WAV::SendSample()
	{
		if(m_Resampler)
		{
			return m_Resampler->SendSample();
		}
		return StereoData_t(SampleType_t(0), SampleType_t(0));
	}

	void WAV::SendBlock(StereoData_t * buffer, uint64_t size)
	{
		m_Resampler->SendBlock(buffer, size);
	}

	MethodTable_t const & WAV::GetMethodTable() const
	{
		return m_Table;
	}
} // namespace Generator
} // namespace AudioEngine

// Private Functions                      //////////////////////////////////////

namespace AudioEngine
{
namespace Generator
{
	void WAV::SetupMethodTable()
	{
		m_Table["ReadFile"] = [this](void * path){ ReadFile(*reinterpret_cast<std::string*>(path)); };
	}

	void WAV::ParseWAV(char const * array, int size)
	{
			// Copy RIFF data to byte vector
		RIFF::vector_t vec;
		vec.assign(CHAR_STR_TO_BYTE_STR(array), CHAR_STR_TO_BYTE_STR(array) + size);
			// Parse the riff data
		RIFF::Reader riff(vec, CONSTRUCT_BYTE_STR("WAVE"));

			// Get the format chunk, check that it's size is correct
		RIFF::vector_t fmt = riff.GetChunk(CONSTRUCT_BYTE_STR("fmt "));
		if(fmt.size() != sizeof(WAVHeader))
		{
			std::cerr << "Error reading WAVE data, malformed header chunk\n";
			return;
		}
			// Cast the data as a header object for easier use
		WAVHeader const * header = reinterpret_cast<WAVHeader const *>(fmt.data());

			// Get the data chunk
		RIFF::vector_t data_vec = riff.GetChunk(CONSTRUCT_BYTE_STR("data"));
		std::vector<StereoData_t> AudioData;
		AudioData.reserve(data_vec.size() / header->BytesPerSample);

			// Parse data
		char const * data = reinterpret_cast<char *>(&data_vec[0]);
		while(data < reinterpret_cast<char *>(&data_vec[0] + data_vec.size()))
		{
			StereoData_t sample;

			if(header->BitsPerSample == 8)
			{
				if(header->ChannelCount == 1)
				{
					Left(sample) = Right(sample) =
						SampleType_t(((*data) << 8) * SQRT_HALF)/SampleType_t(0x8000);
				}
				else
				{
					Left(sample) = SampleType_t((*data) << 8)/SampleType_t(0x8000);
					Right(sample) = SampleType_t((*(data+1)) << 8)/SampleType_t(0x8000);
				}
			}
			else  // assume 16-bit audio is being used
			{
				int16_t const * rdata = reinterpret_cast<int16_t const *>(data);
				if(header->ChannelCount == 1)
				{
					Left(sample) = Right(sample) =
						SampleType_t((*rdata) * SQRT_HALF)/SampleType_t(0x8000);
				}
				else
				{
					Left(sample) = *rdata / SampleType_t(0x8000);
					Right(sample) = *(rdata+1) / SampleType_t(0x8000);
				}
			}

			AudioData.push_back(sample);
			data += header->BytesPerSample;
		}

		m_Resampler = Tools::Resampler::Create(
			AudioData, header->SamplingRate, 0, AudioData.size()-1
		);
	}
} // namespace Generator
} // namespace AudioEngine
