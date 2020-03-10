extern crate bae_rs;

#[cfg(test)]
mod tests {
	const FILE_PREFIX: &'static str = ".junk/generators/";

	#[test]
	fn test_empty() {
		use bae_rs::generators::*;

		run_generator(&mut Zero::new(), "empty.wav");
	}

	#[test]
	fn test_noise() {
		use bae_rs::generators::*;

		run_generator(&mut Noise::new(), "noise.wav");
	}

	#[test]
	fn test_sawtooth() {
		use bae_rs::generators::*;

		run_generator(&mut Sawtooth::new(440.0), "sawtooth.wav");
	}

	#[test]
	fn test_sine() {
		use bae_rs::generators::*;

		run_generator(&mut Sine::new(440.0), "sine.wav");
	}

	#[test]
	fn test_square() {
		use bae_rs::generators::*;

		run_generator(&mut Square::new(440.0), "square.wav");
	}

	#[test]
	fn test_triangle() {
		use bae_rs::generators::*;

		run_generator(&mut Triangle::new(440.0), "triangle.wav");
	}

	#[test]
	fn test_wavplayer() {
		use bae_rs::generators::*;

		let input = "wavplayer_input.wav";
		let mut path = String::from(FILE_PREFIX);
		path.push_str(input);

		run_generator(&mut Triangle::new(440.0), input);
		run_generator(&mut MonoWav::from_file(path.as_str()), "wavplayer.wav");
	}

	fn run_generator(g: &mut dyn bae_rs::generators::Generator, file:&str)
	{
		let mut t = bae_rs::TrackT::new();
	
		for _ in 0..bae_rs::SAMPLE_RATE {
			t.push(g.process());
		}
	
		let mut f = String::from(FILE_PREFIX);
		f.push_str(file);

		bae_rs::tools::write_wav(t, f.as_str()).unwrap();
	}
}
