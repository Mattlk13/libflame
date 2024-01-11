/* slatrs3.f -- translated by f2c (version 20190311). You must link the resulting object file with libf2c: on Microsoft Windows system, link with libf2c.lib;
 on Linux or Unix systems, link with .../path/to/libf2c.a -lm or, if you install libf2c.a in a standard place, with -lf2c -lm -- in that order, at the end of the command line, as in cc *.o -lf2c -lm Source for libf2c is in /netlib/f2c/libf2c.zip, e.g., http://www.netlib.org/f2c/libf2c.zip */
#include "FLA_f2c.h" /* Table of constant values */
static integer c__1 = 1;
static integer c_n1 = -1;
static real c_b35 = -1.f;
static real c_b36 = 1.f;
/* > \brief \b SLATRS3 solves a triangular system of equations with the scale factors set to prevent overflow. */
/* Definition: */
/* =========== */
/* SUBROUTINE SLATRS3( UPLO, TRANS, DIAG, NORMIN, N, NRHS, A, LDA, */
/* X, LDX, SCALE, CNORM, WORK, LWORK, INFO ) */
/* .. Scalar Arguments .. */
/* CHARACTER DIAG, NORMIN, TRANS, UPLO */
/* INTEGER INFO, LDA, LWORK, LDX, N, NRHS */
/* .. */
/* .. Array Arguments .. */
/* REAL A( LDA, * ), CNORM( * ), SCALE( * ), */
/* WORK( * ), X( LDX, * ) */
/* .. */
/* > \par Purpose: */
/* ============= */
/* > */
/* > \verbatim */
/* > */
/* > SLATRS3 solves one of the triangular systems */
/* > */
/* > A * X = B * diag(scale) or A**T * X = B * diag(scale) */
/* > */
/* > with scaling to prevent overflow. Here A is an upper or lower */
/* > triangular matrix, A**T denotes the transpose of A. X and B are */
/* > n by nrhs matrices and scale is an nrhs element vector of scaling */
/* > factors. A scaling factor scale(j) is usually less than or equal */
/* > to 1, chosen such that X(:,j) is less than the overflow threshold. */
/* > If the matrix A is singular (A(j,j) = 0 for some j), then */
/* > a non-trivial solution to A*X = 0 is returned. If the system is */
/* > so badly scaled that the solution cannot be represented as */
/* > (1/scale(k))*X(:,k), then x(:,k) = 0 and scale(k) is returned. */
/* > */
/* > This is a BLAS-3 version of LATRS for solving several right */
/* > hand sides simultaneously. */
/* > */
/* > \endverbatim */
/* Arguments: */
/* ========== */
/* > \param[in] UPLO */
/* > \verbatim */
/* > UPLO is CHARACTER*1 */
/* > Specifies whether the matrix A is upper or lower triangular. */
/* > = 'U': Upper triangular */
/* > = 'L': Lower triangular */
/* > \endverbatim */
/* > */
/* > \param[in] TRANS */
/* > \verbatim */
/* > TRANS is CHARACTER*1 */
/* > Specifies the operation applied to A. */
/* > = 'N': Solve A * x = s*b (No transpose) */
/* > = 'T': Solve A**T* x = s*b (Transpose) */
/* > = 'C': Solve A**T* x = s*b (Conjugate transpose = Transpose) */
/* > \endverbatim */
/* > */
/* > \param[in] DIAG */
/* > \verbatim */
/* > DIAG is CHARACTER*1 */
/* > Specifies whether or not the matrix A is unit triangular. */
/* > = 'N': Non-unit triangular */
/* > = 'U': Unit triangular */
/* > \endverbatim */
/* > */
/* > \param[in] NORMIN */
/* > \verbatim */
/* > NORMIN is CHARACTER*1 */
/* > Specifies whether CNORM has been set or not. */
/* > = 'Y': CNORM contains the column norms on entry */
/* > = 'N': CNORM is not set on entry. On exit, the norms will */
/* > be computed and stored in CNORM. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* > N is INTEGER */
/* > The order of the matrix A. N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] NRHS */
/* > \verbatim */
/* > NRHS is INTEGER */
/* > The number of columns of X. NRHS >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] A */
/* > \verbatim */
/* > A is REAL array, dimension (LDA,N) */
/* > The triangular matrix A. If UPLO = 'U', the leading n by n */
/* > upper triangular part of the array A contains the upper */
/* > triangular matrix, and the strictly lower triangular part of */
/* > A is not referenced. If UPLO = 'L', the leading n by n lower */
/* > triangular part of the array A contains the lower triangular */
/* > matrix, and the strictly upper triangular part of A is not */
/* > referenced. If DIAG = 'U', the diagonal elements of A are */
/* > also not referenced and are assumed to be 1. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* > LDA is INTEGER */
/* > The leading dimension of the array A. LDA >= fla_max (1,N). */
/* > \endverbatim */
/* > */
/* > \param[in,out] X */
/* > \verbatim */
/* > X is REAL array, dimension (LDX,NRHS) */
/* > On entry, the right hand side B of the triangular system. */
/* > On exit, X is overwritten by the solution matrix X. */
/* > \endverbatim */
/* > */
/* > \param[in] LDX */
/* > \verbatim */
/* > LDX is INTEGER */
/* > The leading dimension of the array X. LDX >= fla_max (1,N). */
/* > \endverbatim */
/* > */
/* > \param[out] SCALE */
/* > \verbatim */
/* > SCALE is REAL array, dimension (NRHS) */
/* > The scaling factor s(k) is for the triangular system */
/* > A * x(:,k) = s(k)*b(:,k) or A**T* x(:,k) = s(k)*b(:,k). */
/* > If SCALE = 0, the matrix A is singular or badly scaled. */
/* > If A(j,j) = 0 is encountered, a non-trivial vector x(:,k) */
/* > that is an exact or approximate solution to A*x(:,k) = 0 */
/* > is returned. If the system so badly scaled that solution */
/* > cannot be presented as x(:,k) * 1/s(k), then x(:,k) = 0 */
/* > is returned. */
/* > \endverbatim */
/* > */
/* > \param[in,out] CNORM */
/* > \verbatim */
/* > CNORM is REAL array, dimension (N) */
/* > */
/* > If NORMIN = 'Y', CNORM is an input argument and CNORM(j) */
/* > contains the norm of the off-diagonal part of the j-th column */
/* > of A. If TRANS = 'N', CNORM(j) must be greater than or equal */
/* > to the infinity-norm, and if TRANS = 'T' or 'C', CNORM(j) */
/* > must be greater than or equal to the 1-norm. */
/* > */
/* > If NORMIN = 'N', CNORM is an output argument and CNORM(j) */
/* > returns the 1-norm of the offdiagonal part of the j-th column */
/* > of A. */
/* > \endverbatim */
/* > */
/* > \param[out] WORK */
/* > \verbatim */
/* > WORK is REAL array, dimension (LWORK). */
/* > On exit, if INFO = 0, WORK(1) returns the optimal size of */
/* > WORK. */
/* > \endverbatim */
/* > */
/* > \param[in] LWORK */
/* > LWORK is INTEGER */
/* > LWORK >= fla_max(1, 2*NBA * fla_max(NBA, fla_min(NRHS, 32)), where */
/* > NBA = (N + NB - 1)/NB and NB is the optimal block size. */
/* > */
/* > If LWORK = -1, then a workspace query is assumed;
the routine */
/* > only calculates the optimal dimensions of the WORK array, returns */
/* > this value as the first entry of the WORK array, and no error */
/* > message related to LWORK is issued by XERBLA. */
/* > */
/* > \param[out] INFO */
/* > \verbatim */
/* > INFO is INTEGER */
/* > = 0: successful exit */
/* > < 0: if INFO = -k, the k-th argument had an illegal value */
/* > \endverbatim */
/* Authors: */
/* ======== */
/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */
/* > \ingroup doubleOTHERauxiliary */
/* > \par Further Details: */
/* ===================== */
/* \verbatim */
/* The algorithm follows the structure of a block triangular solve. */
/* The diagonal block is solved with a call to the robust the triangular */
/* solver LATRS for every right-hand side RHS = 1, ..., NRHS */
/* op(A( J, J )) * x( J, RHS ) = SCALOC * b( J, RHS ), */
/* where op( A ) = A or op( A ) = A**T. */
/* The linear block updates operate on block columns of X, */
/* B( I, K ) - op(A( I, J )) * X( J, K ) */
/* and use GEMM. To avoid overflow in the linear block update, the worst case */
/* growth is estimated. For every RHS, a scale factor s <= 1.0 is computed */
/* such that */
/* || s * B( I, RHS )||_oo */
/* + || op(A( I, J )) ||_oo * || s * X( J, RHS ) ||_oo <= Overflow threshold */
/* Once all columns of a block column have been rescaled (BLAS-1), the linear */
/* update is executed with GEMM without overflow. */
/* To limit rescaling, local scale factors track the scaling of column segments. */
/* There is one local scale factor s( I, RHS ) per block row I = 1, ..., NBA */
/* per right-hand side column RHS = 1, ..., NRHS. The global scale factor */
/* SCALE( RHS ) is chosen as the smallest local scale factor s( I, RHS ) */
/* I = 1, ..., NBA. */
/* A triangular solve op(A( J, J )) * x( J, RHS ) = SCALOC * b( J, RHS ) */
/* updates the local scale factor s( J, RHS ) := s( J, RHS ) * SCALOC. The */
/* linear update of potentially inconsistently scaled vector segments */
/* s( I, RHS ) * b( I, RHS ) - op(A( I, J )) * ( s( J, RHS )* x( J, RHS ) ) */
/* computes a consistent scaling SCAMIN = fla_min( s(I, RHS ), s(J, RHS) ) and, */
/* if necessary, rescales the blocks prior to calling GEMM. */
/* \endverbatim */
/* ===================================================================== */
/* References: */
/* C. C. Kjelgaard Mikkelsen, A. B. Schwarz and L. Karlsson (2019). */
/* Parallel robust solution of triangular linear systems. Concurrency */
/* and Computation: Practice and Experience, 31(19), e5064. */
/* Contributor: */
/* Angelika Schwarz, Umea University, Sweden. */
/* ===================================================================== */
/* Subroutine */
void slatrs3_(char *uplo, char *trans, char *diag, char * normin, integer *n, integer *nrhs, real *a, integer *lda, real *x, integer *ldx, real *scale, real *cnorm, real *work, integer *lwork, integer *info)
{
    AOCL_DTL_TRACE_LOG_INIT
    AOCL_DTL_SNPRINTF("slatrs3 inputs: uplo %c, trans %c, diag %c, normin %c, n %" FLA_IS ", nrhs %" FLA_IS ", lda %" FLA_IS ", ldx %" FLA_IS "",*uplo, *trans, *diag, *normin, *n, *nrhs, *lda, *ldx);
    /* System generated locals */
    integer a_dim1, a_offset, x_dim1, x_offset, i__1, i__2, i__3, i__4, i__5, i__6, i__7, i__8;
    real r__1, r__2;
    /* Local variables */
    integer i__, j, k;
    real w[64];
    integer i1, i2, j1, j2, k1, k2, nb, ii, kk, nba, lds, nbx, rhs, iinc, jinc;
    real scal, anrm, bnrm;
    integer awrk;
    real tmax, xnrm[32];
    extern logical lsame_(char *, char *);
    real rscal;
    extern /* Subroutine */
    void sscal_(integer *, real *, real *, integer *), sgemm_(char *, char *, integer *, integer *, integer *, real *, real *, integer *, real *, integer *, real *, real *, integer *);
    integer lanrm, ilast, jlast;
    logical upper;
    integer lscale;
    real scaloc;
    extern real slamch_(char *), slange_(char *, integer *, integer *, real *, integer *, real *);
    real scamin;
    extern /* Subroutine */
    int xerbla_(const char *srname, const integer *info, ftnlen srname_len);
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, integer *, integer *);
    real bignum;
    extern real slarmm_(real *, real *, real *);
    integer ifirst;
    logical notran;
    integer jfirst;
    extern /* Subroutine */
    void slatrs_(char *, char *, char *, char *, integer *, real *, integer *, real *, real *, real *, integer *);
    real smlnum;
    logical nounit, lquery;
    /* .. Scalar Arguments .. */
    /* .. */
    /* .. Array Arguments .. */
    /* .. */
    /* ===================================================================== */
    /* .. Parameters .. */
    /* .. */
    /* .. Local Arrays .. */
    /* .. */
    /* .. Local Scalars .. */
    /* .. */
    /* .. External Functions .. */
    /* .. */
    /* .. External Subroutines .. */
    /* .. */
    /* .. Intrinsic Functions .. */
    /* .. */
    /* .. Executable Statements .. */
    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --scale;
    --cnorm;
    --work;
    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U");
    notran = lsame_(trans, "N");
    nounit = lsame_(diag, "N");
    lquery = *lwork == -1;
    /* Partition A and X into blocks. */
    /* Computing fla_max */
    i__1 = 8;
    i__2 = ilaenv_(&c__1, "SLATRS", "", n, n, &c_n1, &c_n1); // , expr subst
    nb = fla_max(i__1,i__2);
    nb = fla_min(64,nb);
    /* Computing fla_max */
    i__1 = 1;
    i__2 = (*n + nb - 1) / nb; // , expr subst
    nba = fla_max(i__1,i__2);
    /* Computing fla_max */
    i__1 = 1;
    i__2 = (*nrhs + 31) / 32; // , expr subst
    nbx = fla_max(i__1,i__2);
    /* Compute the workspace */
    /* The workspace comprises two parts. */
    /* The first part stores the local scale factors. Each simultaneously */
    /* computed right-hand side requires one local scale factor per block */
    /* row. WORK( I + KK * LDS ) is the scale factor of the vector */
    /* segment associated with the I-th block row and the KK-th vector */
    /* in the block column. */
    /* Computing fla_max */
    i__1 = nba;
    i__2 = fla_min(*nrhs,32); // , expr subst
    lscale = nba * fla_max(i__1,i__2);
    lds = nba;
    /* The second part stores upper bounds of the triangular A. There are */
    /* a total of NBA x NBA blocks, of which only the upper triangular */
    /* part or the lower triangular part is referenced. The upper bound of */
    /* the block A( I, J ) is stored as WORK( AWRK + I + J * NBA ). */
    lanrm = nba * nba;
    awrk = lscale;
    work[1] = (real) (lscale + lanrm);
    /* Test the input parameters. */
    if (! upper && ! lsame_(uplo, "L"))
    {
        *info = -1;
    }
    else if (! notran && ! lsame_(trans, "T") && ! lsame_(trans, "C"))
    {
        *info = -2;
    }
    else if (! nounit && ! lsame_(diag, "U"))
    {
        *info = -3;
    }
    else if (! lsame_(normin, "Y") && ! lsame_(normin, "N"))
    {
        *info = -4;
    }
    else if (*n < 0)
    {
        *info = -5;
    }
    else if (*nrhs < 0)
    {
        *info = -6;
    }
    else if (*lda < fla_max(1,*n))
    {
        *info = -8;
    }
    else if (*ldx < fla_max(1,*n))
    {
        *info = -10;
    }
    else if (! lquery && (real) (*lwork) < work[1])
    {
        *info = -14;
    }
    if (*info != 0)
    {
        i__1 = -(*info);
        xerbla_("SLATRS3", &i__1, (ftnlen)7);
    AOCL_DTL_TRACE_LOG_EXIT
        return;
    }
    else if (lquery)
    {
    AOCL_DTL_TRACE_LOG_EXIT
        return;
    }
    /* Initialize scaling factors */
    i__1 = *nrhs;
    for (kk = 1;
            kk <= i__1;
            ++kk)
    {
        scale[kk] = 1.f;
    }
    /* Quick return if possible */
    if (fla_min(*n,*nrhs) == 0)
    {
    AOCL_DTL_TRACE_LOG_EXIT
        return;
    }
    /* Determine machine dependent constant to control overflow. */
    bignum = slamch_("Overflow");
    smlnum = slamch_("Safe Minimum");
    /* Use unblocked code for small problems */
    if (*nrhs < 2)
    {
        slatrs_(uplo, trans, diag, normin, n, &a[a_offset], lda, &x[x_dim1 + 1], &scale[1], &cnorm[1], info);
        i__1 = *nrhs;
        for (k = 2;
                k <= i__1;
                ++k)
        {
            slatrs_(uplo, trans, diag, "Y", n, &a[a_offset], lda, &x[k * x_dim1 + 1], &scale[k], &cnorm[1], info);
        }
    AOCL_DTL_TRACE_LOG_EXIT
        return;
    }
    /* Compute norms of blocks of A excluding diagonal blocks and find */
    /* the block with the largest norm TMAX. */
    tmax = 0.f;
    i__1 = nba;
    for (j = 1;
            j <= i__1;
            ++j)
    {
        j1 = (j - 1) * nb + 1;
        /* Computing fla_min */
        i__2 = j * nb;
        j2 = fla_min(i__2,*n) + 1;
        if (upper)
        {
            ifirst = 1;
            ilast = j - 1;
        }
        else
        {
            ifirst = j + 1;
            ilast = nba;
        }
        i__2 = ilast;
        for (i__ = ifirst;
                i__ <= i__2;
                ++i__)
        {
            i1 = (i__ - 1) * nb + 1;
            /* Computing fla_min */
            i__3 = i__ * nb;
            i2 = fla_min(i__3,*n) + 1;
            /* Compute upper bound of A( I1:I2-1, J1:J2-1 ). */
            if (notran)
            {
                i__3 = i2 - i1;
                i__4 = j2 - j1;
                anrm = slange_("I", &i__3, &i__4, &a[i1 + j1 * a_dim1], lda, w);
                work[awrk + i__ + (j - 1) * nba] = anrm;
            }
            else
            {
                i__3 = i2 - i1;
                i__4 = j2 - j1;
                anrm = slange_("1", &i__3, &i__4, &a[i1 + j1 * a_dim1], lda, w);
                work[awrk + j + (i__ - 1) * nba] = anrm;
            }
            tmax = fla_max(tmax,anrm);
        }
    }
    if (! (tmax <= slamch_("Overflow")))
    {
        /* Some matrix entries have huge absolute value. At least one upper */
        /* bound norm( A(I1:I2-1, J1:J2-1), 'I') is not a valid floating-point */
        /* number, either due to overflow in LANGE or due to Inf in A. */
        /* Fall back to LATRS. Set normin = 'N' for every right-hand side to */
        /* force computation of TSCAL in LATRS to avoid the likely overflow */
        /* in the computation of the column norms CNORM. */
        i__1 = *nrhs;
        for (k = 1;
                k <= i__1;
                ++k)
        {
            slatrs_(uplo, trans, diag, "N", n, &a[a_offset], lda, &x[k * x_dim1 + 1], &scale[k], &cnorm[1], info);
        }
    AOCL_DTL_TRACE_LOG_EXIT
        return;
    }
    /* Every right-hand side requires workspace to store NBA local scale */
    /* factors. To save workspace, X is computed successively in block columns */
    /* of width NBRHS, requiring a total of NBA x NBRHS space. If sufficient */
    /* workspace is available, larger values of NBRHS or NBRHS = NRHS are viable. */
    i__1 = nbx;
    for (k = 1;
            k <= i__1;
            ++k)
    {
        /* Loop over block columns (index = K) of X and, for column-wise scalings, */
        /* over individual columns (index = KK). */
        /* K1: column index of the first column in X( J, K ) */
        /* K2: column index of the first column in X( J, K+1 ) */
        /* so the K2 - K1 is the column count of the block X( J, K ) */
        k1 = (k - 1 << 5) + 1;
        /* Computing fla_min */
        i__2 = k << 5;
        k2 = fla_min(i__2,*nrhs) + 1;
        /* Initialize local scaling factors of current block column X( J, K ) */
        i__2 = k2 - k1;
        for (kk = 1;
                kk <= i__2;
                ++kk)
        {
            i__3 = nba;
            for (i__ = 1;
                    i__ <= i__3;
                    ++i__)
            {
                work[i__ + kk * lds] = 1.f;
            }
        }
        if (notran)
        {
            /* Solve A * X(:, K1:K2-1) = B * diag(scale(K1:K2-1)) */
            if (upper)
            {
                jfirst = nba;
                jlast = 1;
                jinc = -1;
            }
            else
            {
                jfirst = 1;
                jlast = nba;
                jinc = 1;
            }
        }
        else
        {
            /* Solve A**T * X(:, K1:K2-1) = B * diag(scale(K1:K2-1)) */
            if (upper)
            {
                jfirst = 1;
                jlast = nba;
                jinc = 1;
            }
            else
            {
                jfirst = nba;
                jlast = 1;
                jinc = -1;
            }
        }
        i__2 = jlast;
        i__3 = jinc;
        for (j = jfirst;
                i__3 < 0 ? j >= i__2 : j <= i__2;
                j += i__3)
        {
            /* J1: row index of the first row in A( J, J ) */
            /* J2: row index of the first row in A( J+1, J+1 ) */
            /* so that J2 - J1 is the row count of the block A( J, J ) */
            j1 = (j - 1) * nb + 1;
            /* Computing fla_min */
            i__4 = j * nb;
            j2 = fla_min(i__4,*n) + 1;
            /* Solve op(A( J, J )) * X( J, RHS ) = SCALOC * B( J, RHS ) */
            /* for all right-hand sides in the current block column, */
            /* one RHS at a time. */
            i__4 = k2 - k1;
            for (kk = 1;
                    kk <= i__4;
                    ++kk)
            {
                rhs = k1 + kk - 1;
                if (kk == 1)
                {
                    i__5 = j2 - j1;
                    slatrs_(uplo, trans, diag, "N", &i__5, &a[j1 + j1 * a_dim1], lda, &x[j1 + rhs * x_dim1], &scaloc, & cnorm[1], info);
                }
                else
                {
                    i__5 = j2 - j1;
                    slatrs_(uplo, trans, diag, "Y", &i__5, &a[j1 + j1 * a_dim1], lda, &x[j1 + rhs * x_dim1], &scaloc, & cnorm[1], info);
                }
                /* Find largest absolute value entry in the vector segment */
                /* X( J1:J2-1, RHS ) as an upper bound for the worst case */
                /* growth in the linear updates. */
                i__5 = j2 - j1;
                xnrm[kk - 1] = slange_("I", &i__5, &c__1, &x[j1 + rhs * x_dim1], ldx, w);
                if (scaloc == 0.f)
                {
                    /* LATRS found that A is singular through A(j,j) = 0. */
                    /* Reset the computation x(1:n) = 0, x(j) = 1, SCALE = 0 */
                    /* and compute A*x = 0 (or A**T*x = 0). Note that */
                    /* X(J1:J2-1, KK) is set by LATRS. */
                    scale[rhs] = 0.f;
                    i__5 = j1 - 1;
                    for (ii = 1;
                            ii <= i__5;
                            ++ii)
                    {
                        x[ii + kk * x_dim1] = 0.f;
                    }
                    i__5 = *n;
                    for (ii = j2;
                            ii <= i__5;
                            ++ii)
                    {
                        x[ii + kk * x_dim1] = 0.f;
                    }
                    /* Discard the local scale factors. */
                    i__5 = nba;
                    for (ii = 1;
                            ii <= i__5;
                            ++ii)
                    {
                        work[ii + kk * lds] = 1.f;
                    }
                    scaloc = 1.f;
                }
                else if (scaloc * work[j + kk * lds] == 0.f)
                {
                    /* LATRS computed a valid scale factor, but combined with */
                    /* the current scaling the solution does not have a */
                    /* scale factor > 0. */
                    /* Set WORK( J+KK*LDS ) to smallest valid scale */
                    /* factor and increase SCALOC accordingly. */
                    scal = work[j + kk * lds] / smlnum;
                    scaloc *= scal;
                    work[j + kk * lds] = smlnum;
                    /* If LATRS overestimated the growth, x may be */
                    /* rescaled to preserve a valid combined scale */
                    /* factor WORK( J, KK ) > 0. */
                    rscal = 1.f / scaloc;
                    if (xnrm[kk - 1] * rscal <= bignum)
                    {
                        xnrm[kk - 1] *= rscal;
                        i__5 = j2 - j1;
                        sscal_(&i__5, &rscal, &x[j1 + rhs * x_dim1], &c__1);
                        scaloc = 1.f;
                    }
                    else
                    {
                        /* The system op(A) * x = b is badly scaled and its */
                        /* solution cannot be represented as (1/scale) * x. */
                        /* Set x to zero. This approach deviates from LATRS */
                        /* where a completely meaningless non-zero vector */
                        /* is returned that is not a solution to op(A) * x = b. */
                        scale[rhs] = 0.f;
                        i__5 = *n;
                        for (ii = 1;
                                ii <= i__5;
                                ++ii)
                        {
                            x[ii + kk * x_dim1] = 0.f;
                        }
                        /* Discard the local scale factors. */
                        i__5 = nba;
                        for (ii = 1;
                                ii <= i__5;
                                ++ii)
                        {
                            work[ii + kk * lds] = 1.f;
                        }
                        scaloc = 1.f;
                    }
                }
                scaloc *= work[j + kk * lds];
                work[j + kk * lds] = scaloc;
            }
            /* Linear block updates */
            if (notran)
            {
                if (upper)
                {
                    ifirst = j - 1;
                    ilast = 1;
                    iinc = -1;
                }
                else
                {
                    ifirst = j + 1;
                    ilast = nba;
                    iinc = 1;
                }
            }
            else
            {
                if (upper)
                {
                    ifirst = j + 1;
                    ilast = nba;
                    iinc = 1;
                }
                else
                {
                    ifirst = j - 1;
                    ilast = 1;
                    iinc = -1;
                }
            }
            i__4 = ilast;
            i__5 = iinc;
            for (i__ = ifirst;
                    i__5 < 0 ? i__ >= i__4 : i__ <= i__4;
                    i__ += i__5)
            {
                /* I1: row index of the first column in X( I, K ) */
                /* I2: row index of the first column in X( I+1, K ) */
                /* so the I2 - I1 is the row count of the block X( I, K ) */
                i1 = (i__ - 1) * nb + 1;
                /* Computing fla_min */
                i__6 = i__ * nb;
                i2 = fla_min(i__6,*n) + 1;
                /* Prepare the linear update to be executed with GEMM. */
                /* For each column, compute a consistent scaling, a */
                /* scaling factor to survive the linear update, and */
                /* rescale the column segments, if necesssary. Then */
                /* the linear update is safely executed. */
                i__6 = k2 - k1;
                for (kk = 1;
                        kk <= i__6;
                        ++kk)
                {
                    rhs = k1 + kk - 1;
                    /* Compute consistent scaling */
                    /* Computing fla_min */
                    r__1 = work[i__ + kk * lds];
                    r__2 = work[j + kk * lds]; // , expr subst
                    scamin = fla_min(r__1,r__2);
                    /* Compute scaling factor to survive the linear update */
                    /* simulating consistent scaling. */
                    i__7 = i2 - i1;
                    bnrm = slange_("I", &i__7, &c__1, &x[i1 + rhs * x_dim1], ldx, w);
                    bnrm *= scamin / work[i__ + kk * lds];
                    xnrm[kk - 1] *= scamin / work[j + kk * lds];
                    anrm = work[awrk + i__ + (j - 1) * nba];
                    scaloc = slarmm_(&anrm, &xnrm[kk - 1], &bnrm);
                    /* Simultaneously apply the robust update factor and the */
                    /* consistency scaling factor to B( I, KK ) and B( J, KK ). */
                    scal = scamin / work[i__ + kk * lds] * scaloc;
                    if (scal != 1.f)
                    {
                        i__7 = i2 - i1;
                        sscal_(&i__7, &scal, &x[i1 + rhs * x_dim1], &c__1);
                        work[i__ + kk * lds] = scamin * scaloc;
                    }
                    scal = scamin / work[j + kk * lds] * scaloc;
                    if (scal != 1.f)
                    {
                        i__7 = j2 - j1;
                        sscal_(&i__7, &scal, &x[j1 + rhs * x_dim1], &c__1);
                        work[j + kk * lds] = scamin * scaloc;
                    }
                }
                if (notran)
                {
                    /* B( I, K ) := B( I, K ) - A( I, J ) * X( J, K ) */
                    i__6 = i2 - i1;
                    i__7 = k2 - k1;
                    i__8 = j2 - j1;
                    sgemm_("N", "N", &i__6, &i__7, &i__8, &c_b35, &a[i1 + j1 * a_dim1], lda, &x[j1 + k1 * x_dim1], ldx, &c_b36, &x[i1 + k1 * x_dim1], ldx);
                }
                else
                {
                    /* B( I, K ) := B( I, K ) - A( I, J )**T * X( J, K ) */
                    i__6 = i2 - i1;
                    i__7 = k2 - k1;
                    i__8 = j2 - j1;
                    sgemm_("T", "N", &i__6, &i__7, &i__8, &c_b35, &a[j1 + i1 * a_dim1], lda, &x[j1 + k1 * x_dim1], ldx, &c_b36, &x[i1 + k1 * x_dim1], ldx);
                }
            }
        }
        /* Reduce local scaling factors */
        i__3 = k2 - k1;
        for (kk = 1;
                kk <= i__3;
                ++kk)
        {
            rhs = k1 + kk - 1;
            i__2 = nba;
            for (i__ = 1;
                    i__ <= i__2;
                    ++i__)
            {
                /* Computing fla_min */
                r__1 = scale[rhs];
                r__2 = work[i__ + kk * lds]; // , expr subst
                scale[rhs] = fla_min(r__1,r__2);
            }
        }
        /* Realize consistent scaling */
        i__3 = k2 - k1;
        for (kk = 1;
                kk <= i__3;
                ++kk)
        {
            rhs = k1 + kk - 1;
            if (scale[rhs] != 1.f && scale[rhs] != 0.f)
            {
                i__2 = nba;
                for (i__ = 1;
                        i__ <= i__2;
                        ++i__)
                {
                    i1 = (i__ - 1) * nb + 1;
                    /* Computing fla_min */
                    i__5 = i__ * nb;
                    i2 = fla_min(i__5,*n) + 1;
                    scal = scale[rhs] / work[i__ + kk * lds];
                    if (scal != 1.f)
                    {
                        i__5 = i2 - i1;
                        sscal_(&i__5, &scal, &x[i1 + rhs * x_dim1], &c__1);
                    }
                }
            }
        }
    }
    AOCL_DTL_TRACE_LOG_EXIT
    return;
    /* End of SLATRS3 */
}
/* slatrs3_ */
