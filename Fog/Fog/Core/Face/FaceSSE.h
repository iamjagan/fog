// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_FACE_FACESSE_H
#define _FOG_CORE_FACE_FACESSE_H

// [Dependencies]
#include <Fog/Core/Build.h>
#include <Fog/Core/Face/Features.h>
#include <Fog/Core/Math.h>

#include <Fog/Core/Intrin/IntrinSSE.h>
#if defined(FOG_FACE_HAS_SSE2)
#include <Fog/Core/Intrin/IntrinSSE2.h>
#endif // FOG_FACE_HAS_SSE2

// ============================================================================
// [Fog::Face - SSE - Constants]
// ============================================================================

FOG_SSE_DECLARE_CONST_PI32_SET(m128f_sgn_mask, 0x80000000);
FOG_SSE_DECLARE_CONST_PI32_SET(m128f_num_mask, 0x7FFFFFFF);
FOG_SSE_DECLARE_CONST_PS_SET  (m128f_one     , 1.0f      );
FOG_SSE_DECLARE_CONST_PS_SET  (m128f_epsilon , Fog::MATH_EPSILON_F);

namespace Fog {
namespace Face {

//! @addtogroup Fog_Face_SSE
//! @{

// ============================================================================
// [Fog::Face - SSE - Types]
// ============================================================================

typedef __m128 m128f;

// ============================================================================
// [Fog::Face - SSE - Zero]
// ============================================================================

static FOG_INLINE void m128fZero(m128f& dst0)
{
  dst0 = _mm_setzero_ps();
}

// ============================================================================
// [Fog::Face - SSE - Load/Store]
// ============================================================================

//! @brief Load scalar SP-FP value from @a srcp to @a dst.
//!
//! @verbatim
//! dst[0] = ((float*)srcp)[0]
//! dst[1] = 0.0
//! dst[2] = 0.0
//! dst[3] = 0.0
//! @endverbatim
template<typename SrcT>
static FOG_INLINE void m128fLoad4(m128f& dst0, const SrcT* srcp)
{
  dst0 = _mm_load_ss(reinterpret_cast<const float*>(srcp));
}

//! @brief Load two SP-FP values from @a srcp to @a dst.
//!
//! @verbatim
//! dst[0] = ((float*)srcp)[0]
//! dst[1] = ((float*)srcp)[0]
//! dst[2] = {NO CHANGE}
//! dst[3] = {NO CHANGE}
//! @endverbatim
template<typename SrcT>
static FOG_INLINE void m128fLoad8Lo(m128f& dst0, const SrcT* srcp)
{
  dst0 = _mm_loadl_pi(dst0, reinterpret_cast<const __m64*>(srcp));
}

//! @brief Load two SP-FP values from @a srcp to @a dst.
//!
//! @verbatim
//! dst[0] = {NO CHANGE}
//! dst[1] = {NO CHANGE}
//! dst[2] = ((float*)srcp)[0]
//! dst[3] = ((float*)srcp)[0]
//! @endverbatim
template<typename SrcT>
static FOG_INLINE void m128fLoad8Hi(m128f& dst0, const SrcT* srcp)
{
  dst0 = _mm_loadh_pi(dst0, reinterpret_cast<const __m64*>(srcp));
}

template<typename SrcT>
static FOG_INLINE void m128fLoad16a(m128f& dst0, const SrcT* srcp)
{
  dst0 = _mm_load_ps(reinterpret_cast<const float*>(srcp));
}

template<typename SrcT>
static FOG_INLINE void m128fLoad16u(m128f& dst0, const SrcT* srcp)
{
  dst0 = _mm_loadu_ps(reinterpret_cast<const float*>(srcp));
}

template<typename DstT>
static FOG_INLINE void m128fStore4(DstT* dstp, const m128f& src0)
{
  _mm_store_ss(reinterpret_cast<float*>(dstp), src0);
}

template<typename DstT>
static FOG_INLINE void m128fStore8Lo(DstT* dstp, const m128f& src0)
{
  _mm_storel_pi(reinterpret_cast<__m64*>(dstp), src0);
}

template<typename DstT>
static FOG_INLINE void m128fStore8Hi(DstT* dstp, const m128f& src0)
{
  _mm_storeh_pi(reinterpret_cast<__m64*>(dstp), src0);
}

template<typename DstT>
static FOG_INLINE void m128fStore16a(DstT* dstp, const m128f& src0)
{
  _mm_store_ps(reinterpret_cast<float*>(dstp), src0);
}

template<typename DstT>
static FOG_INLINE void m128fStore16u(DstT* dstp, const m128f& src0)
{
  _mm_storeu_ps(reinterpret_cast<float*>(dstp), src0);
}

template<typename DstT>
static FOG_INLINE void m128fStore16nta(DstT* dstp, const m128f& src0)
{
  _mm_stream_ps(reinterpret_cast<float*>(dstp), src0);
}

// ============================================================================
// [Fog::Face - SSE - Shuffle]
// ============================================================================

//! @brief Shuffle SP-FP values from @a to @a dst.
//!
//! @verbatim
//! dst[0] = a[W]
//! dst[1] = a[X]
//! dst[2] = a[Y]
//! dst[3] = a[Z]
//! @endverbatim
template<int Z, int Y, int X, int W>
static FOG_INLINE void m128fShuffle(m128f& dst, const m128f& a)
{
#if defined(FOG_FACE_HAS_SSE2)
  dst = _mm_shuffle_epi32_f(a, _MM_SHUFFLE(Z, Y, X, W));
#else
  dst = _mm_shuffle_ps(a, a, _MM_SHUFFLE(Z, Y, X, W));
#endif // FOG_FACE_HAS_SSE2
}

//! @brief Shuffle SP-FP values from @a and @a b to @a dst.
//!
//! @verbatim
//! dst[0] = a[W]
//! dst[1] = a[X]
//! dst[2] = b[Y]
//! dst[3] = b[Z]
//! @endverbatim
template<int Z, int Y, int X, int W>
static FOG_INLINE void m128fShuffle(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_shuffle_ps(a, b, _MM_SHUFFLE(Z, Y, X, W));
}

//! @brief Swap low/high two SP-FP values.
//!
//! @verbatim
//! dst[0] := a[1]
//! dst[1] := a[0]
//! dst[2] := a[3]
//! dst[3] := a[2]
//! @endverbatim
static FOG_INLINE void m128fSwapXY(m128f& dst, const m128f& a)
{
  m128fShuffle<2, 3, 0, 1>(dst, a);
}

//! @brief Extend SP-FP value at @a into all packed ones.
//!
//! @verbatim
//! dst[0] := a[0]
//! dst[1] := a[0]
//! dst[2] := a[0]
//! dst[3] := a[0]
//! @endverbatim
static FOG_INLINE void m128fExtendSS(m128f& dst, const m128f& a)
{
  m128fShuffle<0, 0, 0, 0>(dst, a);
}

//! @brief Select and interleave the lower two SP-FP values from @a a and @a b.
//!
//! @verbatim
//! dst[0] = a[0]
//! dst[1] = b[0]
//! dst[2] = a[1]
//! dst[3] = b[1]
//! @endverbatim
static FOG_INLINE void m128fUnpackLo(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_unpacklo_ps(a, b);
}

//! @brief Select and interleave the upper two SP-FP values from @a a and @a b.
//!
//! @verbatim
//! dst[0] = a[2]
//! dst[1] = b[2]
//! dst[2] = a[3]
//! dst[3] = b[3]
//! @endverbatim
static FOG_INLINE void m128fUnpackHi(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_unpackhi_ps(a, b);
}

//! @verbatim
//! dst[0] := b[0]
//! dst[1] := b[1]
//! dst[2] := a[0]
//! dst[3] := a[1]
//! @endverbatim
static FOG_INLINE void m128fMoveLH(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_movelh_ps(a, b);
}

//! @verbatim
//! dst[0] := b[2]
//! dst[1] := b[3]
//! dst[2] := a[2]
//! dst[3] := a[3]
//! @endverbatim
static FOG_INLINE void m128fMoveHL(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_movehl_ps(a, b);
}

// ============================================================================
// [Fog::Face - SSE - Add]
// ============================================================================

//! @brief Add scalar SP-FP value of @b to @a and store the result into @a dst.
//!
//! @verbatim
//! dst[0] = a[0] + b[0]
//! dst[1] = a[1]
//! dst[2] = a[2]
//! dst[3] = a[3]
//! @endverbatim
static FOG_INLINE void m128fAddSS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_add_ss(a, b);
}

//! @brief Add packed SP-FP values of @b to @a and store the result into @a dst.
//!
//! @verbatim
//! dst[0] = a[0] + b[0]
//! dst[1] = a[1] + b[1]
//! dst[2] = a[2] + b[2]
//! dst[3] = a[3] + b[3]
//! @endverbatim
static FOG_INLINE void m128fAddPS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_add_ps(a, b);
}

