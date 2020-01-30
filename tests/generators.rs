extern crate ocae;

#[cfg(test)]
mod tests {
	#[test]
	fn test_empty() {
		use ocae::generators::*;

		run_generator(&mut Empty::new(), "empty.wav");
	}

	#[test]
	fn test_noise() {
		use ocae::generators::*;

		run_generator(&mut Noise::new(), "noise.wav");
	}

	#[test]
	fn test_sawtooth() {
		use ocae::generators::*;

		run_generator(&mut Sawtooth::new(440.0), "sawtooth.wav");
	}

	#[test]
	fn test_sine() {
		use ocae::generators::*;

		run_generator(&mut Sine::new(440.0), "sine.wav");
	}

	#[test]
	fn test_square() {
		use ocae::generators::*;

		run_generator(&mut Square::new(440.0), "square.wav");
	}

	#[test]
	fn test_triangle() {
		use ocae::generators::*;

		run_generator(&mut Triangle::new(440.0), "triangle.wav");
	}

	fn run_generator(g: &mut impl ocae::generators::Generator, file:&str) {
		let mut t = ocae::TrackT::new();
	
		for _ in 0..ocae::SAMPLE_RATE {
			t.push(g.process());
		}
	
		let mut f = String::from(".junk/generators/");
		f.push_str(file);

		ocae::tools::write_wav(t, f.as_str()).unwrap();
	}
}
