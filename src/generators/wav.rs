//! # WAV
//! 
//! A wave data player.

use riff;
use super::*;
use crate::tools::resampler::*;
use crate::tools::WAVHeader;

/// Struct for playing wave files.
pub struct Wav {
	resam: Resampler,
}

impl Wav {
	/// Constructs from a given path.
	pub fn from_file(s: &str) -> Wav {
		let (h, t) = Wav::read_file(s);

		Wav::from_track(h.sampling_rate as u64,t)
	}

	/// Converts from the given track and source sample rate.
	pub fn from_track(sampling_rate:u64, t:TrackT) -> Self {
		Wav{
			resam: Resampler::new(t, sampling_rate, 0, 0)
		}
	}

	/// Borrows the resampler object.
	pub fn get(&self) -> &Resampler {
		&self.resam
	}

	/// Mutably borrows the resampler object.
	pub fn get_mut(&mut self) -> &mut Resampler {
		&mut self.resam
	}

	/// Reads in a file and parses the riff/wave data from it.
	fn read_file(s: &str) -> (WAVHeader, TrackT) {
		let mut v = std::vec::Vec::new();
		let mut h:WAVHeader = WAVHeader::default();

			// Open the wave file
		let mut f = std::fs::File::open(s).unwrap();

			// Read all chunks from the file
		let mut chunks = Vec::new();
		loop {
			match riff::read_chunk(&mut f) {
				Result::Ok(c) => chunks.push(c.0),
				Result::Err(_) => break,
			};
		}

			// Parse all the chunks, saving the format and data chunk data
		let mut sam:Vec<u8> = Vec::new();
		for c in chunks {
			let mut id = Vec::new();
			id.extend_from_slice(&c.id.value);

			if String::from_utf8(id.clone()).unwrap() == String::from("fmt ") {
				match c.content {
					riff::ChunkContent::Subchunk(sc) => h=WAVHeader::from(&sc[0..16]),
					_ => ()
				};
			} else if String::from_utf8(id.clone()).unwrap() == String::from("data") {
				match c.content {
					riff::ChunkContent::Subchunk(sc) => sam=sc,
					_ => ()
				};
			}
		}

			// Convert bytes to StereoData. If channel count is anything but 1
			// or 2, it is assumed the first two indices are the left and right
			// channel, respectively
		let mut i = 0;
		while i < sam.len() {
			match h.bits_per_sample {
				8 => match h.channel_count {
					1 => {
						v.push(StereoData::from_mono(sample_from_u8([sam[i]])));
						i += 1;
					},
					c => {
						v.push(StereoData::from([sam[i],sam[i+1]]));
						i += c as usize;
					},
				},
				16 => match h.channel_count {
					1 => {
						v.push(StereoData::from_mono(sample_from_i16([sam[i],sam[i+1]])));
						i += 2;
					},
					c => {
						v.push(StereoData::from([sam[i],sam[i+1],sam[i+2],sam[i+3]]));
						i += (c as usize)*2;
					},
				},
				24 => match h.channel_count {
					1 => {
						v.push(StereoData::from_mono(sample_from_i24([sam[i],sam[i+1],sam[i+2]])));
						i += 3;
					},
					c => {
						v.push(StereoData::from([sam[i],sam[i+1],sam[i+2],sam[i+3],sam[i+4],sam[i+5]]));
						i += (c as usize)*3;
					},
				},
				_ => ()
			}
		}

		(h,v)
	}
}

impl Generator for Wav {
	fn process(&mut self) -> StereoData {
		self.resam.process()
	}
}
