extern crate ocae;

#[cfg(test)]
mod tests {
	#[test]
	fn test_empty() {
		use ocae::generators::empty::*;

		run_generator(&mut Empty::new(), &String::from("empty.wav"));
	}

	#[test]
	fn test_noise() {
		use ocae::generators::noise::*;

		run_generator(&mut Noise::new(), &String::from("noise.wav"));
	}

	#[test]
	fn test_sawtooth() {
		use ocae::generators::FreqMod;
		use ocae::generators::sawtooth::*;

		run_generator(&mut Sawtooth::new(440.0), &String::from("sawtooth.wav"));
	}

	#[test]
	fn test_sine() {
		use ocae::generators::FreqMod;
		use ocae::generators::sine::*;

		run_generator(&mut Sine::new(440.0), &String::from("sine.wav"));
	}

	#[test]
	fn test_square() {
		use ocae::generators::FreqMod;
		use ocae::generators::square::*;

		run_generator(&mut Square::new(440.0), &String::from("square.wav"));
	}

	#[test]
	fn test_triangle() {
		use ocae::generators::FreqMod;
		use ocae::generators::triangle::*;

		run_generator(&mut Triangle::new(440.0), &String::from("triangle.wav"));
	}

	fn run_generator(g: &mut impl ocae::generators::Generator, file:&String) {
		let mut t = ocae::TrackT::new();
	
		for _ in 0..ocae::SAMPLE_RATE {
			t.push(g.process());
		}
	
		let mut f = String::from(".junk/generators/");
		f.push_str(file.as_str());

		ocae::tools::write_wav(t, f.as_str()).unwrap();
	}
}
