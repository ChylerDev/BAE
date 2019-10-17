/*! ****************************************************************************
\file             FixedPoint.hpp
\author           Chyler Morrison
\par    Email:    contact\@chyler.info
\par    Project:  FixedPoint

\copyright        Copyright © 2019 Chyler
*******************************************************************************/

#ifndef __FIXED_POINT_HPP
#define __FIXED_POINT_HPP

// Include Files                ////////////////////////////////////////////////

#include <cstdint>

#include <type_traits>

// Public Macros                ////////////////////////////////////////////////

#ifndef TO_STR
  #define TO_STR(x) #x
#endif

#ifndef DO_PRAGMA
  #if defined(_MSC_VER)
    #define DO_PRAGMA(x) __pragma(x)
  #elif defined(__clang__) || defined(__GNUC__)
    #define DO_PRAGMA(x) _Pragma(TO_STR(x))
  #else
    #define DO_PRAGMA(x)
  #endif
#endif

#ifndef PUSH_WARNINGS
  #if defined(_MSC_VER)
    #define PUSH_WARNINGS() DO_PRAGMA(warning(push))
  #elif defined(__clang__)
    #define PUSH_WARNINGS() DO_PRAGMA(clang diagnostic push)
  #elif defined(__GNUC__)
    #define PUSH_WARNINGS() DO_PRAGMA(GCC diagnostic push)
  #else
    #define PUSH_WARNINGS()
  #endif
#endif

#ifndef MSVC_DISABLE_WARNING
  #if defined(_MSC_VER)
    #define MSVC_DISABLE_WARNING(x) DO_PRAGMA(warning(disable: x))
  #else
    #define MSVC_DISABLE_WARNING(x)
  #endif
#endif
#ifndef CLANG_DISABLE_WARNING
  #if defined(__clang__)
    #define CLANG_DISABLE_WARNING(x) DO_PRAGMA(clang diagnostic ignored #x)
  #else
    #define CLANG_DISABLE_WARNING(x)
  #endif
#endif
#ifndef GCC_DISABLE_WARNING
  #if defined(__GNUC__)
    #define GCC_DISABLE_WARNING(x) DO_PRAGMA(GCC diagnostic ignored #x)
  #else
    #define GCC_DISABLE_WARNING(x)
  #endif
#endif

#ifndef POP_WARNINGS
  #if defined(_MSC_VER)
    #define POP_WARNINGS() DO_PRAGMA(warning(pop))
  #elif defined(__clang__)
    #define POP_WARNINGS() DO_PRAGMA(clang diagnostic pop)
  #elif defined(__GNUC__)
    #define POP_WARNINGS() DO_PRAGMA(GCC diagnostic pop)
  #else
    #define POP_WARNINGS()
  #endif
#endif

#ifndef MAX_INT
  #define MAX_INT intmax_t
#endif

#ifndef MAX_UINT
  #define MAX_UINT uintmax_t
#endif

#ifndef FIXED_POINT_ROUND
  #define FIXED_POINT_ROUND 0
#endif

// Forward References           ////////////////////////////////////////////////

// Public Enums                 ////////////////////////////////////////////////

// Public Objects               ////////////////////////////////////////////////

namespace FixedPoint
{

  template<uintmax_t num_bits>
  using Fits_Int = std::conditional_t<
                     (num_bits > 8),
                     std::conditional_t<
                       (num_bits > 16),
                       std::conditional_t<
                         (num_bits > 32),
                         int64_t,
                         int32_t
                       >,
                       int16_t
                     >,
                     int8_t
                   >;

  template<uintmax_t num_bits>
  using Larger_Int = std::conditional_t<
                       (num_bits > 8),
                       std::conditional_t<
                         (num_bits > 16),
                         int64_t,
                         int32_t
                       >,
                       int16_t
                     >;

  template<uintmax_t num_bits>
  using Fits_Uint = std::conditional_t<
                      (num_bits > 8),
                      std::conditional_t<
                        (num_bits > 16),
                        std::conditional_t<
                          (num_bits > 32),
                          uint64_t,
                          uint32_t
                        >,
                        uint16_t
                      >,
                      uint8_t
                    >;

  template<uintmax_t num_bits>
  using Larger_Uint = std::conditional_t<
                        (num_bits > 8),
                        std::conditional_t<
                          (num_bits > 16),
                          uint64_t,
                          uint32_t
                        >,
                        uint16_t
                      >;

  template<uintmax_t M2, uintmax_t N2, typename Int2>
  class FixedPoint;

  class QFormat
  {};

