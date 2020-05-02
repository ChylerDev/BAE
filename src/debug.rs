//! # Debug
//! 
//! Module containing utilities used for debugging.

/// Equivalency check of two floating point values.
///
/// # Parameters
///
/// * `a`, `b` - Floating point values that will be compared
/// * `e` - The epsilon value with which the difference between parameters `a`
///   and `b` must be to be deemed equivalent. For built-in floating point types
///   you are welcome to use [`std::f32::EPSILON`] or [`std::f64::EPSILON`].
/// * `abs` - Function that determines the absolute value of the given floating
///   point value. For built-in floating point types you are welcome to use
///   [`f32::abs`] or [`f64::abs`].
/// 
/// [`std::f32::EPSILON`]:
pub fn float_equal<T, F>(a: T, b: T, e: T, abs: F) -> bool
    where T: std::ops::Sub<T, Output=T> + std::cmp::PartialOrd,
          F: FnOnce(T)->T
{
    abs(a-b) < e
}

/// Used for testing purposes to avoid reading a file from disk, and instead
/// store the file contents in the binary.
pub struct VectorReader {
    vec: Vec<u8>,
    pos: usize,
}

impl VectorReader {
    pub fn new(vec: Vec<u8>) -> Self {
        VectorReader {
            vec,
            pos: 0,
        }
    }
}

impl std::io::Read for VectorReader {
    fn read(&mut self, buf: &mut[u8]) -> std::io::Result<usize> {
        let mut n = 0;
        while n < buf.len() && self.pos < self.vec.len() {
            buf[n] = self.vec[self.pos];
            n += 1;
            self.pos += 1;
        }
        Ok(n)
    }
}