// ============================================================================
// [Fog::Face - SSE - Sub]
// ============================================================================

static FOG_INLINE void m128fSubSS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_sub_ss(a, b);
}

static FOG_INLINE void m128fSubPS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_sub_ps(a, b);
}

// ============================================================================
// [Fog::Face - SSE - Mul]
// ============================================================================

static FOG_INLINE void m128fMulSS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_mul_ss(a, b);
}

static FOG_INLINE void m128fMulPS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_mul_ps(a, b);
}

// ============================================================================
// [Fog::Face - SSE - Div]
// ============================================================================

static FOG_INLINE void m128fDivSS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_div_ss(a, b);
}

static FOG_INLINE void m128fDivPS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_div_ps(a, b);
}

// ============================================================================
// [Fog::Face - SSE - Sqrt]
// ============================================================================

static FOG_INLINE void m128fSqrtSS(m128f& dst, const m128f& a)
{
  dst = _mm_sqrt_ss(a);
}

static FOG_INLINE void m128fSqrtPS(m128f& dst, const m128f& a)
{
  dst = _mm_sqrt_ps(a);
}

// ============================================================================
// [Fog::Face - SSE - Reciprocal]
// ============================================================================

static FOG_INLINE void m128fRcpSS(m128f& dst, const m128f& a)
{
  dst = _mm_rcp_ss(a);
}