  /*! **************************************************************************
  \brief
    Class for computing Q-format arithmetic operations.
    Note: division operators not implemented due to inherent ineeficiency of
          fixed point division.

  \tparam M
    The number of bits representing integer values.

  \tparam N
    The number of bits representing fractional values.

  \tparam Int
    The storage type to hold the Q-format number. Must be M+N+1 bits or larger
    (eg. Q3.12 needs type int16_t or larger since 3 + 12 + 1(sign bit) = 16).

    Defaulted to the FixedPointFits_Int with M+N+1 as the template parameter.
  *****************************************************************************/
  template<uintmax_t M, uintmax_t N, typename Int = Fits_Int<M+N+1>>
  class FixedPoint : public QFormat
  {
  public:

    using Int_t = Int;
    static constexpr uintmax_t Q_M = M;
    static constexpr uintmax_t Q_N = N;
    static constexpr uintmax_t BitsUsed_v = Q_M+Q_N+1;

  private:

    template<uintmax_t M2, uintmax_t N2, typename Int2>
    friend class FixedPoint;

    // Members              ///////////////////////

    Int_t m_Int;

  public:

    // Con-/De- structors   ///////////////////////

    /*! ************************************************************************
    \brief
      Default constructor, data initialized to 0.
    ***************************************************************************/
    inline FixedPoint();

    /*! ************************************************************************
    \brief
      Copy constructor, copies from a FixedPoint object with the same template
      parameters.

    \param other
      Object being copied.
    ***************************************************************************/
    inline FixedPoint(FixedPoint const & other);

    /*! ************************************************************************
    \brief
      Copy constructor, copies from a FixedPoint object with different template
      parameters.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param other
      Object being copied.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline FixedPoint(FixedPoint<M2,N2,Int2> const & other);

    /*! ************************************************************************
    \brief
      Copy constructor.

    \tparam Value
      Any type.

    \param val
      Object being copied.
    ***************************************************************************/
    template<typename Value,
      typename std::enable_if_t<
        std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
      > = 0
    >
    inline FixedPoint(Value const & val);

    /*! ************************************************************************
    \brief
      Default constructor
    ***************************************************************************/
    inline ~FixedPoint();

    // Operators            ///////////////////////

    /*! ************************************************************************
    \brief
      Converts the type to a boolean.

    \return
      m_Int != 0.
    ***************************************************************************/
    inline explicit operator bool() const;

    /*! ************************************************************************
    \brief
      Converts the type to a given non-FixedPoint type.

    \tparam Value
      Any type.

    \return
      The converted type.
    ***************************************************************************/
    template<typename Value,
      typename std::enable_if_t<
        std::conjunction_v<
          std::negation<std::is_base_of<FixedPoint::QFormat, Value>>,
          std::negation<std::is_same<Value, bool>>
        >,
        int
      > = 0
    >
    inline explicit operator Value();

    template<typename Value,
      typename std::enable_if_t<
        std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
      > = 0
    >
    inline FixedPoint & operator=(Value const &);

    /*! ************************************************************************
    \brief
      Arithmetic right shift operator.

    \param shift
      The amount to shift by.

    \return
      New FixedPoint object containing the shifted value.
    ***************************************************************************/
    inline FixedPoint operator>>(unsigned shift);

    /*! ************************************************************************
    \brief
      Arithmetic left shift operator.

    \param shift
      The amount to shift by.

    \return
      New FixedPoint object containing the shifted value.
    ***************************************************************************/
    inline FixedPoint operator<<(unsigned shift);

    /*! ************************************************************************
    \brief
      Arithmetic right shift assignment operator.

    \param shift
      The amount to shift by.

    \return
      *this.
    ***************************************************************************/
    inline FixedPoint & operator>>=(unsigned shift);

    /*! ************************************************************************
    \brief
      Arithmetic left shift assignment operator.

    \param shift
      The amount to shift by.

    \return
      *this.
    ***************************************************************************/
    inline FixedPoint & operator<<=(unsigned shift);

    /*! ************************************************************************
    \brief
      Unary plus. Forces value to positive, e.g. taking the absolute value.

    \return
      New FixedPoint object with the result of the operation.
    ***************************************************************************/
    inline FixedPoint operator+() const;

    /*! ************************************************************************
    \brief
      Unary minus.

    \return
      New FixedPoint object with the result of the operation.
    ***************************************************************************/
    inline FixedPoint operator-() const;

    /*! ************************************************************************
    \brief
      Prefix increment.

    \return
      *this.
    ***************************************************************************/
    inline FixedPoint & operator++();

