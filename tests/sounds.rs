extern crate bae_rs;

#[cfg(test)]
mod tests {
    use std::sync::Arc;
    use std::fs::File;
    use std::time::Duration;
    use bae_rs::{generators::*, modifiers::*, sounds::*, utils::*};

    #[test]
    fn test_blocks() {
        let mut b = StandardBlock::from_generator(Sine::new(440.0));
        let mut s = Sine::new(440.0);

        for _ in 0..seconds_to_samples(std::time::Duration::from_secs_f64(1.0/440.0)) {
            assert!((b.process() - s.process()).abs() < 1e-15);
        }

        let mut b = StandardBlock::from_modifier(LowPass::new(440.0, 1.0));
        let mut n = Noise::new();

        let mut t = bae_rs::SampleTrackT::new();

        for _ in 0..seconds_to_samples(Duration::from_secs_f64(0.5)) {
            b.prime_input(n.process());
            t.push(b.process());
        }

        normalize_write(-1.5, t, &mut File::create(".junk/sounds/block_NoiseLP.wav").unwrap())
}

    #[test]
    fn test_simple_sounds() {
        let mut ss = SimpleSound::new(1.0, 0.5,
            Arc::new(StandardBlock::from_generator(Noise::new()))
        );
        ss.extend_modifiers(
            vec![
                Arc::new(
                    StandardBlock::from_modifier(
                        LowPass::new(440.0, 1.0)
                    )
                ),
                Arc::new(
                    StandardBlock::from_modifier(
                        HighPass::new(220.0, 1.0)
                    )
                )
            ]
        );

        let mut t = bae_rs::SampleTrackT::new();

        for _ in 0..seconds_to_samples(Duration::from_secs(4)) {
            t.push(ss.process(0.0));
        }

        normalize_write(-1.5, t, &mut File::create(".junk/sounds/simple_sounds.wav").unwrap())
    }

    #[test]
    fn test_complex_sounds() {
        let mut cs = ComplexSound::new(1.0, 1.0);

        let n = cs.add_block(
            Arc::new(
                StandardBlock::from_generator(Noise::new())
            )
        );
        let lp = cs.add_block(
            Arc::new(
                StandardBlock::from_modifier(LowPass::new(440.0, 1.0))
            )
        );
        let hp = cs.add_block(
            Arc::new(
                StandardBlock::from_modifier(HighPass::new(220.0, 1.0))
            )
        );

        cs.add_connection(cs.get_input_gain(), n);
        cs.add_connection(n, lp);
        cs.add_connection(lp, hp);
        cs.add_connection(hp, cs.get_output_gain());

        let mut t = bae_rs::SampleTrackT::new();

        for _ in 0..seconds_to_samples(Duration::from_secs(4)) {
            t.push(cs.process(0.0));
        }

        normalize_write(-1.5, t, &mut File::create(".junk/sounds/complex_sounds.wav").unwrap())
    }

    fn normalize_write(db: bae_rs::MathT, mut t: bae_rs::SampleTrackT, d: &mut dyn std::io::Write) {
        normalize(db, &mut t);

        write_wav(vec![t], 24, d, false).expect("Failed to write wav file");
    }
}
