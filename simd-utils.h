#if !defined(SIMD_UTILS_H__)
#define SIMD_UTILS_H__ 1

#include <inttypes.h>
#include <x86intrin.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>

// 64 and 128 bit integers.
#include "simd-utils/simd-int.h"

#if defined(__MMX__)

// 64 bit vectors
#include "simd-utils/simd-64.h"

#if defined(__SSE2__)

// 128 bit vectors
#include "simd-utils/simd-128.h"

#if defined(__AVX__)

// 256 bit vector basics
#include "simd-utils/simd-256.h"

#if defined(__AVX2__)

// Utilities that require AVX2 are defined in simd-256.h.

// Skylake-X has all these
#if defined(__AVX512F__) && defined(__AVX512VL__) && defined(__AVX512DQ__) && defined(__AVX512BW__)

// 512 bit vectors
#include "simd-utils/simd-512.h"

#endif  // AVX512
#endif  // AVX2
#endif  // AVX
#endif  // SSE2
#endif  // MMX

#include "simd-utils/intrlv.h"

#endif  // SIMD_UTILS_H__
