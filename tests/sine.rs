extern crate ocae;

#[cfg(test)]
mod tests {
    #[test]
    fn test_sine() {
        use ocae::generator::Generator;
        use ocae::generator::sine::*;

        let mut s = Sine::new(440.0);

        let mut t = ocae::TrackT::new();

        for _ in 0..ocae::SAMPLE_RATE {
            t.push(s.process());
        }

        ocae::tools::write_wav(t, String::from("sinetest.wav"));
    }
}
