struct Sine {
    ind:MathT,
    inc:MathT,
}

static wavetable: 'static [SampleT; ] = [];

impl Sine {
    pub fn new(MathT f) -> Self {
        Sine{
            ind: 0.0,
            inc: 
        }
    }
}

impl Generator for Sine {
    pub fn Process(&self) -> StereoData {

    }
}
