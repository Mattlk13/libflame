/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/
/*
 *     Modifications Copyright (c) 2021-2023 Advanced Micro Devices, Inc.  All rights reserved.
 */
#include "FLAME.h"
#if FLA_ENABLE_AOCL_BLAS
#include "blis.h"
#endif

#ifndef FLA_LAPACK2FLAME_RETURN_DEFS_H
#define FLA_LAPACK2FLAME_RETURN_DEFS_H

// --- LAPACK return values ----

#define LAPACK_SUCCESS 512
#define LAPACK_FAILURE 312
#define LAPACK_QUICK_RETURN 212
#define LAPACK_QUERY_RETURN 112

#define LAPACK_RETURN_CHECK(r_check)                                      \
    {                                                                     \
        int r_val = r_check;                                              \
        switch(r_val)                                                     \
        {                                                                 \
            case LAPACK_FAILURE:                                          \
                return FLA_FAILURE;                                       \
            case LAPACK_QUERY_RETURN:;                                    \
            case LAPACK_QUICK_RETURN:                                     \
                return 0;                                                 \
            case LAPACK_SUCCESS:;                                         \
            default:;                                                     \
                if(r_val > 0)                                             \
                {                                                         \
                    ;                                                     \
                }                                                         \
                else                                                      \
                {                                                         \
                    FLA_Check_error_code(FLA_LAPAC2FLAME_INVALID_RETURN); \
                }                                                         \
        }                                                                 \
    }

#define LAPACK_RETURN_CHECK_VAR1(r_check, fla_error) \
    {                                                \
        int r_val = r_check;                         \
        switch(r_val)                                \
        {                                            \
            case LAPACK_FAILURE:                     \
                fla_error = FLA_FAILURE;             \
                break;                               \
            case LAPACK_QUERY_RETURN:;               \
            case LAPACK_QUICK_RETURN:                \
                fla_error = 0;                       \
                break;                               \
            case LAPACK_SUCCESS:;                    \
            default:;                                \
                if(r_val > 0)                        \
                {                                    \
                    ;                                \
                }                                    \
                else                                 \
                    fla_error = FLA_FAILURE;         \
        }                                            \
    }

#ifndef FLA_ENABLE_AOCL_BLAS
extern int lsame_(char *, char *, integer a, integer b);
extern void xerbla_(const char *srname, const integer *info, ftnlen srname_len);
#endif
extern integer ilaenv_(integer *, char *, char *, integer *, integer *, integer *, integer *);

#endif
