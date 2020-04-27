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
