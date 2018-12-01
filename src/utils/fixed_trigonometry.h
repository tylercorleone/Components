// @see https://www.logre.eu/wiki/Trigonométrie_en_virgule_fixe
#ifndef FIXED_TRIGONOMETRY_H
#define FIXED_TRIGONOMETRY_H

#define _TWO_PI 6.283185307179586476925286766559

/*
 * cos_fix.h: Fixed-point cos() and sin() functions, based on a sixth
 * degree polynomial approximation.
 *
 * argument is in units of 2*M_PI/2^16.
 * result is in units of 1/2^14 (range = [-2^14 : 2^14]).
 *
 * The cosine function uses an even-polynomial approximation of
 * cos(M_PI/2*x) for x in [0:1], and symmetries when x is outside [0:1].
 * Sine is defined as sin(x) = cos(3*M_PI/2+x).
 */

#include <stdint.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Sixth degree polynomial:
 *      cos(M_PI/2*x) ~ (1 - x^2)*(1 - x^2*(0.23352 - 0.019531*x^2))
 * for x in [0:1]. Max error = 9.53e-5
 */
int16_t cos_fix(uint16_t x);

/*
 * Fixed point sin().
 */
static inline int16_t sin_fix(uint16_t x)
{
    return cos_fix(0xc000 + x);
}

#ifdef __cplusplus
}
#endif

/*
 * cos_fix.c: Fixed-point cos() function.
 *
 * Compile for AVR:
 *      avr-gcc -c -mmcu=avr5 -Os -Wall -Wextra cos_fix.c
 *
 * Compile for AVR with no ASM code:
 *      avr-gcc -DNO_ASM -c -mmcu=avr5 -Os -Wall -Wextra cos_fix.c
 *
 * Compile test program:
 *      gcc -DRUN_TEST -O -Wall -Wextra cos_fix.c -o cos_fix
 *
 * Usage (test program):
 *      ./cos_fix > cos_fix.tsv
 */

#define FIXED(x, n) ((uint16_t)((float)(x) * ((uint32_t)1 << (n)) + .5))

#if !defined(NO_ASM)
# if defined(__AVR_HAVE_MUL__) && defined(__AVR_HAVE_MOVW__)
#  define USE_AVR_ASM
# endif
#endif

/*
 * Fixed point multiplication.
 *
 * Multiply two fixed point numbers in u16,16 (unsigned 0.16) format.
 * Returns result in the same format.
 * Rounds to nearest, ties rounded up.
 */
static inline uint16_t mul_fix_u16(uint16_t x, uint16_t y)
{
    uint16_t result;
#ifdef USE_AVR_ASM
    /* Optimized ASM version. */
    asm volatile(
	    "mul  %B1, %B2\n\t"
	    "movw %A0, r0\n\t"
	    "ldi  r19, 0x80\n\t"
	    "clr  r18\n\t"
	    "mul  %A1, %A2\n\t"
	    "add  r19, r1\n\t"
	    "adc  %A0, r18\n\t"
	    "adc  %B0, r18\n\t"
	    "mul  %B1, %A2\n\t"
	    "add  r19, r0\n\t"
	    "adc  %A0, r1\n\t"
	    "adc  %B0, r18\n\t"
	    "mul  %A1, %B2\n\t"
	    "add  r19, r0\n\t"
	    "adc  %A0, r1\n\t"
	    "adc  %B0, r18\n\t"
	    "clr  r1"
        : "=&r" (result)
        : "r" (x), "r" (y)
        : "r18", "r19"
    );
#else
    /* Generic C version. Compiles to inefficient 32 bit code. */
    result = ((uint32_t) x * y + 0x8000) >> 16;
#endif
    return result;
}

/*
 * Cheap and rough fixed point multiplication: multiply only the high
 * bytes of the operands, return 16 bit result.
 *
 * For some reason, the equivalent macro compiles to inefficient code.
 * This compiles to 3 instructions (mul a,b; movw res,r0; clr r1).
 */
static inline uint16_t mul_high_bytes(uint16_t x, uint16_t y)
{
    return (uint8_t)(x >> 8) * (uint8_t)(y >> 8);
}

/*
 * Fixed point cos() function: sixth degree polynomial approximation.
 *
 * argument is in units of 2*M_PI/2^16.
 * result is in units of 1/2^14 (range = [-2^14 : 2^14]).
 *
 * Uses the approximation
 *      cos(M_PI/2*x) ~ P(x^2), with
 *      P(u) = (1 - u) * (1 - u * (0.23352 - 0.019531 * u))
 * for x in [0 : 1]. Max error = 9.53e-5
 */
inline int16_t cos_fix(uint16_t x)
{
    uint16_t y, s;
    uint8_t i = (x >> 8) & 0xc0;  // quadrant information
    x = (x & 0x3fff) << 1;        // .15
    if (i & 0x40) x = FIXED(1, 15) - x;
    x = mul_fix_u16(x, x) << 1;   // .15
    y = FIXED(1, 15) - x;         // .15
    s = FIXED(0.23361, 16) - mul_high_bytes(FIXED(0.019531, 17), x); // .16
    s = FIXED(1, 15) - mul_fix_u16(x, s);  // .15
    s = mul_fix_u16(y, s);        // .14
    return (i == 0x40 || i == 0x80) ? -s : s;
}

inline float _sin(float x) {
	return static_cast<float>(sin_fix(x / _TWO_PI * UINT16_MAX)) / (0x1 << 14);
}

inline float _cos(float x) {
	return static_cast<float>(cos_fix(x / _TWO_PI * UINT16_MAX)) / (0x1 << 14);
}

#endif