    /*! ************************************************************************
    \brief
      Postfix increment.

    \return
      *this value before the increment.
    ***************************************************************************/
    inline FixedPoint operator++(int);

    /*! ************************************************************************
    \brief
      Prefix decrement.

    \return
      *this.
    ***************************************************************************/
    inline FixedPoint & operator--();

    /*! ************************************************************************
    \brief
      Postfix decrement.

    \return
      *this value before the increment.
    ***************************************************************************/
    inline FixedPoint operator--(int);

    /*! ************************************************************************
    \brief
      Logical NOT operator.

    \return
      The result.
    ***************************************************************************/
    inline bool operator!() const;

    //////// FixedPoint Operators ////////

    /*! ************************************************************************
    \brief
      Assignment operator, copies from a FixedPoint object with the same
      template parameters.

    \param rhs
      Object being copied.

    \return
      *this.
    ***************************************************************************/
    inline FixedPoint & operator=(FixedPoint const & rhs);

    /*! ************************************************************************
    \brief
      Addition operator, adds two FixedPoint objects with the same template
      parameters.

    \param rhs
      Other object being operated on.

    \return
      New fixed point object with the result of the operation.
    ***************************************************************************/
    inline FixedPoint operator+(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Subtraction operator, subtracts two FixedPoint objects with the same
      template parameters.

    \param rhs
      Other object being operated on.

    \return
      New fixed point object with the result of the operation.
    ***************************************************************************/
    inline FixedPoint operator-(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Multiplication operator, multiplies two FixedPoint objects with the same
      template parameters.

    \param rhs
      Other object being operated on.

    \return
      New fixed point object with the result of the operation.
    ***************************************************************************/
    inline FixedPoint operator*(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Division operator, divides two FixedPoint objects with the same template
      parameters. This function assumes that the operands are such that
      (abs(rhs) > abs(*this)). This is the most common use of division with
      FixedPoint, and removes complicated checks for overflow conditions.

      Due to the general inefficiency of division it is recommended where
      possible to use a multiplication of a recipricol if the right-hand operand
      is a constant.

      NOTE: There are no checks for if the operands satisfy
      (abs(rhs) > abs(*this)) as that wastes extra CPU cycles and branching.

    \param rhs
      Other object being operated on.

    \return
      New FixedPoint object with the result of the operation.
    ***************************************************************************/
    inline FixedPoint operator/(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Addition assignment operator, adds two FixedPoint objects with the same
      template parameters.

    \param rhs
      Other object being operated on.

    \return
      *this
    ***************************************************************************/
    inline FixedPoint & operator+=(FixedPoint const & rhs);

    /*! ************************************************************************
    \brief
      Subtraction assignment operator, subtracts two FixedPoint objects with the
      same template parameters.

    \param rhs
      Other object being operated on.

    \return
      *this
    ***************************************************************************/
    inline FixedPoint & operator-=(FixedPoint const & rhs);

    /*! ************************************************************************
    \brief
      Multiplication assignment operator, multiplies two FixedPoint objects with
      the same template parameters.

    \param rhs
      Other object being operated on.

    \return
      *this
    ***************************************************************************/
    inline FixedPoint & operator*=(FixedPoint const & rhs);

    /*! ************************************************************************
    \brief
      Division assignment operator, divides two FixedPoint objects with the same
      template parameters. This function assumes that the operands are such that
      (abs(rhs) > abs(*this)). This is the most common use of division with
      FixedPoint, and removes complicated checks for overflow conditions.

      Due to the general inefficiency of division it is recommended where
      possible to use a multiplication of a recipricol if the right-hand operand
      is a constant.

      NOTE: There are no checks for if the operands satisfy
      (abs(rhs) > abs(*this)) as that wastes extra CPU cycles and branching.

    \param rhs
      Other object being operated on.

    \return
      *this
    ***************************************************************************/
    inline FixedPoint & operator/=(FixedPoint const & rhs);

    /*! ************************************************************************
    \brief
      Equal operator.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    inline bool operator==(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Not-equal operator.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    inline bool operator!=(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Greater-than operator.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    inline bool operator>(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Greater-than equal operator.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    inline bool operator>=(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Less-than operator.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    inline bool operator<(FixedPoint const & rhs) const;

    /*! ************************************************************************
    \brief
      Less-than equal operator.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    inline bool operator<=(FixedPoint const & rhs) const;

    //////// Templated FixedPoint Operators ////////

    /*! ************************************************************************
    \brief
      Assignment operator, copies from a FixedPoint object with different
      template parameters.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Object being copied.

    \return
      *this.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline FixedPoint & operator=(FixedPoint<M2,N2,Int2> const & rhs);

    /*! ************************************************************************
    \brief
      Addition operator, adds two FixedPoint objects with different template
      parameters.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Object being operated on.

    \return
      New FixedPoint object with the result of the operation.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline std::conditional_t<
             (N>N2),
             FixedPoint<M,N,Int>,
             FixedPoint<M2,N2,Int2>
      > operator+(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Subtraction operator, subtracts two FixedPoint objects with different
      template parameters.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Object being operated on.

    \return
      New FixedPoint object with the result of the operation.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline std::conditional_t<
             (N>N2),
             FixedPoint<M,N,Int>,
             FixedPoint<M2,N2,Int2>
    > operator-(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Multiplication operator, multiplies two FixedPoint objects with different
      template parameters.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Object being operated on.

    \return
      New FixedPoint object with the result of the operation.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline std::conditional_t<
             (M+N == M2+N2 ? (N>N2) : (M+N > M2+N2)),
             FixedPoint<M,N,Int>,
             FixedPoint<M2,N2,Int2>
    > operator*(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Division operator, divides two FixedPoint objects with different template
      parameters. This function assumes that the operands are such that
      (abs(rhs) > abs(*this)). This is the most common use of division with
      FixedPoint, and removes complicated checks for overflow conditions.

      Due to the general inefficiency of division it is recommended where
      possible to use a multiplication of a recipricol if the right-hand operand
      is a constant.

      NOTE: There are no checks for if the operands satisfy
      (abs(rhs) > abs(*this)) as that wastes extra CPU cycles and branching.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Other object being operated on.

    \return
      New FixedPoint object with the result of the operation.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline std::conditional_t<
             (M+N == M2+N2 ? (N>N2) : (M+N > M2+N2)),
             FixedPoint<M,N,Int>,
             FixedPoint<M2,N2,Int2>
    > operator/(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Addition assignment operator, adds two FixedPoint objects with different
      template parameters.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Object being operated on.

    \return
      *this
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline FixedPoint & operator+=(FixedPoint<M2,N2,Int2> const & rhs);

    /*! ************************************************************************
    \brief
      Subtraction assignment operator, subtracts two FixedPoint objects with
      different template parameters.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Object being operated on.

    \return
      *this
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline FixedPoint & operator-=(FixedPoint<M2,N2,Int2> const & rhs);

    /*! ************************************************************************
    \brief
      Multiplication assignment operator, multiplies two FixedPoint objects with
      different template parameters.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Object being operated on.

    \return
      *this
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline FixedPoint & operator*=(FixedPoint<M2,N2,Int2> const & rhs);

    /*! ************************************************************************
    \brief
      Division assignment operator, divides two FixedPoint objects with
      different template parameters. This function assumes that the operands are
      such that (abs(rhs) > abs(*this)). This is the most common use of division
      with FixedPoint, and removes complicated checks for overflow conditions.

      Due to the general inefficiency of division it is recommended where
      possible to use a multiplication of a recipricol if the right-hand operand
      is a constant.

      NOTE: There are no checks for if the operands satisfy
      (abs(rhs) > abs(*this)) as that wastes extra CPU cycles and branching.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      Other object being operated on.

    \return
      *this
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline FixedPoint<M,N,Int> & operator/=(FixedPoint<M2,N2,Int2> const & rhs);

    /*! ************************************************************************
    \brief
      Equal operator.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline bool operator==(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Not-equal operator.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline bool operator!=(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Greater-than operator.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline bool operator>(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Greater-than equal operator.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline bool operator>=(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Less-than operator.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline bool operator<(FixedPoint<M2,N2,Int2> const & rhs) const;

    /*! ************************************************************************
    \brief
      Less-than equal operator.

    \tparam M2
      The number of bits representing integer values.

    \tparam N2
      The number of bits representing fractional values.

    \tparam Int2
      The storage type to hold the Q-format number. Must be greater than M+N+1
      bits large (eg. Q3.12 needs type int16_t or larger since
      3+12+1(sign bit)=16).
      Note: Must be a signed type.

    \param rhs
      The other object.

    \return
      The result.
    ***************************************************************************/
    template<uintmax_t M2, uintmax_t N2, typename Int2>
    inline bool operator<=(FixedPoint<M2,N2,Int2> const & rhs) const;

    // Accossors/Mutators   ///////////////////////

  #ifdef FIXED_POINT_RAW_ACCESS
    /*! ************************************************************************
    \brief
      Accesses the internal integer containing the fixed-point number.

    \return
      Reference to the internal integer.
    ***************************************************************************/
    inline Int_t & Data(void);

    /*! ************************************************************************
    \brief
      Accesses the internal integer containing the fixed-point number.

    \return
      Reference to the internal integer.
    ***************************************************************************/
    inline Int_t const & Data(void) const;
  #endif

    // Functions            ///////////////////////

  private:

    // Functions                  ///////////////////////

    template<uintmax_t M_out, uintmax_t N_out, typename Int_out, typename Int_in>
    static inline Int_out SaturationOption(Int_in integer);

  }; // class FixedPoint

  using Audio24Bit_t = FixedPoint<7,24,int32_t>;
  using Audio16Bit_t = FixedPoint<0,15,int16_t>;
  using Audio12Bit_t = FixedPoint<3,12,int16_t>;
  using Audio8Bit_t = FixedPoint<0,7,int8_t>; // Signed 8-bit. 0 is unity gain

} // namespace FixedPoint

// Public Functions             ////////////////////////////////////////////////

namespace FixedPoint
{

  template<uintmax_t M_out, uintmax_t N_out, typename Int_out, typename Int_in>
  inline Int_out CheckSaturation(Int_in integer);

} // namespace FixedPoint

/*! ****************************************************************************
\brief
  Addition operator, adds a Value object and a FixedPoint object.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  New fixed point object with the result of the operation.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline FixedPoint::FixedPoint<M,N,Int> operator+(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Subtraction operator, subtracts a Value object and a FixedPoint object.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  New fixed point object with the result of the operation.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline FixedPoint::FixedPoint<M,N,Int> operator-(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Multiplication operator, multiplies a Value object and a FixedPoint object.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  New fixed point object with the result of the operation.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline FixedPoint::FixedPoint<M,N,Int> operator*(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Division operator, divides a Value object and a FixedPoint object. This
  function assumes that the operands are such that (abs(rhs) > abs(*this)). This
  is the most common use of division with FixedPoint, and removes complicated
  checks for overflow conditions.

  Due to the general inefficiency of division it is recommended where possible
  to use a multiplication of a recipricol if the right-hand operand is a
  constant.

  NOTE: There are no checks for if the operands satisfy (abs(rhs) > abs(*this))
  as that wastes extra CPU cycles and branching.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1 bits
  large (eg. Q3.12 needs type int16_t or larger since 3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  New fixed point object with the result of the operation.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline FixedPoint::FixedPoint<M,N,Int> operator/(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Addition assignment operator, adds a Value object and a FixedPoint object.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  *lhs
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline Value & operator+=(Value & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Subtraction assignment operator, subtracts a Value object and a FixedPoint
  object.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  lhs.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline Value & operator-=(Value & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Multiplication assignment operator, multiplies a Value object and a FixedPoint
  object.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  *lhs
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline Value & operator*=(Value & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Division assignment operator, divides a Value object and a FixedPoint object.
  This function assumes that the operands are such that (abs(rhs) > abs(*this)).
  This is the most common use of division with FixedPoint, and removes
  complicated checks for overflow conditions.

  Due to the general inefficiency of division it is recommended where possible
  to use a multiplication of a recipricol if the right-hand operand is a
  constant.

  NOTE: There are no checks for if the operands satisfy (abs(rhs) > abs(*this))
  as that wastes extra CPU cycles and branching.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1 bits
  large (eg. Q3.12 needs type int16_t or larger since 3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  *lhs
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline Value & operator/=(Value & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Equal operator.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  The result.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline bool operator==(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Not-equal operator.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  The result.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline bool operator!=(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Greater-than operator.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  The result.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline bool operator>(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Greater-than equal operator.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  The result.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline bool operator>=(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Less-than operator.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  The result.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline bool operator<(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

/*! ****************************************************************************
\brief
  Less-than equal operator.

\tparam M
  The number of bits representing integer values.

\tparam N
  The number of bits representing fractional values.

\tparam Int
  The storage type to hold the Q-format number. Must be greater than M+N+1
  bits large (eg. Q3.12 needs type int16_t or larger since
  3+12+1(sign bit)=16).

\param lhs
  Left-hand object being operated on.

\param rhs
  Right-hand object being operated on.

\return
  The result.
*******************************************************************************/
template<uintmax_t M, uintmax_t N, typename Int, typename Value,
  typename std::enable_if_t<
    std::negation_v<std::is_base_of<FixedPoint::QFormat, Value>>, int
  > = 0
>
inline bool operator<=(Value const & lhs, FixedPoint::FixedPoint<M,N,Int> const & rhs);

// Inline Includes              ////////////////////////////////////////////////

#include "FixedPoint.inl"

#endif // __FIXED_POINT_HPP
