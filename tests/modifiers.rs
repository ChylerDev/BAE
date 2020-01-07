extern crate ocae;

#[cfg(test)]
mod tests {
	#[test]
	fn test_adsr() {
		use ocae::modifiers::{*, adsr::*};
		use std::time::Duration;

		let mut a = ADSR::new(
			Duration::from_secs_f64(0.03125),
			Duration::from_secs_f64(0.125),
			ocae::linear_db(0.5),
			Duration::from_secs_f64(0.5)
		);

		use ocae::generators::{Generator, FreqMod, sine::*};

		let mut g = Sine::new(440.0);
		let mut t = ocae::TrackT::new();

		for i in 0..ocae::SAMPLE_RATE {
			if i == ocae::SAMPLE_RATE/2 {
				a.release();
			}

			t.push(a.process(g.process()));
		}

		let f = String::from(".junk/modifiers/adsr.wav");
		ocae::tools::write_wav(t, f).unwrap();
	}

	fn run_modifier(m: &mut impl ocae::modifiers::Modifier, file:String) {
		use ocae::generators::{Generator, FreqMod, sine::*};

		let mut g = Sine::new(440.0);
		let mut t = ocae::TrackT::new();

		for _ in 0..ocae::SAMPLE_RATE {
			t.push(m.process(g.process()));
		}

		let mut f = String::from(".junk/modifiers/");
		f.push_str(file.as_str());

		ocae::tools::write_wav(t, f).unwrap();
	}
}
