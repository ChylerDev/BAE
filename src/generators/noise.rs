use super::*;
use rand;

#[derive(Default)]
pub struct Noise {
    engine: rand::rngs::ThreadRng,
}

impl Noise {
    pub fn new() -> Self {
        Noise{
            engine: rand::thread_rng()
        }
    }
}

impl Generator for Noise {
    fn process(&mut self) -> StereoData {
        use rand::Rng;

        StereoData::from_mono(
            self.engine.gen::<SampleT>()*2.0-1.0
        )
    }
}
