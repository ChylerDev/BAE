extern crate ocae;

#[cfg(test)]
mod tests {
	#[test]
	fn test_write_wav() {
		use ocae::tools::write_wav;

		let mut t = ocae::TrackT::new();

		for i in 0..16 {
			t.push(ocae::StereoData::from_mono((2.0 * std::f32::consts::PI * 440.0 * i as f32 * ocae::INV_SAMPLE_RATE as f32).sin()));
		}

		write_wav(t, String::from(".junk/tools/wavwrite.wav"));
	}

	#[test]
	fn test_resampler() {
		use ocae::StereoData;
		use ocae::tools::resampler::*;

		let sam = vec![StereoData::from_stereo(0.0, 0.0), StereoData::from_stereo(1.0,1.0), StereoData::from_stereo(2.0, 2.0), StereoData::from_stereo(3.0, 3.0)];

		let mut r = Resampler::new(sam.clone(), ocae::SAMPLE_RATE/2, 0,0);
		for i in 0..7 {
			let s = r.process();

			assert_eq!(s.left(), i as ocae::SampleT/2.0);
			assert_eq!(s.right(), i as ocae::SampleT/2.0);
		}

		let mut r = Resampler::new(sam.clone(), ocae::SAMPLE_RATE * 2, 0,0);
		for i in 0..2 {
			let s = r.process();

			assert_eq!(s.left(), (i * 2) as ocae::SampleT);
			assert_eq!(s.right(), (i * 2) as ocae::SampleT);
		}

		let mut r = Resampler::new(sam.clone(), ocae::SAMPLE_RATE, 0,0);
		r.set_playback_speed(0.5);
		for i in 0..7 {
			let s = r.process();

			assert_eq!(s.left(), i as ocae::SampleT/2.0);
			assert_eq!(s.right(), i as ocae::SampleT/2.0);
		}

		let mut r = Resampler::new(sam.clone(), ocae::SAMPLE_RATE * 2, 0,0);
		r.set_playback_speed(0.5);
		for i in 0..sam.len() {
			let s = r.process();

			assert_eq!(s.left(), sam[i].left());
			assert_eq!(s.right(), sam[i].right());
		}
	}
}
