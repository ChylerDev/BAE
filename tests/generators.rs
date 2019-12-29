extern crate ocae;

#[cfg(test)]
mod tests {
    #[test]
    fn test_noise() {
        use ocae::generators::Generator;
        use ocae::generators::noise::*;

        let mut n = Noise::new();

        let mut t = ocae::TrackT::new();
        
        for _ in 0..ocae::SAMPLE_RATE {
            t.push(n.process());
        }

        ocae::tools::write_wav(t, String::from(".junk/generators/noise.wav"));
    }

    #[test]
    fn test_sawtooth() {
        use ocae::generators::Generator;
        use ocae::generators::sawtooth::*;

        let mut s = Sine::new(440.0);

        let mut t = ocae::TrackT::new();

        for _ in 0..ocae::SAMPLE_RATE {
            t.push(s.process());
        }

        ocae::tools::write_wav(t, String::from(".junk/generators/sawtooth.wav"))
    }

    #[test]
    fn test_sine() {
        use ocae::generators::Generator;
        use ocae::generators::sine::*;

        let mut s = Sine::new(440.0);

        let mut t = ocae::TrackT::new();

        for _ in 0..ocae::SAMPLE_RATE {
            t.push(s.process());
        }

        ocae::tools::write_wav(t, String::from(".junk/generators/sine.wav"));
    }
}
