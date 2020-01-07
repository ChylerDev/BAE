extern crate ocae;

#[cfg(test)]
mod tests {
	#[test]
	fn test_adsr() {
		use ocae::modifiers::adsr::*;
		use std::time::Duration;

		let mut a = ADSR::new(
			Duration::from_secs_f64(0.0625),
			Duration::from_secs_f64(0.125),
			ocae::linear_db(0.75),
			Duration::from_secs_f64(0.5)
		);

		run_modifier(
			&mut a,
			Box::new(|i|{if i == ocae::SAMPLE_RATE/2 { a.release(); } }),
			String::from("adsr.wav")
		);
	}

	fn run_modifier(m: &mut impl ocae::modifiers::Modifier, mut c: Box<dyn FnMut(u64)>, file:String)
	{
		use ocae::generators::{Generator, FreqMod, sine::*};

		let mut g = Sine::new(440.0);
		let mut t = ocae::TrackT::new();

		for i in 0..ocae::SAMPLE_RATE {
			(*c)(i);
			t.push(m.process(g.process()));
		}

		let mut f = String::from(".junk/modifiers/");
		f.push_str(file.as_str());

		ocae::tools::write_wav(t, f).unwrap();
	}
}
