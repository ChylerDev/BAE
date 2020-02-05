//! # WAV
//! 
//! A wave data player.

use riff;
use super::*;
use crate::tools::resampler::*;

/// Struct for playing wave files.
#[derive(Clone)]
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
	fn read_file(s: &str) -> (wav::Header, TrackT) {
		let mut v = std::vec::Vec::new();
		let mut h:wav::Header = wav::Header::default();

			// Open the wave file
		let mut f = std::fs::File::open(s).unwrap();

			// Read all chunks from the file
		let mut chunks = Vec::new();
		while let Result::Ok(c) = riff::read_chunk(&mut f) {
			chunks.push(c.0);
		}

			// Parse all the chunks, saving the format and data chunk data
		let mut sam:Vec<u8> = Vec::new();
		for c in chunks {
			let mut id = Vec::new();
			id.extend_from_slice(&c.id.value);

			if String::from_utf8(id.clone()).unwrap() == "fmt " {
				if let riff::ChunkContent::Subchunk(sc) = c.content {
					h=wav::Header::from(&sc[0..16]);
				}
			} else if String::from_utf8(id.clone()).unwrap() == "data" {
				if let riff::ChunkContent::Subchunk(sc) = c.content {
					sam = sc;
				}
			}
		}

			// Convert bytes to StereoData. If channel count is anything but 1
			// or 2, it is assumed the first two indices are the left and right
			// channels, respectively
		let mut ind = 0;
		while ind < sam.len() {
			match h.bits_per_sample {
				8 => match h.channel_count {
					1 => {
						v.push(StereoData::from_mono(sample_from_u8([sam[ind]])));
						ind += 1;
					},
					cc => {
						v.push(StereoData::from([sam[ind],sam[ind+1]]));
						ind += cc as usize;
					},
				},
				16 => match h.channel_count {
					1 => {
						v.push(StereoData::from_mono(sample_from_i16([sam[ind],sam[ind+1]])));
						ind += 2;
					},
					cc => {
						v.push(StereoData::from([sam[ind],sam[ind+1],sam[ind+2],sam[ind+3]]));
						ind += (cc as usize)*2;
					},
				},
				24 => match h.channel_count {
					1 => {
						v.push(StereoData::from_mono(sample_from_i24([sam[ind],sam[ind+1],sam[ind+2]])));
						ind += 3;
					},
					cc => {
						v.push(StereoData::from([sam[ind],sam[ind+1],sam[ind+2],sam[ind+3],sam[ind+4],sam[ind+5]]));
						ind += (cc as usize)*3;
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

impl Name for Wav {
	fn get_name(&self) -> &str {
		"Generator.Wav"
	}
}
