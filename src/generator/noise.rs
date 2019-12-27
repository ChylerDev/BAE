#[derive(Default)]
struct Noise {
    engine: rand::ThreadRng
}

impl Noise {
    pub fn new() -> Self {
        Noise{
            engine: rand::thread_rng()
        }
    }
}

impl Generator for Noise {
    pub fn process(&self) -> StereoData {
        StereoData::from_mono(
            self.engine.gen()*2.0-1.0
        )
    }
}
