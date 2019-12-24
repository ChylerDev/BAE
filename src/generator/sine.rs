use arr_macro::arr;

struct Sine {
    ind:MathT,
    inc:MathT,
}

const WAVETABLE_FREQ:u64 = 10;
const WAVETABLE_SIZE:u64 = SAMPLE_RATE/WAVETABLE_FREQ;
static wavetable: [SampleT; WAVETABLE_SIZE] = arr![GenWavetable(); WAVETABLE_SIZE];

impl Sine {
    pub fn new(MathT f) -> Self {
        Sine{
            ind: 0.0,
            inc: f/WAVETABLE_FREQ,
        }
    }

    pub fn SetFrequency(&mut self, f: MathT) {
        self.inc = f / WAVETABLE_FREQ;
    }

    pub fn GetFrequency(&self) -> MathT {
        self.inc * WAVETABLE_FREQ
    }
}

impl Generator for Sine {
    pub fn Process(&self) -> StereoData {
        let k = MathT::floor(self.ind);
        let g:MathT = self.ind - k;

        let y = ((1.0-g)*wavetable[k] + g*wavetable[k+1]) as SampleT;

        self.ind += self.inc;

        if self.ind >= WAVETABLE_SIZE-1 {
            self.ind -= WAVETABLE_SIZE;
        }

        y
    }
}

static mut i:u64 = 0;
fn GenWavetable() -> SampleT{
    let s = SampleT::sin(2*SampleT::consts::PI*INV_SAMPLE_RATE*WAVETABLE_FREQ*unsafe{i});
    unsafe{i += 1};

    s
}
