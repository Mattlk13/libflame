/* ../netlib/cgeequ.f -- translated by f2c (version 20100827). You must link the resulting object file with libf2c: on Microsoft Windows system, link with libf2c.lib;
 on Linux or Unix systems, link with .../path/to/libf2c.a -lm or, if you install libf2c.a in a standard place, with -lf2c -lm -- in that order, at the end of the command line, as in cc *.o -lf2c -lm Source for libf2c is in /netlib/f2c/libf2c.zip, e.g., http://www.netlib.org/f2c/libf2c.zip */
#include "FLA_f2c.h" /* > \brief \b CGEEQU */
/* =========== DOCUMENTATION =========== */
/* Online html documentation available at */
/* http://www.netlib.org/lapack/explore-html/ */
/* > \htmlonly */
/* > Download CGEEQU + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/cgeequ. f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/cgeequ. f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/cgeequ. f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */
/* Definition: */
/* =========== */
/* SUBROUTINE CGEEQU( M, N, A, LDA, R, C, ROWCND, COLCND, AMAX, */
/* INFO ) */
/* .. Scalar Arguments .. */
/* INTEGER INFO, LDA, M, N */
/* REAL AMAX, COLCND, ROWCND */
/* .. */
/* .. Array Arguments .. */
/* REAL C( * ), R( * ) */
/* COMPLEX A( LDA, * ) */
/* .. */
/* > \par Purpose: */
/* ============= */
/* > */
/* > \verbatim */
/* > */
/* > CGEEQU computes row and column scalings intended to equilibrate an */
/* > M-by-N matrix A and reduce its condition number. R returns the row */
/* > scale factors and C the column scale factors, chosen to try to make */
/* > the largest element in each row and column of the matrix B with */
/* > elements B(i,j)=R(i)*A(i,j)*C(j) have absolute value 1. */
/* > */
/* > R(i) and C(j) are restricted to be between SMLNUM = smallest safe */
/* > number and BIGNUM = largest safe number. Use of these scaling */
/* > factors is not guaranteed to reduce the condition number of A but */
/* > works well in practice. */
/* > \endverbatim */
/* Arguments: */
/* ========== */
/* > \param[in] M */
/* > \verbatim */
/* > M is INTEGER */
/* > The number of rows of the matrix A. M >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* > N is INTEGER */
/* > The number of columns of the matrix A. N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] A */
/* > \verbatim */
/* > A is COMPLEX array, dimension (LDA,N) */
/* > The M-by-N matrix whose equilibration factors are */
/* > to be computed. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* > LDA is INTEGER */
/* > The leading dimension of the array A. LDA >= max(1,M). */
/* > \endverbatim */
/* > */
/* > \param[out] R */
/* > \verbatim */
/* > R is REAL array, dimension (M) */
/* > If INFO = 0 or INFO > M, R contains the row scale factors */
/* > for A. */
/* > \endverbatim */
/* > */
/* > \param[out] C */
/* > \verbatim */
/* > C is REAL array, dimension (N) */
/* > If INFO = 0, C contains the column scale factors for A. */
/* > \endverbatim */
/* > */
/* > \param[out] ROWCND */
/* > \verbatim */
/* > ROWCND is REAL */
/* > If INFO = 0 or INFO > M, ROWCND contains the ratio of the */
/* > smallest R(i) to the largest R(i). If ROWCND >= 0.1 and */
/* > AMAX is neither too large nor too small, it is not worth */
/* > scaling by R. */
/* > \endverbatim */
/* > */
/* > \param[out] COLCND */
/* > \verbatim */
/* > COLCND is REAL */
/* > If INFO = 0, COLCND contains the ratio of the smallest */
/* > C(i) to the largest C(i). If COLCND >= 0.1, it is not */
/* > worth scaling by C. */
/* > \endverbatim */
/* > */
/* > \param[out] AMAX */
/* > \verbatim */
/* > AMAX is REAL */
/* > Absolute value of largest matrix element. If AMAX is very */
/* > close to overflow or very close to underflow, the matrix */
/* > should be scaled. */
/* > \endverbatim */
/* > */
/* > \param[out] INFO */
/* > \verbatim */
/* > INFO is INTEGER */
/* > = 0: successful exit */
/* > < 0: if INFO = -i, the i-th argument had an illegal value */
/* > > 0: if INFO = i, and i is */
/* > <= M: the i-th row of A is exactly zero */
/* > > M: the (i-M)-th column of A is exactly zero */
/* > \endverbatim */
/* Authors: */
/* ======== */
/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */
/* > \date November 2011 */
/* > \ingroup complexGEcomputational */
/* ===================================================================== */
/* Subroutine */
int cgeequ_(integer *m, integer *n, complex *a, integer *lda, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    real r__1, r__2, r__3, r__4;
    /* Builtin functions */
    double r_imag(complex *);
    /* Local variables */
    integer i__, j;
    real rcmin, rcmax;
    extern real slamch_(char *);
    extern /* Subroutine */
    int xerbla_(char *, integer *);
    real bignum, smlnum;
    /* -- LAPACK computational routine (version 3.4.0) -- */
    /* -- LAPACK is a software package provided by Univ. of Tennessee, -- */
    /* -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
    /* November 2011 */
    /* .. Scalar Arguments .. */
    /* .. */
    /* .. Array Arguments .. */
    /* .. */
    /* ===================================================================== */
    /* .. Parameters .. */
    /* .. */
    /* .. Local Scalars .. */
    /* .. */
    /* .. External Functions .. */
    /* .. */
    /* .. External Subroutines .. */
    /* .. */
    /* .. Intrinsic Functions .. */
    /* .. */
    /* .. Statement Functions .. */
    /* .. */
    /* .. Statement Function definitions .. */
    /* .. */
    /* .. Executable Statements .. */
    /* Test the input parameters. */
    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --r__;
    --c__;
    AOCL_DTL_TRACE_ENTRY(AOCL_DTL_LEVEL_TRACE_5);
    /* Function Body */
#if AOCL_DTL_LOG_ENABLE
    char buffer[256];
    sprintf(buffer, "cgeequ inputs: m %d, n %d, lda %d\n", *m, *n, *lda);
    AOCL_DTL_LOG(AOCL_DTL_LEVEL_TRACE_5, buffer);
#endif
    *info = 0;
    if (*m < 0)
    {
        *info = -1;
    }
    else if (*n < 0)
    {
        *info = -2;
    }
    else if (*lda < max(1,*m))
    {
        *info = -4;
    }
    if (*info != 0)
    {
        i__1 = -(*info);
        xerbla_("CGEEQU", &i__1);
        return 0;
    }
    /* Quick return if possible */
    if (*m == 0 || *n == 0)
    {
        *rowcnd = 1.f;
        *colcnd = 1.f;
        *amax = 0.f;
        return 0;
    }
    /* Get machine constants. */
    smlnum = slamch_("S");
    bignum = 1.f / smlnum;
    /* Compute row scale factors. */
    i__1 = *m;
    for (i__ = 1;
            i__ <= i__1;
            ++i__)
    {
        r__[i__] = 0.f;
        /* L10: */
    }
    /* Find the maximum element in each row. */
    i__1 = *n;
    for (j = 1;
            j <= i__1;
            ++j)
    {
        i__2 = *m;
        for (i__ = 1;
                i__ <= i__2;
                ++i__)
        {
            /* Computing MAX */
            i__3 = i__ + j * a_dim1;
            r__3 = r__[i__];
            r__4 = (r__1 = a[i__3].r, f2c_abs(r__1)) + (r__2 = r_imag(&a[i__ + j * a_dim1]), f2c_abs(r__2)); // , expr subst
            r__[i__] = max(r__3,r__4);
            /* L20: */
        }
        /* L30: */
    }
    /* Find the maximum and minimum scale factors. */
    rcmin = bignum;
    rcmax = 0.f;
    i__1 = *m;
    for (i__ = 1;
            i__ <= i__1;
            ++i__)
    {
        /* Computing MAX */
        r__1 = rcmax;
        r__2 = r__[i__]; // , expr subst
        rcmax = max(r__1,r__2);
        /* Computing MIN */
        r__1 = rcmin;
        r__2 = r__[i__]; // , expr subst
        rcmin = min(r__1,r__2);
        /* L40: */
    }
    *amax = rcmax;
    if (rcmin == 0.f)
    {
        /* Find the first zero scale factor and return an error code. */
        i__1 = *m;
        for (i__ = 1;
                i__ <= i__1;
                ++i__)
        {
            if (r__[i__] == 0.f)
            {
                *info = i__;
                return 0;
            }
            /* L50: */
        }
    }
    else
    {
        /* Invert the scale factors. */
        i__1 = *m;
        for (i__ = 1;
                i__ <= i__1;
                ++i__)
        {
            /* Computing MIN */
            /* Computing MAX */
            r__2 = r__[i__];
            r__1 = max(r__2,smlnum);
            r__[i__] = 1.f / min(r__1,bignum);
            /* L60: */
        }
        /* Compute ROWCND = min(R(I)) / max(R(I)) */
        *rowcnd = max(rcmin,smlnum) / min(rcmax,bignum);
    }
    /* Compute column scale factors */
    i__1 = *n;
    for (j = 1;
            j <= i__1;
            ++j)
    {
        c__[j] = 0.f;
        /* L70: */
    }
    /* Find the maximum element in each column, */
    /* assuming the row scaling computed above. */
    i__1 = *n;
    for (j = 1;
            j <= i__1;
            ++j)
    {
        i__2 = *m;
        for (i__ = 1;
                i__ <= i__2;
                ++i__)
        {
            /* Computing MAX */
            i__3 = i__ + j * a_dim1;
            r__3 = c__[j];
            r__4 = ((r__1 = a[i__3].r, f2c_abs(r__1)) + (r__2 = r_imag(&a[i__ + j * a_dim1]), f2c_abs(r__2))) * r__[i__]; // , expr subst
            c__[j] = max(r__3,r__4);
            /* L80: */
        }
        /* L90: */
    }
    /* Find the maximum and minimum scale factors. */
    rcmin = bignum;
    rcmax = 0.f;
    i__1 = *n;
    for (j = 1;
            j <= i__1;
            ++j)
    {
        /* Computing MIN */
        r__1 = rcmin;
        r__2 = c__[j]; // , expr subst
        rcmin = min(r__1,r__2);
        /* Computing MAX */
        r__1 = rcmax;
        r__2 = c__[j]; // , expr subst
        rcmax = max(r__1,r__2);
        /* L100: */
    }
    if (rcmin == 0.f)
    {
        /* Find the first zero scale factor and return an error code. */
        i__1 = *n;
        for (j = 1;
                j <= i__1;
                ++j)
        {
            if (c__[j] == 0.f)
            {
                *info = *m + j;
                return 0;
            }
            /* L110: */
        }
    }
    else
    {
        /* Invert the scale factors. */
        i__1 = *n;
        for (j = 1;
                j <= i__1;
                ++j)
        {
            /* Computing MIN */
            /* Computing MAX */
            r__2 = c__[j];
            r__1 = max(r__2,smlnum);
            c__[j] = 1.f / min(r__1,bignum);
            /* L120: */
        }
        /* Compute COLCND = min(C(J)) / max(C(J)) */
        *colcnd = max(rcmin,smlnum) / min(rcmax,bignum);
    }
    AOCL_DTL_TRACE_EXIT(AOCL_DTL_LEVEL_TRACE_5);
    return 0;
    /* End of CGEEQU */
}
/* cgeequ_ */
