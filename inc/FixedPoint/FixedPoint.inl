/*! ****************************************************************************
\file             FixedPoint.inl
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  FixedPoint

\copyright        Copyright © 2019 Chyler
*******************************************************************************/

#ifndef __FIXED_POINT_INL
#define __FIXED_POINT_INL

// Include Files                          //////////////////////////////////////

#include <cmath>

#include <type_traits>

#include "FixedPoint.hpp"

// Public Functions                       //////////////////////////////////////

namespace FixedPoint
{

  //////////////// FixedPoint Functions ////////////////

PUSH_WARNINGS() // Popped at end of class functions
  // Disable warning for incorrect shift compilation. Code paths that generate
  // the warning are never reached due to the template parameter but VC++ still
  // compiles both paths
MSVC_DISABLE_WARNING(4293)

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int>::FixedPoint() :
    m_Int(0)
  {
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int>::FixedPoint(FixedPoint const & other) :
    m_Int(other.m_Int)
  {
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  FixedPoint<M,N,Int>::FixedPoint(FixedPoint<M2,N2,Int2> const & other) :
    m_Int(N > N2 ?
            SaturationOption<Q_M,Q_N,Int_t>(
              (Int_t(other.m_Int)) << (MAX_UINT(N-N2))
            ) :
            SaturationOption<Q_M,Q_N,Int_t>(
              (other.m_Int) >> (MAX_UINT(N2-N))
            )
    )
  {
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<typename Value,
    typename std::enable_if_t<
      std::negation_v<std::is_base_of<QFormat, Value>>, int
    >
  >
  FixedPoint<M,N,Int>::FixedPoint(Value const & val) :
    m_Int(Int_t(val * (Int_t(1) << Q_N)))
  {
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int>::~FixedPoint()
  {
    static_assert(std::is_signed_v<Int_t>, "FixedPoint's Int type must be signed");
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int>::operator bool() const
  {
    return bool(m_Int);
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<typename Value,
    typename std::enable_if_t<
      std::conjunction_v<
        std::negation<std::is_base_of<QFormat, Value>>,
        std::negation<std::is_same<Value, bool>>
      >,
      int
    >
  >
  FixedPoint<M,N,Int>::operator Value()
  {
    return Value(
      std::is_floating_point_v<Value> ?
        m_Int / (Int_t(1) << Q_N) :
        m_Int >> Q_N
    );
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<typename Value,
    typename std::enable_if_t<
      std::negation_v<std::is_base_of<QFormat, Value>>, int
    >
  >
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator=(Value const & rhs)
  {
    m_Int = Int_t(rhs * (Int_t(1) << Q_N));

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> FixedPoint<M,N,Int>::operator>>(unsigned shift)
  {
    FixedPoint<M,N,Int> result;

    result.m_Int = m_Int >> shift;

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> FixedPoint<M,N,Int>::operator<<(unsigned shift)
  {
    FixedPoint<M,N,Int> result;

    result.m_Int = m_Int << shift;

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator>>=(unsigned shift)
  {
    m_Int >>= shift;

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator<<=(unsigned shift)
  {
    m_Int <<= shift;

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> FixedPoint<M,N,Int>::operator+() const
  {
    FixedPoint result(*this);

    if(m_Int < 0)
    {
      result.m_Int = Int_t(-m_Int);
    }

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> FixedPoint<M,N,Int>::operator-() const
  {
    FixedPoint result;

    result.m_Int = Int_t(-m_Int);

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator++()
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;

    m_Int = SaturationOption<Q_M,Q_N,Int_t>(
      LargerInt_t(m_Int) + (LargerInt_t(1) << Q_N)
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> FixedPoint<M,N,Int>::operator++(int dummy)
  {
    (void)(dummy);

    FixedPoint before(*this);

    ++(*this);

    return before;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator--()
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;

    m_Int = SaturationOption<Q_M,Q_N,Int_t>(
      LargerInt_t(m_Int) - (LargerInt_t(1) << Q_N)
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> FixedPoint<M,N,Int>::operator--(int dummy)
  {
    (void)(dummy);

    FixedPoint before(*this);

    --(*this);

    return before;
  }

    //////// FixedPoint Operators ////////

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator=(FixedPoint const & rhs)
  {
    m_Int = rhs.m_Int;

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int>
    FixedPoint<M,N,Int>::operator+(FixedPoint const & rhs) const
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;

    FixedPoint result;
    result.m_Int = FixedPoint::SaturationOption<Q_M,Q_N,Int_t>(
      LargerInt_t(m_Int) + LargerInt_t(rhs.m_Int)
    );

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int>
    FixedPoint<M,N,Int>::operator-(FixedPoint const & rhs) const
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;

    FixedPoint result;

    result.m_Int = FixedPoint::SaturationOption<Q_M,Q_N,Int_t>(
      LargerInt_t(m_Int) - LargerInt_t(rhs.m_Int)
    );

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int>
    FixedPoint<M,N,Int>::operator*(FixedPoint const & rhs) const
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;
    static constexpr uintmax_t Shift_v = Q_N;
    #if FIXED_POINT_ROUND
      static constexpr Int_t K = 1 << (Q_N - 1);
    #endif

    FixedPoint result;

    result.m_Int = SaturationOption<Q_M, Q_N, Int_t>(
      ((LargerInt_t(m_Int) * LargerInt_t(rhs.m_Int))
      #if FIXED_POINT_ROUND
        + K
      #endif
      ) >> Shift_v
    );

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int>
    FixedPoint<M,N,Int>::operator/(FixedPoint const & rhs) const
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;
    static constexpr uintmax_t Shift_v = Q_N;

    FixedPoint result;

    result.m_Int = Int_t(
      (LargerInt_t(m_Int) << Shift_v) / LargerInt_t(rhs.m_Int)
    );

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator+=(FixedPoint const & rhs)
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;

    m_Int = SaturationOption<Q_M,Q_N,Int_t>(
      LargerInt_t(m_Int) + LargerInt_t(rhs.m_Int)
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator-=(FixedPoint const & rhs)
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;

    m_Int = SaturationOption<Q_M, Q_N, Int_t>(
      LargerInt_t(m_Int) - LargerInt_t(rhs.m_Int)
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator*=(FixedPoint const & rhs)
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;
    static constexpr uintmax_t Shift_v = Q_N;
    #if FIXED_POINT_ROUND
      static constexpr Int_t K = 1 << (Q_N - 1);
    #endif

    m_Int = SaturationOption<Q_M,Q_N,Int_t>(
      ((LargerInt_t(m_Int) * LargerInt_t(rhs.m_Int))
      #if FIXED_POINT_ROUND
        + K
      #endif
      ) >> Shift_v
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator/=(FixedPoint const & rhs)
  {
    using LargerInt_t = Larger_Int<BitsUsed_v>;
    static constexpr uintmax_t Shift_v = Q_N;

    m_Int = Int_t(
      (LargerInt_t(m_Int) << Shift_v) / LargerInt_t(rhs.m_Int)
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  bool FixedPoint<M,N,Int>::operator==(FixedPoint const & rhs) const
  {
    return m_Int == rhs.m_Int;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  bool FixedPoint<M,N,Int>::operator!=(FixedPoint const & rhs) const
  {
    return !(*this == rhs);
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  bool FixedPoint<M,N,Int>::operator>(FixedPoint const & rhs) const
  {
    return m_Int > rhs.m_Int;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  bool FixedPoint<M,N,Int>::operator>=(FixedPoint const & rhs) const
  {
    return !(*this < rhs);
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  bool FixedPoint<M,N,Int>::operator<(FixedPoint const & rhs) const
  {
    return m_Int < rhs.m_Int;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  bool FixedPoint<M,N,Int>::operator<=(FixedPoint const & rhs) const
  {
    return !(*this > rhs);
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  bool FixedPoint<M,N,Int>::operator!() const
  {
    return m_Int == 0;
  }

    //////// Templated FixedPoint Operators ////////

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  FixedPoint<M,N,Int> &
    FixedPoint<M,N,Int>::operator=(FixedPoint<M2,N2,Int2> const & rhs)
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;

    m_Int = CheckSaturation<Q_M,Q_N,Int_t>(
      Q_N > RHS_t::Q_N ?
        Int_t(rhs.m_Int) << (Q_N - RHS_t::Q_N) :
        typename RHS_t::Int_t(rhs.m_Int >> (RHS_t::Q_N - Q_N))
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  std::conditional_t<(N>N2), FixedPoint<M,N,Int>, FixedPoint<M2,N2,Int2>>
    FixedPoint<M,N,Int>::operator+(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using Result_t = std::conditional_t<
                       (Q_N>RHS_t::Q_N),
                       FixedPoint<Q_M,Q_N,Int_t>,
                       FixedPoint<RHS_t::Q_M,RHS_t::Q_N,typename RHS_t::Int_t>
                     >;
    using LargerInt_t = Larger_Int<(N>N2) ? BitsUsed_v : RHS_t::BitsUsed_v>;

    Result_t result;

    result.m_Int = SaturationOption<
                     Result_t::Q_M,
                     Result_t::Q_N,
                     typename Result_t::Int_t
                   >(
      Q_N > RHS_t::Q_N ?
        LargerInt_t(m_Int) + (LargerInt_t(rhs.m_Int) << (Q_N - RHS_t::Q_N)) :
        (LargerInt_t(m_Int) << (RHS_t::Q_N - Q_N)) + LargerInt_t(rhs.m_Int)
    );

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  std::conditional_t<(N>N2), FixedPoint<M,N,Int>, FixedPoint<M2,N2,Int2>>
    FixedPoint<M,N,Int>::operator-(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using Result_t = std::conditional_t<
                       (N>N2),
                       FixedPoint<M,N,Int>,
                       FixedPoint<M2,N2,Int2>
                     >;
    using LargerInt_t = Larger_Int<
                          (Q_M+Q_N > RHS_t::Q_M+RHS_t::Q_N) ?
                            Q_M+Q_N+1 : RHS_t::Q_M+RHS_t::Q_N+1
                        >;

    Result_t result;
    LargerInt_t temp;

    temp = Q_N > RHS_t::Q_N ?
      m_Int - LargerInt_t(rhs.m_Int << (Q_N - RHS_t::Q_N)) :
      LargerInt_t(m_Int << (RHS_t::Q_N - Q_N)) - rhs.m_Int;

    result.m_Int = SaturationOption<
                     Result_t::Q_M,
                     Result_t::Q_N,
                     typename Result_t::Int_t
                   >(temp);

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  std::conditional_t<
             (M+N == M2+N2 ? (N > N2) : (M+N > M2+N2)),
             FixedPoint<M,N,Int>,
             FixedPoint<M2,N2,Int2>
  > FixedPoint<M,N,Int>::operator*(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using Result_t = std::conditional_t<
                       (M+N == M2+N2 ? (N > N2) : (M+N > M2+N2)),
                       FixedPoint<M,N,Int_t>,
                       FixedPoint<M2,N2,typename RHS_t::Int_t>
    >;
    using LargerInt_t = Larger_Int<Result_t::BitsUsed_v>;
    static constexpr uintmax_t Shift_v = Q_N + RHS_t::Q_N - Result_t::Q_N;
    #if FIXED_POINT_ROUND
      static constexpr LargerInt_t K = 1 << (Q_N + RHS_t::Q_N - 1);
    #endif

    Result_t result;
    result.m_Int = SaturationOption<
                     Result_t::Q_M,
                     Result_t::Q_N,
                     typename Result_t::Int_t
                   >(
      ((LargerInt_t(m_Int) * LargerInt_t(rhs.m_Int))
      #if FIXED_POINT_ROUND
        + K
      #endif
      ) >> Shift_v
    );

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2, uintmax_t N2, typename Int2>
  std::conditional_t<
             (M+N == M2+N2 ? (N > N2) : (M+N > M2+N2)),
             FixedPoint<M,N,Int>,
             FixedPoint<M2,N2,Int2>
  > FixedPoint<M,N,Int>::operator/(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using Result_t = std::conditional_t<
                       (M+N == M2+N2 ? (N > N2) : (M+N > M2+N2)),
                       FixedPoint<M,N,Int_t>,
                       FixedPoint<M2,N2,typename RHS_t::Int_t>
    >;
    using LargerInt_t = Larger_Int<
      (BitsUsed_v > RHS_t::BitsUsed_v ?
        BitsUsed_v :
        RHS_t::BitsUsed_v)
    >;
    static constexpr uintmax_t Shift_v = Q_N > RHS_t::Q_N ? 0 : RHS_t::Q_N-Q_N;

    Result_t result;

    result.m_Int = SaturationOption<
                     Result_t::Q_M,
                     Result_t::Q_N,
                     typename Result_t::Int_t
                   >(
      ((LargerInt_t(m_Int) << RHS_t::Q_N) / LargerInt_t(rhs.m_Int)) << Shift_v
    );

    return result;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  FixedPoint<M,N,Int> &
    FixedPoint<M,N,Int>::operator+=(FixedPoint<M2,N2,Int2> const & rhs)
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using LargerInt_t = Larger_Int<BitsUsed_v>;

    m_Int = SaturationOption<Q_M, Q_N, Int_t>(
      Q_N > RHS_t::Q_N ?
        LargerInt_t(m_Int) + (LargerInt_t(rhs.m_Int) << (Q_N - RHS_t::Q_N)) :
        LargerInt_t(m_Int) + (LargerInt_t(rhs.m_Int >> (RHS_t::Q_N - Q_N)))
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  FixedPoint<M,N,Int> &
    FixedPoint<M,N,Int>::operator-=(FixedPoint<M2,N2,Int2> const & rhs)
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using LargerInt_t = Larger_Int<BitsUsed_v>;

    m_Int = SaturationOption<Q_M, Q_N, Int_t>(
      Q_N > RHS_t::Q_N ?
        LargerInt_t(m_Int) - (LargerInt_t(rhs.m_Int) << (Q_N - RHS_t::Q_N)) :
        LargerInt_t(m_Int) - (LargerInt_t(rhs.m_Int << (RHS_t::Q_N - Q_N)))
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  FixedPoint<M,N,Int> &
    FixedPoint<M,N,Int>::operator*=(FixedPoint<M2,N2,Int2> const & rhs)
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using LargerInt_t = std::conditional_t<
                          (M+N == M2+N2 ? (N > N2) : (M+N > M2+N2)),
                          Int_t,
                          typename RHS_t::Int_t
    >;
    static constexpr uintmax_t Shift_v = RHS_t::Q_N;
    #if FIXED_POINT_ROUND
      static constexpr LargerInt_t K = 1 << (Q_N + RHS_t::Q_N - 1);
    #endif

    m_Int = SaturationOption<Q_M, Q_N, Int_t>(
      ((LargerInt_t(m_Int) * LargerInt_t(rhs.m_Int))
      #if FIXED_POINT_ROUND
        + K
      #endif
      ) >> Shift_v
    );

    return *this;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2, uintmax_t N2, typename Int2>
  FixedPoint<M,N,Int> & FixedPoint<M,N,Int>::operator/=(FixedPoint<M2,N2,Int2> const & rhs)
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using LargerInt_t = Larger_Int<
      (BitsUsed_v > RHS_t::BitsUsed_v ?
        BitsUsed_v :
        RHS_t::BitsUsed_v)
    >;
    m_Int = SaturationOption<Q_M, Q_N, Int_t>(
      (LargerInt_t(m_Int) << RHS_t::Q_N) / LargerInt_t(rhs.m_Int)
    );

    return *this;
  }

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wshift-count-overflow"
#elif defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wshift-count-overflow"
#elif defined (_MSC_VER)
  #pragma warning(push)
  #pragma warning(disable: 4127)
#endif
  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  bool FixedPoint<M,N,Int>::operator==(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using RHS_Int_t = typename RHS_t::Int_t;

    return (Q_N > RHS_t::Q_N) ?
             (RHS_Int_t(m_Int >> (Q_N - RHS_t::Q_N)) == rhs.m_Int) :
             (m_Int == Int_t(rhs.m_Int >> (RHS_t::Q_N - Q_N)));
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  bool FixedPoint<M,N,Int>::operator!=(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    return !(*this == rhs);
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  bool FixedPoint<M,N,Int>::operator>(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using RHS_Int_t = typename RHS_t::Int_t;

    return (Q_N > RHS_t::Q_N) ?
             (RHS_Int_t(m_Int >> (Q_N - RHS_t::Q_N)) > rhs.m_Int) :
             (m_Int > Int_t(rhs.m_Int >> (RHS_t::Q_N - Q_N)));
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  bool FixedPoint<M,N,Int>::operator>=(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    return !(*this < rhs);
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  bool FixedPoint<M,N,Int>::operator<(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    using RHS_t = FixedPoint<M2,N2,Int2>;
    using RHS_Int_t = typename RHS_t::Int_t;

    return (Q_N > RHS_t::Q_N) ?
      (RHS_Int_t(m_Int >> (Q_N - RHS_t::Q_N)) < rhs.m_Int) :
      (m_Int < Int_t(rhs.m_Int >> (RHS_t::Q_N - Q_N)));
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M2,uintmax_t N2,typename Int2>
  bool FixedPoint<M,N,Int>::operator<=(FixedPoint<M2,N2,Int2> const & rhs) const
  {
    return !(*this > rhs);
  }
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#elif defined(__clang__)
  #pragma clang diagnostic pop
#elif defined(_MSC_VER)
  #pragma warning(pop)
#endif

    //////// Class Functions ////////

  template<uintmax_t M, uintmax_t N, typename Int>
  template<uintmax_t M_out, uintmax_t N_out, typename Int_out, typename Int_in>
  Int_out FixedPoint<M,N,Int>::SaturationOption(Int_in integer)
  {
    #if !defined(FIXED_POINT_FAST)
      return CheckSaturation<M_out,N_out,Int_out,Int_in>(integer);
    #else
      return Int_out(integer);
    #endif
  }

#ifdef FIXED_POINT_RAW_ACCESS
  template<uintmax_t M, uintmax_t N, typename Int>
  Int & FixedPoint<M,N,Int>::Data()
  {
    return m_Int;
  }

  template<uintmax_t M, uintmax_t N, typename Int>
  Int const & FixedPoint<M,N,Int>::Data() const
  {
    return m_Int;
  }
#endif

POP_WARNINGS()

  //////////////// Namespace Functions ////////////////

PUSH_WARNINGS()
  // Disable warning for integer overflow (overflow is intensional)
MSVC_DISABLE_WARNING(4307)
  // Disable warning on cast of -1 to an unsigned value
MSVC_DISABLE_WARNING(4309)

  template<uintmax_t M_out, uintmax_t N_out, typename Int_out, typename Int_in>
  Int_out CheckSaturation(Int_in in)
  {
    static Int_in const mask = static_cast<Int_in>(
      ~(static_cast<MAX_UINT>(-1) << (M_out + N_out))
    );

    if(in > mask)
    {
      in = Int_out(mask);
    }
    else if(in < static_cast<Int_in>(-static_cast<MAX_INT>(static_cast<MAX_UINT>(mask) + 1)))
    {
      in = static_cast<Int_in>(-static_cast<MAX_INT>(static_cast<MAX_UINT>(mask) + 1));
    }

    return Int_out(in);
  }

POP_WARNINGS()

} // namespace FixedPoint

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
FixedPoint::FixedPoint<M,N,Int> operator+(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return FixedPoint::FixedPoint<M,N,Int>(lhs) + rhs;
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
FixedPoint::FixedPoint<M,N,Int> operator-(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return FixedPoint::FixedPoint<M,N,Int>(lhs) - rhs;
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
FixedPoint::FixedPoint<M,N,Int> operator*(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return FixedPoint::FixedPoint<M,N,Int>(lhs) * rhs;
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
FixedPoint::FixedPoint<M,N,Int> operator/(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return FixedPoint::FixedPoint<M,N,Int>(lhs) / rhs;
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
Value & operator+=(Value & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return lhs = Value(FixedPoint::FixedPoint<M,N,Int>(lhs) + rhs);
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
Value & operator-=(Value & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return lhs = Value(FixedPoint::FixedPoint<M,N,Int>(lhs) - rhs);
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
Value & operator*=(Value & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return lhs = Value(FixedPoint::FixedPoint<M,N,Int>(lhs) * rhs);
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
Value & operator/=(Value & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return lhs = Value(FixedPoint::FixedPoint<M,N,Int>(lhs) / rhs);
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
bool operator==(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return FixedPoint::FixedPoint<M,N,Int>(lhs) == rhs;
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
bool operator!=(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return !(lhs == rhs);
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
bool operator>(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return FixedPoint::FixedPoint<M,N,Int>(lhs) > rhs;
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
bool operator>=(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return !(lhs < rhs);
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
bool operator<(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return FixedPoint::FixedPoint<M,N,Int>(lhs) < rhs;
}

template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  >
>
bool operator<=(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs)
{
  return !(lhs > rhs);
}

// Private Functions                      //////////////////////////////////////

#endif // __FIXED_POINT_INL
