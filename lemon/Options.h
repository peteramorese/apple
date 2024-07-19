#pragma once

/* Enable bounds checking */
#define LMN_ENABLE_BOUNDS_CHECK

/* Enable inline */
#define LMN_ENABLE_INL

/* Enable asserts */
#define LMN_ASSERTS

/* Enable debug tools */
#define LMN_DEBUG_TOOLS

/* Enable logging in color */
#define LMN_LOG_COLOR

/* Floating point difference tolerance */
#define LMN_FLOAT_DIFF_TOL 1.0e-12


#ifdef LMN_ENABLE_INL
    #define LMN_INL inline
#elif
    #define LMN_INL
#endif