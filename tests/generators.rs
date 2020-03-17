extern crate bae_rs;

#[cfg(test)]
mod tests {
    use std::fs::File;
    use bae_rs::generators::*;

    const FILE_PREFIX: &'static str = ".junk/generators/";

    #[test]
    fn test_empty() {
        run_generator(&mut Zero::new(), "empty.wav");
    }

    #[test]
    fn test_noise() {
        run_generator(&mut Noise::new(), "noise.wav");
    }

    #[test]
    fn test_sawtooth() {
        run_generator(&mut Sawtooth::new(440.0), "sawtooth.wav");
    }

    #[test]
    fn test_sine() {
        run_generator(&mut Sine::new(440.0), "sine.wav");
    }

    #[test]
    fn test_square() {
        run_generator(&mut Square::new(440.0), "square.wav");
    }

    #[test]
    fn test_triangle() {
        run_generator(&mut Triangle::new(440.0), "triangle.wav");
    }

    #[test]
    fn test_wavplayer() {
        let input = "wavplayer_input.wav";
        let mut path = String::from(FILE_PREFIX);
        path.push_str(input);

        run_generator(&mut Triangle::new(440.0), input);
        run_generator(&mut MonoWav::from_source(&mut File::open(path.as_str()).unwrap()), "wavplayer.wav");
    }

    fn run_generator(g: &mut dyn bae_rs::generators::Generator, file:&str)
    {
        use bae_rs::tools::write_wav;

        let mut t = bae_rs::TrackT::new();
    
        for _ in 0..bae_rs::SAMPLE_RATE {
            t.push(g.process());
        }

        let mut f = String::from(FILE_PREFIX);
        f.push_str(file);

        write_wav(vec![t], 24, &mut File::create(f.as_str()).unwrap()).unwrap();
    }
}