static FOG_INLINE void m128fRcpPS(m128f& dst, const m128f& a)
{
  dst = _mm_rcp_ps(a);
}

// ============================================================================
// [Fog::Face - SSE - Sqrt-Reciprocal]
// ============================================================================

static FOG_INLINE void m128fSqrtRcpSS(m128f& dst, const m128f& a)
{
  dst = _mm_rsqrt_ss(a);
}

static FOG_INLINE void m128fSqrtRcpPS(m128f& dst, const m128f& a)
{
  dst = _mm_rsqrt_ps(a);
}

// ============================================================================
// [Fog::Face - SSE - Min/Max]
// ============================================================================

static FOG_INLINE void m128fMinSS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_min_ss(a, b);
}

static FOG_INLINE void m128fMinPS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_min_ps(a, b);
}

static FOG_INLINE void m128fMaxSS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_max_ss(a, b);
}

static FOG_INLINE void m128fMaxPS(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_max_ps(a, b);
}

// ============================================================================
// [Fog::Face - SSE - BitOps]
// ============================================================================

static FOG_INLINE void m128fAnd(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_and_ps(a, b);
}

static FOG_INLINE void m128fAndNot(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_andnot_ps(a, b);
}

static FOG_INLINE void m128fOr(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_or_ps(a, b);
}

static FOG_INLINE void m128fXor(m128f& dst, const m128f& a, const m128f& b)
{
  dst = _mm_xor_ps(a, b);
}

// ============================================================================
// [Fog::Face - SSE - Epsilon]
// ============================================================================

static FOG_INLINE void m128fEpsilonSS(m128f& dst, const m128f& a)
{
  m128f sgn;
  sgn = FOG_SSE_GET_CONST_PS(m128f_sgn_mask);
  sgn = _mm_and_ps(sgn, a);

  dst = _mm_and_ps(a, FOG_SSE_GET_CONST_PS(m128f_num_mask));
  dst = _mm_max_ss(dst, FOG_SSE_GET_CONST_PS(m128f_epsilon));
  dst = _mm_or_ps(dst, sgn);
}

static FOG_INLINE void m128fEpsilonPS(m128f& dst, const m128f& a)
{
  m128f sgn;
  sgn = FOG_SSE_GET_CONST_PS(m128f_sgn_mask);
  sgn = _mm_and_ps(sgn, a);

  dst = _mm_and_ps(a, FOG_SSE_GET_CONST_PS(m128f_num_mask));
  dst = _mm_max_ps(dst, FOG_SSE_GET_CONST_PS(m128f_epsilon));
  dst = _mm_or_ps(dst, sgn);
}

// ============================================================================
// [Fog::Face - SSE - MoveMask]
// ============================================================================

//! @brief Create a 4-bit mask from the most significant bits of the four SP-FP values.
static FOG_INLINE void m128fMoveMask(int& dst, const m128f& a)
{
  dst = _mm_movemask_ps(a);
}

//! @}

} // Face namespace
} // Fog namespace

// [Guard]
#endif // _FOG_CORE_FACE_FACESSE_H
