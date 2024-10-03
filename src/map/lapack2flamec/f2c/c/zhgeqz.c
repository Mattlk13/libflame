/* zhgeqz.f -- translated by f2c (version 20160102). You must link the resulting object file with
 libf2c: on Microsoft Windows system, link with libf2c.lib; on Linux or Unix systems, link with
 .../path/to/libf2c.a -lm or, if you install libf2c.a in a standard place, with -lf2c -lm -- in that
 order, at the end of the command line, as in cc *.o -lf2c -lm Source for libf2c is in
 /netlib/f2c/libf2c.zip, e.g., http://www.netlib.org/f2c/libf2c.zip */

/*
*     Modifications Copyright (c) 2024 Advanced Micro Devices, Inc.  All rights reserved.
*/

#include "FLA_f2c.h" /* Table of constant values */
#ifdef FLA_OPENMP_MULTITHREADING
#include "omp.h"
#endif

#define QZ_MAX_SWEEPS 32

static doublecomplex c_b1 = {0., 0.};
static doublecomplex c_b2 = {1., 0.};
static integer c__1 = 1;
static integer c__2 = 2;

/* > \brief \b ZHGEQZ */
/* =========== DOCUMENTATION =========== */
/* Online html documentation available at */
/* http://www.netlib.org/lapack/explore-html/ */
/* > \htmlonly */
/* > Download ZHGEQZ + dependencies */
/* > <a
 * href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/zhgeqz.
 * f"> */
/* > [TGZ]</a> */
/* > <a
 * href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/zhgeqz.
 * f"> */
/* > [ZIP]</a> */
/* > <a
 * href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/zhgeqz.
 * f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */
/* Definition: */
/* =========== */
/* SUBROUTINE ZHGEQZ( JOB, COMPQ, COMPZ, N, ILO, IHI, H, LDH, T, LDT, */
/* ALPHA, BETA, Q, LDQ, Z, LDZ, WORK, LWORK, */
/* RWORK, INFO ) */
/* .. Scalar Arguments .. */
/* CHARACTER COMPQ, COMPZ, JOB */
/* INTEGER IHI, ILO, INFO, LDH, LDQ, LDT, LDZ, LWORK, N */
/* .. */
/* .. Array Arguments .. */
/* DOUBLE PRECISION RWORK( * ) */
/* COMPLEX*16 ALPHA( * ), BETA( * ), H( LDH, * ), */
/* $ Q( LDQ, * ), T( LDT, * ), WORK( * ), */
/* $ Z( LDZ, * ) */
/* .. */
/* > \par Purpose: */
/* ============= */
/* > */
/* > \verbatim */
/* > */
/* > ZHGEQZ computes the eigenvalues of a complex matrix pair (H,T), */
/* > where H is an upper Hessenberg matrix and T is upper triangular, */
/* > using the single-shift QZ method. */
/* > Matrix pairs of this type are produced by the reduction to */
/* > generalized upper Hessenberg form of a complex matrix pair (A,B): */
/* > */
/* > A = Q1*H*Z1**H, B = Q1*T*Z1**H, */
/* > */
/* > as computed by ZGGHRD. */
/* > */
/* > If JOB='S', then the Hessenberg-triangular pair (H,T) is */
/* > also reduced to generalized Schur form, */
/* > */
/* > H = Q*S*Z**H, T = Q*P*Z**H, */
/* > */
/* > where Q and Z are unitary matrices and S and P are upper triangular. */
/* > */
/* > Optionally, the unitary matrix Q from the generalized Schur */
/* > factorization may be postmultiplied into an input matrix Q1, and the */
/* > unitary matrix Z may be postmultiplied into an input matrix Z1. */
/* > If Q1 and Z1 are the unitary matrices from ZGGHRD that reduced */
/* > the matrix pair (A,B) to generalized Hessenberg form, then the output */
/* > matrices Q1*Q and Z1*Z are the unitary factors from the generalized */
/* > Schur factorization of (A,B): */
/* > */
/* > A = (Q1*Q)*S*(Z1*Z)**H, B = (Q1*Q)*P*(Z1*Z)**H. */
/* > */
/* > To avoid overflow, eigenvalues of the matrix pair (H,T) */
/* > (equivalently, of (A,B)) are computed as a pair of complex values */
/* > (alpha,beta). If beta is nonzero, lambda = alpha / beta is an */
/* > eigenvalue of the generalized nonsymmetric eigenvalue problem (GNEP) */
/* > A*x = lambda*B*x */
/* > and if alpha is nonzero, mu = beta / alpha is an eigenvalue of the */
/* > alternate form of the GNEP */
/* > mu*A*y = B*y. */
/* > The values of alpha and beta for the i-th eigenvalue can be read */
/* > directly from the generalized Schur form: alpha = S(i,i), */
/* > beta = P(i,i). */
/* > */
/* > Ref: C.B. Moler & G.W. Stewart, "An Algorithm for Generalized Matrix */
/* > Eigenvalue Problems", SIAM J. Numer. Anal., 10(1973), */
/* > pp. 241--256. */
/* > \endverbatim */
/* Arguments: */
/* ========== */
/* > \param[in] JOB */
/* > \verbatim */
/* > JOB is CHARACTER*1 */
/* > = 'E': Compute eigenvalues only;
 */
/* > = 'S': Computer eigenvalues and the Schur form. */
/* > \endverbatim */
/* > */
/* > \param[in] COMPQ */
/* > \verbatim */
/* > COMPQ is CHARACTER*1 */
/* > = 'N': Left Schur vectors (Q) are not computed;
 */
/* > = 'I': Q is initialized to the unit matrix and the matrix Q */
/* > of left Schur vectors of (H,T) is returned;
 */
/* > = 'V': Q must contain a unitary matrix Q1 on entry and */
/* > the product Q1*Q is returned. */
/* > \endverbatim */
/* > */
/* > \param[in] COMPZ */
/* > \verbatim */
/* > COMPZ is CHARACTER*1 */
/* > = 'N': Right Schur vectors (Z) are not computed;
 */
/* > = 'I': Q is initialized to the unit matrix and the matrix Z */
/* > of right Schur vectors of (H,T) is returned;
 */
/* > = 'V': Z must contain a unitary matrix Z1 on entry and */
/* > the product Z1*Z is returned. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* > N is INTEGER */
/* > The order of the matrices H, T, Q, and Z. N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] ILO */
/* > \verbatim */
/* > ILO is INTEGER */
/* > \endverbatim */
/* > */
/* > \param[in] IHI */
/* > \verbatim */
/* > IHI is INTEGER */
/* > ILO and IHI mark the rows and columns of H which are in */
/* > Hessenberg form. It is assumed that A is already upper */
/* > triangular in rows and columns 1:ILO-1 and IHI+1:N. */
/* > If N > 0, 1 <= ILO <= IHI <= N;
if N = 0, ILO=1 and IHI=0. */
/* > \endverbatim */
/* > */
/* > \param[in,out] H */
/* > \verbatim */
/* > H is COMPLEX*16 array, dimension (LDH, N) */
/* > On entry, the N-by-N upper Hessenberg matrix H. */
/* > On exit, if JOB = 'S', H contains the upper triangular */
/* > matrix S from the generalized Schur factorization. */
/* > If JOB = 'E', the diagonal of H matches that of S, but */
/* > the rest of H is unspecified. */
/* > \endverbatim */
/* > */
/* > \param[in] LDH */
/* > \verbatim */
/* > LDH is INTEGER */
/* > The leading dimension of the array H. LDH >= fla_max( 1, N ). */
/* > \endverbatim */
/* > */
/* > \param[in,out] T */
/* > \verbatim */
/* > T is COMPLEX*16 array, dimension (LDT, N) */
/* > On entry, the N-by-N upper triangular matrix T. */
/* > On exit, if JOB = 'S', T contains the upper triangular */
/* > matrix P from the generalized Schur factorization. */
/* > If JOB = 'E', the diagonal of T matches that of P, but */
/* > the rest of T is unspecified. */
/* > \endverbatim */
/* > */
/* > \param[in] LDT */
/* > \verbatim */
/* > LDT is INTEGER */
/* > The leading dimension of the array T. LDT >= fla_max( 1, N ). */
/* > \endverbatim */
/* > */
/* > \param[out] ALPHA */
/* > \verbatim */
/* > ALPHA is COMPLEX*16 array, dimension (N) */
/* > The complex scalars alpha that define the eigenvalues of */
/* > GNEP. ALPHA(i) = S(i,i) in the generalized Schur */
/* > factorization. */
/* > \endverbatim */
/* > */
/* > \param[out] BETA */
/* > \verbatim */
/* > BETA is COMPLEX*16 array, dimension (N) */
/* > The real non-negative scalars beta that define the */
/* > eigenvalues of GNEP. BETA(i) = P(i,i) in the generalized */
/* > Schur factorization. */
/* > */
/* > Together, the quantities alpha = ALPHA(j) and beta = BETA(j) */
/* > represent the j-th eigenvalue of the matrix pair (A,B), in */
/* > one of the forms lambda = alpha/beta or mu = beta/alpha. */
/* > Since either lambda or mu may overflow, they should not, */
/* > in general, be computed. */
/* > \endverbatim */
/* > */
/* > \param[in,out] Q */
/* > \verbatim */
/* > Q is COMPLEX*16 array, dimension (LDQ, N) */
/* > On entry, if COMPQ = 'V', the unitary matrix Q1 used in the */
/* > reduction of (A,B) to generalized Hessenberg form. */
/* > On exit, if COMPQ = 'I', the unitary matrix of left Schur */
/* > vectors of (H,T), and if COMPQ = 'V', the unitary matrix of */
/* > left Schur vectors of (A,B). */
/* > Not referenced if COMPQ = 'N'. */
/* > \endverbatim */
/* > */
/* > \param[in] LDQ */
/* > \verbatim */
/* > LDQ is INTEGER */
/* > The leading dimension of the array Q. LDQ >= 1. */
/* > If COMPQ='V' or 'I', then LDQ >= N. */
/* > \endverbatim */
/* > */
/* > \param[in,out] Z */
/* > \verbatim */
/* > Z is COMPLEX*16 array, dimension (LDZ, N) */
/* > On entry, if COMPZ = 'V', the unitary matrix Z1 used in the */
/* > reduction of (A,B) to generalized Hessenberg form. */
/* > On exit, if COMPZ = 'I', the unitary matrix of right Schur */
/* > vectors of (H,T), and if COMPZ = 'V', the unitary matrix of */
/* > right Schur vectors of (A,B). */
/* > Not referenced if COMPZ = 'N'. */
/* > \endverbatim */
/* > */
/* > \param[in] LDZ */
/* > \verbatim */
/* > LDZ is INTEGER */
/* > The leading dimension of the array Z. LDZ >= 1. */
/* > If COMPZ='V' or 'I', then LDZ >= N. */
/* > \endverbatim */
/* > */
/* > \param[out] WORK */
/* > \verbatim */
/* > WORK is COMPLEX*16 array, dimension (MAX(1,LWORK)) */
/* > On exit, if INFO >= 0, WORK(1) returns the optimal LWORK. */
/* > \endverbatim */
/* > */
/* > \param[in] LWORK */
/* > \verbatim */
/* > LWORK is INTEGER */
/* > The dimension of the array WORK. LWORK >= fla_max(1,N). */
/* > */
/* > If LWORK = -1, then a workspace query is assumed;
the routine */
/* > only calculates the optimal size of the WORK array, returns */
/* > this value as the first entry of the WORK array, and no error */
/* > message related to LWORK is issued by XERBLA. */
/* > \endverbatim */
/* > */
/* > \param[out] RWORK */
/* > \verbatim */
/* > RWORK is DOUBLE PRECISION array, dimension (N) */
/* > \endverbatim */
/* > */
/* > \param[out] INFO */
/* > \verbatim */
/* > INFO is INTEGER */
/* > = 0: successful exit */
/* > < 0: if INFO = -i, the i-th argument had an illegal value */
/* > = 1,...,N: the QZ iteration did not converge. (H,T) is not */
/* > in Schur form, but ALPHA(i) and BETA(i), */
/* > i=INFO+1,...,N should be correct. */
/* > = N+1,...,2*N: the shift calculation failed. (H,T) is not */
/* > in Schur form, but ALPHA(i) and BETA(i), */
/* > i=INFO-N+1,...,N should be correct. */
/* > \endverbatim */
/* Authors: */
/* ======== */
/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */
/* > \ingroup complex16GEcomputational */
/* > \par Further Details: */
/* ===================== */
/* > */
/* > \verbatim */
/* > */
/* > We assume that complex ABS works as long as its value is less than */
/* > overflow. */
/* > \endverbatim */
/* > */
/* ===================================================================== */
/* Subroutine */
void apply_grots_q(integer num_swps, integer *rots_per_swp, integer *rcol, doublereal **rots_sptr,
                   doublecomplex *q, integer n, integer ldq);
void apply_grots_z(integer num_swps, integer *rots_per_swp, integer *rcol, doublereal **rots_sptr,
                   doublecomplex *z, integer n, integer ldz, integer num_scal, integer *scol,
                   doublecomplex *scalv);
extern void zrot_(integer *, doublecomplex *, integer *, doublecomplex *, integer *, doublereal *,
                  doublecomplex *);
extern void zscal_(integer *, doublecomplex *, doublecomplex *, integer *);

void zhgeqz_(char *job, char *compq, char *compz, integer *n, integer *ilo, integer *ihi,
             doublecomplex *h__, integer *ldh, doublecomplex *t, integer *ldt, doublecomplex *alpha,
             doublecomplex *beta, doublecomplex *q, integer *ldq, doublecomplex *z__, integer *ldz,
             doublecomplex *work, integer *lwork, doublereal *rwork, integer *info)
{
    AOCL_DTL_TRACE_LOG_INIT
    AOCL_DTL_SNPRINTF("zhgeqz inputs: job %c, compq %c, compz %c, n %" FLA_IS ", ilo %" FLA_IS
                      ", ihi %" FLA_IS ", ldh %" FLA_IS ", ldt %" FLA_IS ", ldq %" FLA_IS
                      ", ldz %" FLA_IS "",
                      *job, *compq, *compz, *n, *ilo, *ihi, *ldh, *ldt, *ldq, *ldz);
    extern void fla_zhgeqz(char *job, char *compq, char *compz, integer *n, integer *ilo,
                           integer *ihi, doublecomplex *h__, integer *ldh, doublecomplex *t,
                           integer *ldt, doublecomplex *alpha, doublecomplex *beta,
                           doublecomplex *q, integer *ldq, doublecomplex *z__, integer *ldz,
                           doublecomplex *work, integer *lwork, doublereal *rwork, integer *info);
    fla_zhgeqz(job, compq, compz, n, ilo, ihi, h__, ldh, t, ldt, alpha, beta, q, ldq, z__, ldz,
               work, lwork, rwork, info);
    AOCL_DTL_TRACE_LOG_EXIT
    return;
}

void fla_zhgeqz(char *job, char *compq, char *compz, integer *n, integer *ilo, integer *ihi,
                doublecomplex *h__, integer *ldh, doublecomplex *t, integer *ldt,
                doublecomplex *alpha, doublecomplex *beta, doublecomplex *q, integer *ldq,
                doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork,
                doublereal *rwork, integer *info)
{
    /* System generated locals */
    integer h_dim1, h_offset, q_dim1, q_offset, t_dim1, t_offset, z_dim1, z_offset, i__1, i__2,
        i__3, i__4, i__5;
    doublereal d__1, d__2, d__3, d__4, d__5, d__6, d__7, d__8;
    doublecomplex z__1, z__2, z__3, z__4, z__5, z__6, z__7;
    /* Builtin functions */
    doublereal z_abs(doublecomplex *);
    doublereal d_imag(doublecomplex *);
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *),
        z_sqrt(doublecomplex *, doublecomplex *),
        pow_zi(doublecomplex *, doublecomplex *, integer *);
    /* Local variables */
#ifdef FLA_OPENMP_MULTITHREADING
    extern /* Function */
        int
        fla_thread_get_num_threads();
#endif
    doublereal c__;
    integer j;
    doublecomplex s, x, y;
    integer in;
#ifdef FLA_ENABLE_AMD_OPT
    doublecomplex sc;
#else
    integer i__6, jc, jr;
    void d_cnjg(doublecomplex *, doublecomplex *);
    double d_imag(doublecomplex *);
#endif
    doublecomplex u12;
    doublecomplex ad11, ad12, ad21, ad22;
    integer jch;
    logical ilq, ilz;
    doublereal ulp;
    doublecomplex abi12, abi22;
    doublereal absb, atol, btol, temp;
    extern /* Subroutine */
        void
        zrot_(integer *, doublecomplex *, integer *, doublecomplex *, integer *, doublereal *,
              doublecomplex *);
    doublereal temp2;
    extern logical lsame_(char *, char *, integer, integer);
    doublecomplex ctemp;
    integer iiter, ilast, jiter;
    doublereal anorm, bnorm;
    integer maxit;
    doublecomplex shift;
    doublereal tempr;
    doublecomplex ctemp2, ctemp3;
    logical ilazr2;
    doublereal ascale, bscale;
    extern doublereal dlamch_(char *);
    doublecomplex signbc;
    doublereal safmin;
    extern /* Subroutine */
        void
        xerbla_(const char *srname, const integer *info, ftnlen srname_len);
    doublecomplex eshift;
    logical ilschr;
    integer icompq, ilastm;
    extern /* Double Complex */
        VOID
        zladiv_f2c_(doublecomplex *, doublecomplex *, doublecomplex *);
    integer ischur;
    extern doublereal zlanhs_(char *, integer *, doublecomplex *, integer *, doublereal *);
    logical ilazro;
    integer icompz, ifirst;
    extern /* Subroutine */
        void
        zlartg_(doublecomplex *, doublecomplex *, doublereal *, doublecomplex *, doublecomplex *);
    integer ifrstm;
    extern /* Subroutine */
        void
        zlaset_(char *, integer *, integer *, doublecomplex *, doublecomplex *, doublecomplex *,
                integer *);
    integer istart;
    logical lquery;
#ifdef FLA_ENABLE_AMD_OPT
    int num_threads, tid;
    /* Initialize global context data */
    aocl_fla_init();
    doublereal c1;
    doublecomplex s1;
    integer max_swps = QZ_MAX_SWEEPS;
    integer num_swps, num_qrots, num_zrots;
    integer umem_sz;

    integer blk_enable = 0;

    integer *qrots_per_swp, *qcols;
    doublereal **qrots_st_ptr, *qrots, *qrots_ptr;

    integer *zrots_per_swp, *zcols;
    doublereal **zrots_st_ptr, *zrots, *zrots_ptr;

    integer *scal_cols;
    doublecomplex *scal_ptr;

    char *umem = NULL, *uptr;

    integer qidx, zidx, sidx;
    integer enable_opt = 0;

    num_swps = num_qrots = num_zrots = 0;
    umem_sz = 0;

    qidx = zidx = sidx = 0;

    qrots_per_swp = NULL;
    qcols = zcols = NULL;
    qrots_st_ptr = zrots_st_ptr = NULL;
    qrots = zrots = NULL;
    qrots_ptr = zrots_ptr = NULL;
    qrots_per_swp = zrots_per_swp = NULL;

    scal_cols = NULL;
    scal_ptr = NULL;
#endif /* FLA_ENABLE_AMD_OPT */
    /* -- LAPACK computational routine -- */
    /* -- LAPACK is a software package provided by Univ. of Tennessee, -- */
    /* -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
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
    /* Decode JOB, COMPQ, COMPZ */
    /* Parameter adjustments */
    h_dim1 = *ldh;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;
    t_dim1 = *ldt;
    t_offset = 1 + t_dim1;
    t -= t_offset;
    --alpha;
    --beta;
    q_dim1 = *ldq;
    q_offset = 1 + q_dim1;
    q -= q_offset;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1;
    z__ -= z_offset;
    --work;
    --rwork;
    /* Function Body */
    if(lsame_(job, "E", 1, 1))
    {
        ilschr = FALSE_;
        ischur = 1;
    }
    else if(lsame_(job, "S", 1, 1))
    {
        ilschr = TRUE_;
        ischur = 2;
    }
    else
    {
        ilschr = TRUE_;
        ischur = 0;
    }
    if(lsame_(compq, "N", 1, 1))
    {
        ilq = FALSE_;
        icompq = 1;
    }
    else if(lsame_(compq, "V", 1, 1))
    {
        ilq = TRUE_;
        icompq = 2;
    }
    else if(lsame_(compq, "I", 1, 1))
    {
        ilq = TRUE_;
        icompq = 3;
    }
    else
    {
        ilq = TRUE_;
        icompq = 0;
    }
    if(lsame_(compz, "N", 1, 1))
    {
        ilz = FALSE_;
        icompz = 1;
    }
    else if(lsame_(compz, "V", 1, 1))
    {
        ilz = TRUE_;
        icompz = 2;
    }
    else if(lsame_(compz, "I", 1, 1))
    {
        ilz = TRUE_;
        icompz = 3;
    }
    else
    {
        ilz = TRUE_;
        icompz = 0;
    }
    /* Check Argument Values */
    *info = 0;
    i__1 = fla_max(1, *n);
    work[1].r = (doublereal)i__1;
    work[1].i = 0.; // , expr subst
    lquery = *lwork == -1;
    if(ischur == 0)
    {
        *info = -1;
    }
    else if(icompq == 0)
    {
        *info = -2;
    }
    else if(icompz == 0)
    {
        *info = -3;
    }
    else if(*n < 0)
    {
        *info = -4;
    }
    else if(*ilo < 1)
    {
        *info = -5;
    }
    else if(*ihi > *n || *ihi < *ilo - 1)
    {
        *info = -6;
    }
    else if(*ldh < *n)
    {
        *info = -8;
    }
    else if(*ldt < *n)
    {
        *info = -10;
    }
    else if(*ldq < 1 || ilq && *ldq < *n)
    {
        *info = -14;
    }
    else if(*ldz < 1 || ilz && *ldz < *n)
    {
        *info = -16;
    }
    else if(*lwork < fla_max(1, *n) && !lquery)
    {
        *info = -18;
    }
    if(*info != 0)
    {
        i__1 = -(*info);
        xerbla_("ZHGEQZ", &i__1, (ftnlen)6);
        return;
    }
    else if(lquery)
    {
        return;
    }
    /* Quick return if possible */
    /* WORK( 1 ) = CMPLX( 1 ) */
    if(*n <= 0)
    {
        work[1].r = 1.;
        work[1].i = 0.; // , expr subst
        return;
    }
    /* Initialize Q and Z */
    if(icompq == 3)
    {
        zlaset_("Full", n, n, &c_b1, &c_b2, &q[q_offset], ldq);
    }
    if(icompz == 3)
    {
        zlaset_("Full", n, n, &c_b1, &c_b2, &z__[z_offset], ldz);
    }
    /* Machine Constants */
    in = *ihi + 1 - *ilo;
    safmin = dlamch_("S");
    ulp = dlamch_("E") * dlamch_("B");
    anorm = zlanhs_("F", &in, &h__[*ilo + *ilo * h_dim1], ldh, &rwork[1]);
    bnorm = zlanhs_("F", &in, &t[*ilo + *ilo * t_dim1], ldt, &rwork[1]);
    /* Computing MAX */
    d__1 = safmin;
    d__2 = ulp * anorm; // , expr subst
    atol = fla_max(d__1, d__2);
    /* Computing MAX */
    d__1 = safmin;
    d__2 = ulp * bnorm; // , expr subst
    btol = fla_max(d__1, d__2);
    ascale = 1. / fla_max(safmin, anorm);
    bscale = 1. / fla_max(safmin, bnorm);
    /* Set Eigenvalues IHI+1:N */
    i__1 = *n;
    for(j = *ihi + 1; j <= i__1; ++j)
    {
        absb = z_abs(&t[j + j * t_dim1]);
        if(absb > safmin)
        {
            i__2 = j + j * t_dim1;
            z__2.r = t[i__2].r / absb;
            z__2.i = t[i__2].i / absb; // , expr subst
            signbc.r = z__2.r;
            signbc.i = -z__2.i; // , expr subst
            i__2 = j + j * t_dim1;
            t[i__2].r = absb;
            t[i__2].i = 0.; // , expr subst
            if(ilschr)
            {
                i__2 = j - 1;
                zscal_(&i__2, &signbc, &t[j * t_dim1 + 1], &c__1);
                zscal_(&j, &signbc, &h__[j * h_dim1 + 1], &c__1);
            }
            else
            {
                zscal_(&c__1, &signbc, &h__[j + j * h_dim1], &c__1);
            }
            if(ilz)
            {
                zscal_(n, &signbc, &z__[j * z_dim1 + 1], &c__1);
            }
        }
        else
        {
            i__2 = j + j * t_dim1;
            t[i__2].r = 0.;
            t[i__2].i = 0.; // , expr subst
        }
        i__2 = j;
        i__3 = j + j * h_dim1;
        alpha[i__2].r = h__[i__3].r;
        alpha[i__2].i = h__[i__3].i; // , expr subst
        i__2 = j;
        i__3 = j + j * t_dim1;
        beta[i__2].r = t[i__3].r;
        beta[i__2].i = t[i__3].i; // , expr subst
        /* L10: */
    }
    /* If IHI < ILO, skip QZ steps */
    if(*ihi < *ilo)
    {
        goto L190;
    }
    /* MAIN QZ ITERATION LOOP */
    /* Initialize dynamic indices */
    /* Eigenvalues ILAST+1:N have been found. */
    /* Column operations modify rows IFRSTM:whatever */
    /* Row operations modify columns whatever:ILASTM */
    /* If only eigenvalues are being computed, then */
    /* IFRSTM is the row of the last splitting row above row ILAST;
     */
    /* this is always at least ILO. */
    /* IITER counts iterations since the last eigenvalue was found, */
    /* to tell when to use an extraordinary shift. */
    /* MAXIT is the maximum number of QZ sweeps allowed. */
    ilast = *ihi;
    if(ilschr)
    {
        ifrstm = 1;
        ilastm = *n;
    }
    else
    {
        ifrstm = *ilo;
        ilastm = *ihi;
    }
    iiter = 0;
    eshift.r = 0.;
    eshift.i = 0.; // , expr subst
    maxit = (*ihi - *ilo + 1) * 30;
    i__1 = maxit;
#ifdef FLA_ENABLE_AMD_OPT
    /* compute and allocate memory requirement for optimal computations of Q & Z */
    blk_enable = (ilq | ilz);
    if(blk_enable)
    {
        umem_sz = 0;
        if(ilq)
        {
            /* Memory requirement for Q computation:
             *
             * max_swps * sizeof(integer) - Array for number of rotations per sweep
             * max_swps * sizeof(integer) - Starting column for rotations
             * max_swps * sizeof(doublereal *) - Pointer array for starting locations of Q rotations
             * max_swps * *n * 3 * sizeof(doublereal) - Array of Q rotations for each sweep ((c, sr,
             * si)[])
             *
             * */
            umem_sz += max_swps
                       * (2 * sizeof(integer) + sizeof(doublereal *) + *n * 3 * sizeof(doublereal));
        }
        if(ilz)
        {
            /* Memory requirement for Z computation:
             *
             * max_swps * sizeof(integer) - Array for number of rotations per sweep
             * max_swps * sizeof(integer) - Starting column for rotations
             * max_swps * sizeof(doublereal *) - Pointer array for starting locations of Z rotations
             * max_swps * *n * 3 * sizeof(doublereal) - Array of Z rotations for each sweep ((c, sr,
             * si)[])
             *
             * Memory for Z Scaling:
             *
             * *n * sizeof(integer) - Array of indices for ZSCAL of Z
             * *n * sizeof(doublecomplex) - Array of scale factors
             * */
            umem_sz += max_swps
                       * (2 * sizeof(integer) + sizeof(doublereal *) + *n * 3 * sizeof(doublereal));
            umem_sz += *n * (sizeof(integer) + sizeof(doublecomplex));
        }

        /* Allocate the memory */
        umem = (char *)malloc(umem_sz);
        if(umem == NULL)
        {
            enable_opt = 0;
        }
        else
        {
            uptr = umem;
            enable_opt = 1;
        }

        /* Distribute the memory allocated */
        if(enable_opt && ilq)
        {
            qrots_per_swp = (integer *)uptr;
            uptr += max_swps * sizeof(integer);

            qcols = (integer *)uptr;
            uptr += max_swps * sizeof(integer);

            qrots_st_ptr = (doublereal **)uptr;
            uptr += max_swps * sizeof(doublereal *);

            qrots = (doublereal *)uptr;
            uptr += max_swps * *n * 3 * sizeof(doublereal);

            qrots_ptr = qrots;
        }
        if(enable_opt && ilz)
        {
            zrots_per_swp = (integer *)uptr;
            uptr += max_swps * sizeof(integer);

            zcols = (integer *)uptr;
            uptr += max_swps * sizeof(integer);

            zrots_st_ptr = (doublereal **)uptr;
            uptr += max_swps * sizeof(doublereal *);

            zrots = (doublereal *)uptr;
            uptr += max_swps * *n * 3 * sizeof(doublereal);

            scal_cols = (integer *)uptr;
            uptr += *n * sizeof(integer);

            scal_ptr = (doublecomplex *)uptr;
            uptr += *n * sizeof(doublecomplex);

            zrots_ptr = zrots;
        }
        sidx = qidx = zidx = 0;
    }
    else
    {
        enable_opt = 0;
    }
#endif /* FLA_ENABLE_AMD_OPT */
    for(jiter = 1; jiter <= i__1; ++jiter)
    {
        /* Check for too many iterations. */
        if(jiter > maxit)
        {
#ifdef FLA_ENABLE_AMD_OPT
            if(enable_opt)
            {
                if(ilq)
                {
                    apply_grots_q(qidx, qrots_per_swp, qcols, qrots_st_ptr, q, *n, q_dim1);
                    qidx = 0;
                    qrots_ptr = qrots;
                }
                if(ilz)
                {
                    apply_grots_z(zidx, zrots_per_swp, zcols, zrots_st_ptr, z__, *n, z_dim1, sidx,
                                  scal_cols, scal_ptr);
                    zidx = sidx = 0;
                    zrots_ptr = zrots;
                }
                num_swps = 0;
            }
#endif /* FLA_ENABLE_AMD_OPT */
            goto L180;
        }
        /* Split the matrix if possible. */
        /* Two tests: */
        /* 1: H(j,j-1)=0 or j=ILO */
        /* 2: T(j,j)=0 */
        /* Special case: j=ILAST */
        if(ilast == *ilo)
        {
            goto L60;
        }
        else
        {
            i__2 = ilast + (ilast - 1) * h_dim1;
            /* Computing MAX */
            i__3 = ilast + ilast * h_dim1;
            i__4 = ilast - 1 + (ilast - 1) * h_dim1;
            d__7 = safmin;
            d__8 = ulp
                   * ((d__1 = h__[i__3].r, f2c_abs(d__1))
                      + (d__2 = d_imag(&h__[ilast + ilast * h_dim1]), f2c_abs(d__2))
                      + ((d__3 = h__[i__4].r, f2c_abs(d__3))
                         + (d__4 = d_imag(&h__[ilast - 1 + (ilast - 1) * h_dim1]),
                            f2c_abs(d__4)))); // , expr subst
            if((d__5 = h__[i__2].r, f2c_abs(d__5))
                   + (d__6 = d_imag(&h__[ilast + (ilast - 1) * h_dim1]), f2c_abs(d__6))
               <= fla_max(d__7, d__8))
            {
                i__2 = ilast + (ilast - 1) * h_dim1;
                h__[i__2].r = 0.;
                h__[i__2].i = 0.; // , expr subst
                goto L60;
            }
        }
        /* Computing MAX */
        d__1 = safmin;
        d__2 = ulp
               * (z_abs(&t[ilast - 1 + ilast * t_dim1])
                  + z_abs(&t[ilast - 1 + (ilast - 1) * t_dim1])); // , expr subst
        if(z_abs(&t[ilast + ilast * t_dim1]) <= fla_max(d__1, d__2))
        {
#ifdef FLA_ENABLE_AMD_OPT
            if(enable_opt)
            {
                if(ilq)
                {
                    apply_grots_q(qidx, qrots_per_swp, qcols, qrots_st_ptr, q, *n, q_dim1);
                    qidx = 0;
                    qrots_ptr = qrots;
                }
                if(ilz)
                {
                    apply_grots_z(zidx, zrots_per_swp, zcols, zrots_st_ptr, z__, *n, z_dim1, sidx,
                                  scal_cols, scal_ptr);
                    zidx = sidx = 0;
                    zrots_ptr = zrots;
                }
                num_swps = 0;
            }
#endif /* FLA_ENABLE_AMD_OPT */
            i__2 = ilast + ilast * t_dim1;
            t[i__2].r = 0.;
            t[i__2].i = 0.; // , expr subst
            goto L50;
        }
        /* General case: j<ILAST */
        i__2 = *ilo;
        for(j = ilast - 1; j >= i__2; --j)
        {
            /* Test 1: for H(j,j-1)=0 or j=ILO */
            if(j == *ilo)
            {
                ilazro = TRUE_;
            }
            else
            {
                i__3 = j + (j - 1) * h_dim1;
                /* Computing MAX */
                i__4 = j + j * h_dim1;
                i__5 = j - 1 + (j - 1) * h_dim1;
                d__7 = safmin;
                d__8 = ulp
                       * ((d__1 = h__[i__4].r, f2c_abs(d__1))
                          + (d__2 = d_imag(&h__[j + j * h_dim1]), f2c_abs(d__2))
                          + ((d__3 = h__[i__5].r, f2c_abs(d__3))
                             + (d__4 = d_imag(&h__[j - 1 + (j - 1) * h_dim1]),
                                f2c_abs(d__4)))); // , expr subst
                if((d__5 = h__[i__3].r, f2c_abs(d__5))
                       + (d__6 = d_imag(&h__[j + (j - 1) * h_dim1]), f2c_abs(d__6))
                   <= fla_max(d__7, d__8))
                {
                    i__3 = j + (j - 1) * h_dim1;
                    h__[i__3].r = 0.;
                    h__[i__3].i = 0.; // , expr subst
                    ilazro = TRUE_;
                }
                else
                {
                    ilazro = FALSE_;
                }
            }
            /* Test 2: for T(j,j)=0 */
            temp = z_abs(&t[j + (j + 1) * t_dim1]);
            if(j > *ilo)
            {
                temp += z_abs(&t[j - 1 + j * t_dim1]);
            }
            /* Computing MAX */
            d__1 = safmin;
            d__2 = ulp * temp; // , expr subst
            if(z_abs(&t[j + j * t_dim1]) < fla_max(d__1, d__2))
            {
                i__3 = j + j * t_dim1;
                t[i__3].r = 0.;
                t[i__3].i = 0.; // , expr subst
                /* Test 1a: Check for 2 consecutive small subdiagonals in A */
                ilazr2 = FALSE_;
                if(!ilazro)
                {
                    i__3 = j + (j - 1) * h_dim1;
                    i__4 = j + 1 + j * h_dim1;
                    i__5 = j + j * h_dim1;
                    if(((d__1 = h__[i__3].r, f2c_abs(d__1))
                        + (d__2 = d_imag(&h__[j + (j - 1) * h_dim1]), f2c_abs(d__2)))
                           * (ascale
                              * ((d__3 = h__[i__4].r, f2c_abs(d__3))
                                 + (d__4 = d_imag(&h__[j + 1 + j * h_dim1]), f2c_abs(d__4))))
                       <= ((d__5 = h__[i__5].r, f2c_abs(d__5))
                           + (d__6 = d_imag(&h__[j + j * h_dim1]), f2c_abs(d__6)))
                              * (ascale * atol))
                    {
                        ilazr2 = TRUE_;
                    }
                }
#ifdef FLA_ENABLE_AMD_OPT
                if(enable_opt)
                {
                    if(ilq)
                    {
                        apply_grots_q(qidx, qrots_per_swp, qcols, qrots_st_ptr, q, *n, q_dim1);
                        qidx = 0;
                        qrots_ptr = qrots;
                    }
                    if(ilz)
                    {
                        apply_grots_z(zidx, zrots_per_swp, zcols, zrots_st_ptr, z__, *n, z_dim1,
                                      sidx, scal_cols, scal_ptr);
                        zidx = sidx = 0;
                        zrots_ptr = zrots;
                    }
                    num_swps = 0;
                }
#endif /* FLA_ENABLE_AMD_OPT */
                /* If both tests pass (1 & 2), i.e., the leading diagonal */
                /* element of B in the block is zero, split a 1x1 block off */
                /* at the top. (I.e., at the J-th row/column) The leading */
                /* diagonal element of the remainder can also be zero, so */
                /* this may have to be done repeatedly. */
                if(ilazro || ilazr2)
                {
                    i__3 = ilast - 1;
                    for(jch = j; jch <= i__3; ++jch)
                    {
                        i__4 = jch + jch * h_dim1;
                        ctemp.r = h__[i__4].r;
                        ctemp.i = h__[i__4].i; // , expr subst
                        zlartg_(&ctemp, &h__[jch + 1 + jch * h_dim1], &c__, &s,
                                &h__[jch + jch * h_dim1]);
                        i__4 = jch + 1 + jch * h_dim1;
                        h__[i__4].r = 0.;
                        h__[i__4].i = 0.; // , expr subst
                        i__4 = ilastm - jch;
                        zrot_(&i__4, &h__[jch + (jch + 1) * h_dim1], ldh,
                              &h__[jch + 1 + (jch + 1) * h_dim1], ldh, &c__, &s);
                        i__4 = ilastm - jch;
                        zrot_(&i__4, &t[jch + (jch + 1) * t_dim1], ldt,
                              &t[jch + 1 + (jch + 1) * t_dim1], ldt, &c__, &s);
                        if(ilq)
                        {
                            z__1.r = s.r;
                            z__1.i = -s.i;
                            zrot_(n, &q[jch * q_dim1 + 1], &c__1, &q[(jch + 1) * q_dim1 + 1], &c__1,
                                  &c__, &z__1);
                        }
                        if(ilazr2)
                        {
                            i__4 = jch + (jch - 1) * h_dim1;
                            i__5 = jch + (jch - 1) * h_dim1;
                            z__1.r = c__ * h__[i__5].r;
                            z__1.i = c__ * h__[i__5].i; // , expr subst
                            h__[i__4].r = z__1.r;
                            h__[i__4].i = z__1.i; // , expr subst
                        }
                        ilazr2 = FALSE_;
                        i__4 = jch + 1 + (jch + 1) * t_dim1;
                        if((d__1 = t[i__4].r, f2c_abs(d__1))
                               + (d__2 = d_imag(&t[jch + 1 + (jch + 1) * t_dim1]), f2c_abs(d__2))
                           >= btol)
                        {
                            if(jch + 1 >= ilast)
                            {
                                goto L60;
                            }
                            else
                            {
                                ifirst = jch + 1;
                                goto L70;
                            }
                        }
                        i__4 = jch + 1 + (jch + 1) * t_dim1;
                        t[i__4].r = 0.;
                        t[i__4].i = 0.; // , expr subst
                        /* L20: */
                    }
                    goto L50;
                }
                else
                {
                    /* Only test 2 passed -- chase the zero to T(ILAST,ILAST) */
                    /* Then process as in the case T(ILAST,ILAST)=0 */
                    i__3 = ilast - 1;
                    for(jch = j; jch <= i__3; ++jch)
                    {
                        i__4 = jch + (jch + 1) * t_dim1;
                        ctemp.r = t[i__4].r;
                        ctemp.i = t[i__4].i; // , expr subst
                        zlartg_(&ctemp, &t[jch + 1 + (jch + 1) * t_dim1], &c__, &s,
                                &t[jch + (jch + 1) * t_dim1]);
                        i__4 = jch + 1 + (jch + 1) * t_dim1;
                        t[i__4].r = 0.;
                        t[i__4].i = 0.; // , expr subst
                        if(jch < ilastm - 1)
                        {
                            i__4 = ilastm - jch - 1;
                            zrot_(&i__4, &t[jch + (jch + 2) * t_dim1], ldt,
                                  &t[jch + 1 + (jch + 2) * t_dim1], ldt, &c__, &s);
                        }
                        i__4 = ilastm - jch + 2;
                        zrot_(&i__4, &h__[jch + (jch - 1) * h_dim1], ldh,
                              &h__[jch + 1 + (jch - 1) * h_dim1], ldh, &c__, &s);
                        if(ilq)
                        {
                            z__1.r = s.r;
                            z__1.i = -s.i;
                            zrot_(n, &q[jch * q_dim1 + 1], &c__1, &q[(jch + 1) * q_dim1 + 1], &c__1,
                                  &c__, &z__1);
                        }
                        i__4 = jch + 1 + jch * h_dim1;
                        ctemp.r = h__[i__4].r;
                        ctemp.i = h__[i__4].i; // , expr subst
                        zlartg_(&ctemp, &h__[jch + 1 + (jch - 1) * h_dim1], &c__, &s,
                                &h__[jch + 1 + jch * h_dim1]);
                        i__4 = jch + 1 + (jch - 1) * h_dim1;
                        h__[i__4].r = 0.;
                        h__[i__4].i = 0.; // , expr subst
                        i__4 = jch + 1 - ifrstm;
                        zrot_(&i__4, &h__[ifrstm + jch * h_dim1], &c__1,
                              &h__[ifrstm + (jch - 1) * h_dim1], &c__1, &c__, &s);
                        i__4 = jch - ifrstm;
                        zrot_(&i__4, &t[ifrstm + jch * t_dim1], &c__1,
                              &t[ifrstm + (jch - 1) * t_dim1], &c__1, &c__, &s);
                        if(ilz)
                        {
                            zrot_(n, &z__[jch * z_dim1 + 1], &c__1, &z__[(jch - 1) * z_dim1 + 1],
                                  &c__1, &c__, &s);
                        }
                        /* L30: */
                    }
                    goto L50;
                }
            }
            else if(ilazro)
            {
                /* Only test 1 passed -- work on J:ILAST */
                ifirst = j;
                goto L70;
            }
            /* Neither test passed -- try next J */
            /* L40: */
        }
        /* (Drop-through is "impossible") */
        *info = (*n << 1) + 1;
        goto L210;
        /* T(ILAST,ILAST)=0 -- clear H(ILAST,ILAST-1) to split off a */
        /* 1x1 block. */
    L50:
        i__2 = ilast + ilast * h_dim1;
        ctemp.r = h__[i__2].r;
        ctemp.i = h__[i__2].i; // , expr subst
        zlartg_(&ctemp, &h__[ilast + (ilast - 1) * h_dim1], &c__, &s, &h__[ilast + ilast * h_dim1]);
        i__2 = ilast + (ilast - 1) * h_dim1;
        h__[i__2].r = 0.;
        h__[i__2].i = 0.; // , expr subst
        i__2 = ilast - ifrstm;
        zrot_(&i__2, &h__[ifrstm + ilast * h_dim1], &c__1, &h__[ifrstm + (ilast - 1) * h_dim1],
              &c__1, &c__, &s);
        i__2 = ilast - ifrstm;
        zrot_(&i__2, &t[ifrstm + ilast * t_dim1], &c__1, &t[ifrstm + (ilast - 1) * t_dim1], &c__1,
              &c__, &s);
        if(ilz)
        {
            zrot_(n, &z__[ilast * z_dim1 + 1], &c__1, &z__[(ilast - 1) * z_dim1 + 1], &c__1, &c__,
                  &s);
        }
        /* H(ILAST,ILAST-1)=0 -- Standardize B, set ALPHA and BETA */
    L60:
        absb = z_abs(&t[ilast + ilast * t_dim1]);
        if(absb > safmin)
        {
            i__2 = ilast + ilast * t_dim1;
            z__2.r = t[i__2].r / absb;
            z__2.i = t[i__2].i / absb; // , expr subst
            signbc.r = z__2.r;
            signbc.i = -z__2.i; // , expr subst
            i__2 = ilast + ilast * t_dim1;
            t[i__2].r = absb;
            t[i__2].i = 0.; // , expr subst
            if(ilschr)
            {
                i__2 = ilast - ifrstm;
                zscal_(&i__2, &signbc, &t[ifrstm + ilast * t_dim1], &c__1);
                i__2 = ilast + 1 - ifrstm;
                zscal_(&i__2, &signbc, &h__[ifrstm + ilast * h_dim1], &c__1);
            }
            else
            {
                zscal_(&c__1, &signbc, &h__[ilast + ilast * h_dim1], &c__1);
            }
#ifdef FLA_ENABLE_AMD_OPT
            if(enable_opt && ilz)
            {
                scal_cols[sidx] = ilast;
                scal_ptr[sidx].r = signbc.r;
                scal_ptr[sidx].i = signbc.i;
                sidx++;
            }
            else if(ilz)
            {
                zscal_(n, &signbc, &z__[ilast * z_dim1 + 1], &c__1);
            }
#else /* FLA_ENABLE_AMD_OPT */
            if(ilz)
            {
                zscal_(n, &signbc, &z__[ilast * z_dim1 + 1], &c__1);
            }
#endif /* FLA_ENABLE_AMD_OPT */
        }
        else
        {
            i__2 = ilast + ilast * t_dim1;
            t[i__2].r = 0.;
            t[i__2].i = 0.; // , expr subst
        }
        i__2 = ilast;
        i__3 = ilast + ilast * h_dim1;
        alpha[i__2].r = h__[i__3].r;
        alpha[i__2].i = h__[i__3].i; // , expr subst
        i__2 = ilast;
        i__3 = ilast + ilast * t_dim1;
        beta[i__2].r = t[i__3].r;
        beta[i__2].i = t[i__3].i; // , expr subst
        /* Go to next block -- exit if finished. */
        --ilast;
        if(ilast < *ilo)
        {
            goto L190;
        }
        /* Reset counters */
        iiter = 0;
        eshift.r = 0.;
        eshift.i = 0.; // , expr subst
        if(!ilschr)
        {
            ilastm = ilast;
            if(ifrstm > ilast)
            {
                ifrstm = *ilo;
            }
        }
        goto L160;
        /* QZ step */
        /* This iteration only involves rows/columns IFIRST:ILAST. We */
        /* assume IFIRST < ILAST, and that the diagonal of B is non-zero. */
    L70:
        ++iiter;
        if(!ilschr)
        {
            ifrstm = ifirst;
        }
        /* Compute the Shift. */
        /* At this point, IFIRST < ILAST, and the diagonal elements of */
        /* T(IFIRST:ILAST,IFIRST,ILAST) are larger than BTOL (in */
        /* magnitude) */
        if(iiter / 10 * 10 != iiter)
        {
            /* The Wilkinson shift (AEP p.512), i.e., the eigenvalue of */
            /* the bottom-right 2x2 block of A inv(B) which is nearest to */
            /* the bottom-right element. */
            /* We factor B as U*D, where U has unit diagonals, and */
            /* compute (A*inv(D))*inv(U). */
            i__2 = ilast - 1 + ilast * t_dim1;
            z__2.r = bscale * t[i__2].r;
            z__2.i = bscale * t[i__2].i; // , expr subst
            i__3 = ilast + ilast * t_dim1;
            z__3.r = bscale * t[i__3].r;
            z__3.i = bscale * t[i__3].i; // , expr subst
            z_div(&z__1, &z__2, &z__3);
            u12.r = z__1.r;
            u12.i = z__1.i; // , expr subst
            i__2 = ilast - 1 + (ilast - 1) * h_dim1;
            z__2.r = ascale * h__[i__2].r;
            z__2.i = ascale * h__[i__2].i; // , expr subst
            i__3 = ilast - 1 + (ilast - 1) * t_dim1;
            z__3.r = bscale * t[i__3].r;
            z__3.i = bscale * t[i__3].i; // , expr subst
            z_div(&z__1, &z__2, &z__3);
            ad11.r = z__1.r;
            ad11.i = z__1.i; // , expr subst
            i__2 = ilast + (ilast - 1) * h_dim1;
            z__2.r = ascale * h__[i__2].r;
            z__2.i = ascale * h__[i__2].i; // , expr subst
            i__3 = ilast - 1 + (ilast - 1) * t_dim1;
            z__3.r = bscale * t[i__3].r;
            z__3.i = bscale * t[i__3].i; // , expr subst
            z_div(&z__1, &z__2, &z__3);
            ad21.r = z__1.r;
            ad21.i = z__1.i; // , expr subst
            i__2 = ilast - 1 + ilast * h_dim1;
            z__2.r = ascale * h__[i__2].r;
            z__2.i = ascale * h__[i__2].i; // , expr subst
            i__3 = ilast + ilast * t_dim1;
            z__3.r = bscale * t[i__3].r;
            z__3.i = bscale * t[i__3].i; // , expr subst
            z_div(&z__1, &z__2, &z__3);
            ad12.r = z__1.r;
            ad12.i = z__1.i; // , expr subst
            i__2 = ilast + ilast * h_dim1;
            z__2.r = ascale * h__[i__2].r;
            z__2.i = ascale * h__[i__2].i; // , expr subst
            i__3 = ilast + ilast * t_dim1;
            z__3.r = bscale * t[i__3].r;
            z__3.i = bscale * t[i__3].i; // , expr subst
            z_div(&z__1, &z__2, &z__3);
            ad22.r = z__1.r;
            ad22.i = z__1.i; // , expr subst
            z__2.r = u12.r * ad21.r - u12.i * ad21.i;
            z__2.i = u12.r * ad21.i + u12.i * ad21.r; // , expr subst
            z__1.r = ad22.r - z__2.r;
            z__1.i = ad22.i - z__2.i; // , expr subst
            abi22.r = z__1.r;
            abi22.i = z__1.i; // , expr subst
            z__2.r = u12.r * ad11.r - u12.i * ad11.i;
            z__2.i = u12.r * ad11.i + u12.i * ad11.r; // , expr subst
            z__1.r = ad12.r - z__2.r;
            z__1.i = ad12.i - z__2.i; // , expr subst
            abi12.r = z__1.r;
            abi12.i = z__1.i; // , expr subst
            shift.r = abi22.r;
            shift.i = abi22.i; // , expr subst
            z_sqrt(&z__2, &abi12);
            z_sqrt(&z__3, &ad21);
            z__1.r = z__2.r * z__3.r - z__2.i * z__3.i;
            z__1.i = z__2.r * z__3.i + z__2.i * z__3.r; // , expr subst
            ctemp.r = z__1.r;
            ctemp.i = z__1.i; // , expr subst
            temp = (d__1 = ctemp.r, f2c_abs(d__1)) + (d__2 = d_imag(&ctemp), f2c_abs(d__2));
            if(ctemp.r != 0. || ctemp.i != 0.)
            {
                z__2.r = ad11.r - shift.r;
                z__2.i = ad11.i - shift.i; // , expr subst
                z__1.r = z__2.r * .5;
                z__1.i = z__2.i * .5; // , expr subst
                x.r = z__1.r;
                x.i = z__1.i; // , expr subst
                temp2 = (d__1 = x.r, f2c_abs(d__1)) + (d__2 = d_imag(&x), f2c_abs(d__2));
                /* Computing MAX */
                d__3 = temp;
                d__4 = (d__1 = x.r, f2c_abs(d__1))
                       + (d__2 = d_imag(&x), f2c_abs(d__2)); // , expr subst
                temp = fla_max(d__3, d__4);
                z__5.r = x.r / temp;
                z__5.i = x.i / temp; // , expr subst
                pow_zi(&z__4, &z__5, &c__2);
                z__7.r = ctemp.r / temp;
                z__7.i = ctemp.i / temp; // , expr subst
                pow_zi(&z__6, &z__7, &c__2);
                z__3.r = z__4.r + z__6.r;
                z__3.i = z__4.i + z__6.i; // , expr subst
                z_sqrt(&z__2, &z__3);
                z__1.r = temp * z__2.r;
                z__1.i = temp * z__2.i; // , expr subst
                y.r = z__1.r;
                y.i = z__1.i; // , expr subst
                if(temp2 > 0.)
                {
                    z__1.r = x.r / temp2;
                    z__1.i = x.i / temp2; // , expr subst
                    z__2.r = x.r / temp2;
                    z__2.i = x.i / temp2; // , expr subst
                    if(z__1.r * y.r + d_imag(&z__2) * d_imag(&y) < 0.)
                    {
                        z__3.r = -y.r;
                        z__3.i = -y.i; // , expr subst
                        y.r = z__3.r;
                        y.i = z__3.i; // , expr subst
                    }
                }
                z__4.r = x.r + y.r;
                z__4.i = x.i + y.i; // , expr subst
                zladiv_f2c_(&z__3, &ctemp, &z__4);
                z__2.r = ctemp.r * z__3.r - ctemp.i * z__3.i;
                z__2.i = ctemp.r * z__3.i + ctemp.i * z__3.r; // , expr subst
                z__1.r = shift.r - z__2.r;
                z__1.i = shift.i - z__2.i; // , expr subst
                shift.r = z__1.r;
                shift.i = z__1.i; // , expr subst
            }
        }
        else
        {
            /* Exceptional shift. Chosen for no particularly good reason. */
            i__2 = ilast + ilast * t_dim1;
            if(iiter / 20 * 20 == iiter
               && bscale
                          * ((d__1 = t[i__2].r, f2c_abs(d__1))
                             + (d__2 = d_imag(&t[ilast + ilast * t_dim1]), f2c_abs(d__2)))
                      > safmin)
            {
                i__2 = ilast + ilast * h_dim1;
                z__3.r = ascale * h__[i__2].r;
                z__3.i = ascale * h__[i__2].i; // , expr subst
                i__3 = ilast + ilast * t_dim1;
                z__4.r = bscale * t[i__3].r;
                z__4.i = bscale * t[i__3].i; // , expr subst
                z_div(&z__2, &z__3, &z__4);
                z__1.r = eshift.r + z__2.r;
                z__1.i = eshift.i + z__2.i; // , expr subst
                eshift.r = z__1.r;
                eshift.i = z__1.i; // , expr subst
            }
            else
            {
                i__2 = ilast + (ilast - 1) * h_dim1;
                z__3.r = ascale * h__[i__2].r;
                z__3.i = ascale * h__[i__2].i; // , expr subst
                i__3 = ilast - 1 + (ilast - 1) * t_dim1;
                z__4.r = bscale * t[i__3].r;
                z__4.i = bscale * t[i__3].i; // , expr subst
                z_div(&z__2, &z__3, &z__4);
                z__1.r = eshift.r + z__2.r;
                z__1.i = eshift.i + z__2.i; // , expr subst
                eshift.r = z__1.r;
                eshift.i = z__1.i; // , expr subst
            }
            shift.r = eshift.r;
            shift.i = eshift.i; // , expr subst
        }
        /* Now check for two consecutive small subdiagonals. */
        i__2 = ifirst + 1;
        for(j = ilast - 1; j >= i__2; --j)
        {
            istart = j;
            i__3 = j + j * h_dim1;
            z__2.r = ascale * h__[i__3].r;
            z__2.i = ascale * h__[i__3].i; // , expr subst
            i__4 = j + j * t_dim1;
            z__4.r = bscale * t[i__4].r;
            z__4.i = bscale * t[i__4].i; // , expr subst
            z__3.r = shift.r * z__4.r - shift.i * z__4.i;
            z__3.i = shift.r * z__4.i + shift.i * z__4.r; // , expr subst
            z__1.r = z__2.r - z__3.r;
            z__1.i = z__2.i - z__3.i; // , expr subst
            ctemp.r = z__1.r;
            ctemp.i = z__1.i; // , expr subst
            temp = (d__1 = ctemp.r, f2c_abs(d__1)) + (d__2 = d_imag(&ctemp), f2c_abs(d__2));
            i__3 = j + 1 + j * h_dim1;
            temp2 = ascale
                    * ((d__1 = h__[i__3].r, f2c_abs(d__1))
                       + (d__2 = d_imag(&h__[j + 1 + j * h_dim1]), f2c_abs(d__2)));
            tempr = fla_max(temp, temp2);
            if(tempr < 1. && tempr != 0.)
            {
                temp /= tempr;
                temp2 /= tempr;
            }
            i__3 = j + (j - 1) * h_dim1;
            if(((d__1 = h__[i__3].r, f2c_abs(d__1))
                + (d__2 = d_imag(&h__[j + (j - 1) * h_dim1]), f2c_abs(d__2)))
                   * temp2
               <= temp * atol)
            {
                goto L90;
            }
            /* L80: */
        }
        istart = ifirst;
        i__2 = ifirst + ifirst * h_dim1;
        z__2.r = ascale * h__[i__2].r;
        z__2.i = ascale * h__[i__2].i; // , expr subst
        i__3 = ifirst + ifirst * t_dim1;
        z__4.r = bscale * t[i__3].r;
        z__4.i = bscale * t[i__3].i; // , expr subst
        z__3.r = shift.r * z__4.r - shift.i * z__4.i;
        z__3.i = shift.r * z__4.i + shift.i * z__4.r; // , expr subst
        z__1.r = z__2.r - z__3.r;
        z__1.i = z__2.i - z__3.i; // , expr subst
        ctemp.r = z__1.r;
        ctemp.i = z__1.i; // , expr subst
    L90: /* Do an implicit-shift QZ sweep. */
        /* Initial Q */
        i__2 = istart + 1 + istart * h_dim1;
        z__1.r = ascale * h__[i__2].r;
        z__1.i = ascale * h__[i__2].i; // , expr subst
        ctemp2.r = z__1.r;
        ctemp2.i = z__1.i; // , expr subst
        zlartg_(&ctemp, &ctemp2, &c__, &s, &ctemp3);
        /* Sweep */
        i__2 = ilast - 1;
#ifdef FLA_ENABLE_AMD_OPT
        if(enable_opt && blk_enable)
        {
            if(num_swps == max_swps)
            {
                if(ilq)
                {
                    apply_grots_q(qidx, qrots_per_swp, qcols, qrots_st_ptr, q, *n, q_dim1);
                    qidx = 0;
                    qrots_ptr = qrots;
                }
                if(ilz)
                {
                    apply_grots_z(zidx, zrots_per_swp, zcols, zrots_st_ptr, z__, *n, z_dim1, sidx,
                                  scal_cols, scal_ptr);
                    zidx = sidx = 0;
                    zrots_ptr = zrots;
                }
                num_swps = 0;
            }

            if(ilq)
            {
                num_qrots = 0;
                qcols[qidx] = istart;
                qrots_st_ptr[qidx] = qrots_ptr;
            }

            if(ilz)
            {
                num_zrots = 0;
                zcols[zidx] = istart;
                zrots_st_ptr[zidx] = zrots_ptr;
            }

            num_swps++;
        }

#ifdef FLA_OPENMP_MULTITHREADING
        num_threads = fla_thread_get_num_threads();

        num_threads = fla_min(2, num_threads);
#pragma omp parallel num_threads(num_threads) private(j, i__3, i__4, i__5, tid)
        {
            tid = omp_get_thread_num();
#else
        num_threads = 1;
        tid = 0;
        {
#endif
            for(j = istart; j <= i__2; ++j)
            {
                if(tid == 0)
                {
                    if(j > istart)
                    {
                        i__3 = j + (j - 1) * h_dim1;
                        ctemp.r = h__[i__3].r;
                        ctemp.i = h__[i__3].i; // , expr subst
                        zlartg_(&ctemp, &h__[j + 1 + (j - 1) * h_dim1], &c__, &s,
                                &h__[j + (j - 1) * h_dim1]);
                        i__3 = j + 1 + (j - 1) * h_dim1;
                        h__[i__3].r = 0.;
                        h__[i__3].i = 0.; // , expr subst
                    }
                }
#ifdef FLA_OPENMP_MULTITHREADING
#pragma omp barrier
#endif
                i__3 = ilastm - j + 1;
                if(i__3 >= 0)
                {
                    if(tid == 0)
                    {
                        i__4 = j + j * h_dim1;
                        i__5 = j + 1 + j * h_dim1;
                        zrot_(&i__3, &h__[i__4], &h_dim1, &h__[i__5], &h_dim1, &c__, &s);
                    }
                    if(tid == 1 || num_threads == 1)
                    {
                        i__4 = j + j * t_dim1;
                        i__5 = j + 1 + j * t_dim1;
                        zrot_(&i__3, &t[i__4], &t_dim1, &t[i__5], &t_dim1, &c__, &s);
                    }
                }
                if(tid == 1 || num_threads == 1)
                {
                    if(ilq)
                    {
                        if(enable_opt)
                        {
                            num_qrots++;
                            *qrots_ptr++ = c__;
                            *qrots_ptr++ = s.r;
                            *qrots_ptr++ = -s.i;
                        }
                        else
                        {
                            i__4 = 1 + (j)*q_dim1;
                            i__5 = 1 + (j + 1) * q_dim1;
                            sc.r = s.r;
                            sc.i = -s.i;
                            zrot_(n, &q[i__4], &c__1, &q[i__5], &c__1, &c__, &sc);
                        }
                    }
                    i__3 = j + 1 + (j + 1) * t_dim1;
                    ctemp.r = t[i__3].r;
                    ctemp.i = t[i__3].i; // , expr subst
                    zlartg_(&ctemp, &t[j + 1 + j * t_dim1], &c1, &s1, &t[j + 1 + (j + 1) * t_dim1]);
                    i__3 = j + 1 + j * t_dim1;
                    t[i__3].r = 0.;
                    t[i__3].i = 0.; // , expr subst
                }
#ifdef FLA_OPENMP_MULTITHREADING
#pragma omp barrier
#endif
                if(tid == 0)
                {
                    i__3 = fla_min(j + 2, ilast) - ifrstm + 1;
                    if(i__3 >= 0)
                    {
                        i__4 = ifrstm + (j + 1) * h_dim1;
                        i__5 = ifrstm + (j)*h_dim1;
                        zrot_(&i__3, &h__[i__4], &c__1, &h__[i__5], &c__1, &c1, &s1);
                    }
                }
                if(tid == 1 || num_threads == 1)
                {
                    i__3 = j - ifrstm + 1;
                    if(i__3 >= 0)
                    {
                        i__4 = ifrstm + (j + 1) * t_dim1;
                        i__5 = ifrstm + (j)*t_dim1;
                        zrot_(&i__3, &t[i__4], &c__1, &t[i__5], &c__1, &c1, &s1);
                    }
                    if(ilz)
                    {
                        if(enable_opt)
                        {
                            num_zrots++;
                            *zrots_ptr++ = c1;
                            *zrots_ptr++ = s1.r;
                            *zrots_ptr++ = s1.i;
                        }
                        else
                        {
                            i__4 = 1 + (j + 1) * z_dim1;
                            i__5 = 1 + (j)*z_dim1;
                            zrot_(n, &z__[i__4], &c__1, &z__[i__5], &c__1, &c1, &s1);
                        }
                    }
                }
                /* L150: */
            }
        }
        if(enable_opt)
        {
            if(ilq)
            {
                qrots_per_swp[qidx] = num_qrots;
                qidx++;
            }

            if(ilz)
            {
                zrots_per_swp[zidx] = num_zrots;
                zidx++;
            }
        }
#else /* FLA_ENABLE_AMD_OPT */
        for(j = istart; j <= i__2; ++j)
        {
            if(j > istart)
            {
                i__3 = j + (j - 1) * h_dim1;
                ctemp.r = h__[i__3].r;
                ctemp.i = h__[i__3].i; // , expr subst
                zlartg_(&ctemp, &h__[j + 1 + (j - 1) * h_dim1], &c__, &s,
                        &h__[j + (j - 1) * h_dim1]);
                i__3 = j + 1 + (j - 1) * h_dim1;
                h__[i__3].r = 0.;
                h__[i__3].i = 0.; // , expr subst
            }
            i__3 = ilastm;
            for(jc = j; jc <= i__3; ++jc)
            {
                i__4 = j + jc * h_dim1;
                z__2.r = c__ * h__[i__4].r;
                z__2.i = c__ * h__[i__4].i; // , expr subst
                i__5 = j + 1 + jc * h_dim1;
                z__3.r = s.r * h__[i__5].r - s.i * h__[i__5].i;
                z__3.i = s.r * h__[i__5].i + s.i * h__[i__5].r; // , expr subst
                z__1.r = z__2.r + z__3.r;
                z__1.i = z__2.i + z__3.i; // , expr subst
                ctemp.r = z__1.r;
                ctemp.i = z__1.i; // , expr subst
                i__4 = j + 1 + jc * h_dim1;
                d_cnjg(&z__4, &s);
                z__3.r = -z__4.r;
                z__3.i = -z__4.i; // , expr subst
                i__5 = j + jc * h_dim1;
                z__2.r = z__3.r * h__[i__5].r - z__3.i * h__[i__5].i;
                z__2.i = z__3.r * h__[i__5].i + z__3.i * h__[i__5].r; // , expr subst
                i__6 = j + 1 + jc * h_dim1;
                z__5.r = c__ * h__[i__6].r;
                z__5.i = c__ * h__[i__6].i; // , expr subst
                z__1.r = z__2.r + z__5.r;
                z__1.i = z__2.i + z__5.i; // , expr subst
                h__[i__4].r = z__1.r;
                h__[i__4].i = z__1.i; // , expr subst
                i__4 = j + jc * h_dim1;
                h__[i__4].r = ctemp.r;
                h__[i__4].i = ctemp.i; // , expr subst
                i__4 = j + jc * t_dim1;
                z__2.r = c__ * t[i__4].r;
                z__2.i = c__ * t[i__4].i; // , expr subst
                i__5 = j + 1 + jc * t_dim1;
                z__3.r = s.r * t[i__5].r - s.i * t[i__5].i;
                z__3.i = s.r * t[i__5].i + s.i * t[i__5].r; // , expr subst
                z__1.r = z__2.r + z__3.r;
                z__1.i = z__2.i + z__3.i; // , expr subst
                ctemp2.r = z__1.r;
                ctemp2.i = z__1.i; // , expr subst
                i__4 = j + 1 + jc * t_dim1;
                d_cnjg(&z__4, &s);
                z__3.r = -z__4.r;
                z__3.i = -z__4.i; // , expr subst
                i__5 = j + jc * t_dim1;
                z__2.r = z__3.r * t[i__5].r - z__3.i * t[i__5].i;
                z__2.i = z__3.r * t[i__5].i + z__3.i * t[i__5].r; // , expr subst
                i__6 = j + 1 + jc * t_dim1;
                z__5.r = c__ * t[i__6].r;
                z__5.i = c__ * t[i__6].i; // , expr subst
                z__1.r = z__2.r + z__5.r;
                z__1.i = z__2.i + z__5.i; // , expr subst
                t[i__4].r = z__1.r;
                t[i__4].i = z__1.i; // , expr subst
                i__4 = j + jc * t_dim1;
                t[i__4].r = ctemp2.r;
                t[i__4].i = ctemp2.i; // , expr subst
                /* L100: */
            }
            if(ilq)
            {
                i__3 = *n;
                for(jr = 1; jr <= i__3; ++jr)
                {
                    i__4 = jr + j * q_dim1;
                    z__2.r = c__ * q[i__4].r;
                    z__2.i = c__ * q[i__4].i; // , expr subst
                    d_cnjg(&z__4, &s);
                    i__5 = jr + (j + 1) * q_dim1;
                    z__3.r = z__4.r * q[i__5].r - z__4.i * q[i__5].i;
                    z__3.i = z__4.r * q[i__5].i + z__4.i * q[i__5].r; // , expr subst
                    z__1.r = z__2.r + z__3.r;
                    z__1.i = z__2.i + z__3.i; // , expr subst
                    ctemp.r = z__1.r;
                    ctemp.i = z__1.i; // , expr subst
                    i__4 = jr + (j + 1) * q_dim1;
                    z__3.r = -s.r;
                    z__3.i = -s.i; // , expr subst
                    i__5 = jr + j * q_dim1;
                    z__2.r = z__3.r * q[i__5].r - z__3.i * q[i__5].i;
                    z__2.i = z__3.r * q[i__5].i + z__3.i * q[i__5].r; // , expr subst
                    i__6 = jr + (j + 1) * q_dim1;
                    z__4.r = c__ * q[i__6].r;
                    z__4.i = c__ * q[i__6].i; // , expr subst
                    z__1.r = z__2.r + z__4.r;
                    z__1.i = z__2.i + z__4.i; // , expr subst
                    q[i__4].r = z__1.r;
                    q[i__4].i = z__1.i; // , expr subst
                    i__4 = jr + j * q_dim1;
                    q[i__4].r = ctemp.r;
                    q[i__4].i = ctemp.i; // , expr subst
                    /* L110: */
                }
            }
            i__3 = j + 1 + (j + 1) * t_dim1;
            ctemp.r = t[i__3].r;
            ctemp.i = t[i__3].i; // , expr subst
            zlartg_(&ctemp, &t[j + 1 + j * t_dim1], &c__, &s, &t[j + 1 + (j + 1) * t_dim1]);
            i__3 = j + 1 + j * t_dim1;
            t[i__3].r = 0.;
            t[i__3].i = 0.; // , expr subst
            /* Computing MIN */
            i__4 = j + 2;
            i__3 = fla_min(i__4, ilast);
            for(jr = ifrstm; jr <= i__3; ++jr)
            {
                i__4 = jr + (j + 1) * h_dim1;
                z__2.r = c__ * h__[i__4].r;
                z__2.i = c__ * h__[i__4].i; // , expr subst
                i__5 = jr + j * h_dim1;
                z__3.r = s.r * h__[i__5].r - s.i * h__[i__5].i;
                z__3.i = s.r * h__[i__5].i + s.i * h__[i__5].r; // , expr subst
                z__1.r = z__2.r + z__3.r;
                z__1.i = z__2.i + z__3.i; // , expr subst
                ctemp.r = z__1.r;
                ctemp.i = z__1.i; // , expr subst
                i__4 = jr + j * h_dim1;
                d_cnjg(&z__4, &s);
                z__3.r = -z__4.r;
                z__3.i = -z__4.i; // , expr subst
                i__5 = jr + (j + 1) * h_dim1;
                z__2.r = z__3.r * h__[i__5].r - z__3.i * h__[i__5].i;
                z__2.i = z__3.r * h__[i__5].i + z__3.i * h__[i__5].r; // , expr subst
                i__6 = jr + j * h_dim1;
                z__5.r = c__ * h__[i__6].r;
                z__5.i = c__ * h__[i__6].i; // , expr subst
                z__1.r = z__2.r + z__5.r;
                z__1.i = z__2.i + z__5.i; // , expr subst
                h__[i__4].r = z__1.r;
                h__[i__4].i = z__1.i; // , expr subst
                i__4 = jr + (j + 1) * h_dim1;
                h__[i__4].r = ctemp.r;
                h__[i__4].i = ctemp.i; // , expr subst
                /* L120: */
            }
            i__3 = j;
            for(jr = ifrstm; jr <= i__3; ++jr)
            {
                i__4 = jr + (j + 1) * t_dim1;
                z__2.r = c__ * t[i__4].r;
                z__2.i = c__ * t[i__4].i; // , expr subst
                i__5 = jr + j * t_dim1;
                z__3.r = s.r * t[i__5].r - s.i * t[i__5].i;
                z__3.i = s.r * t[i__5].i + s.i * t[i__5].r; // , expr subst
                z__1.r = z__2.r + z__3.r;
                z__1.i = z__2.i + z__3.i; // , expr subst
                ctemp.r = z__1.r;
                ctemp.i = z__1.i; // , expr subst
                i__4 = jr + j * t_dim1;
                d_cnjg(&z__4, &s);
                z__3.r = -z__4.r;
                z__3.i = -z__4.i; // , expr subst
                i__5 = jr + (j + 1) * t_dim1;
                z__2.r = z__3.r * t[i__5].r - z__3.i * t[i__5].i;
                z__2.i = z__3.r * t[i__5].i + z__3.i * t[i__5].r; // , expr subst
                i__6 = jr + j * t_dim1;
                z__5.r = c__ * t[i__6].r;
                z__5.i = c__ * t[i__6].i; // , expr subst
                z__1.r = z__2.r + z__5.r;
                z__1.i = z__2.i + z__5.i; // , expr subst
                t[i__4].r = z__1.r;
                t[i__4].i = z__1.i; // , expr subst
                i__4 = jr + (j + 1) * t_dim1;
                t[i__4].r = ctemp.r;
                t[i__4].i = ctemp.i; // , expr subst
                /* L130: */
            }
            if(ilz)
            {
                i__3 = *n;
                for(jr = 1; jr <= i__3; ++jr)
                {
                    i__4 = jr + (j + 1) * z_dim1;
                    z__2.r = c__ * z__[i__4].r;
                    z__2.i = c__ * z__[i__4].i; // , expr subst
                    i__5 = jr + j * z_dim1;
                    z__3.r = s.r * z__[i__5].r - s.i * z__[i__5].i;
                    z__3.i = s.r * z__[i__5].i + s.i * z__[i__5].r; // , expr subst
                    z__1.r = z__2.r + z__3.r;
                    z__1.i = z__2.i + z__3.i; // , expr subst
                    ctemp.r = z__1.r;
                    ctemp.i = z__1.i; // , expr subst
                    i__4 = jr + j * z_dim1;
                    d_cnjg(&z__4, &s);
                    z__3.r = -z__4.r;
                    z__3.i = -z__4.i; // , expr subst
                    i__5 = jr + (j + 1) * z_dim1;
                    z__2.r = z__3.r * z__[i__5].r - z__3.i * z__[i__5].i;
                    z__2.i = z__3.r * z__[i__5].i + z__3.i * z__[i__5].r; // , expr subst
                    i__6 = jr + j * z_dim1;
                    z__5.r = c__ * z__[i__6].r;
                    z__5.i = c__ * z__[i__6].i; // , expr subst
                    z__1.r = z__2.r + z__5.r;
                    z__1.i = z__2.i + z__5.i; // , expr subst
                    z__[i__4].r = z__1.r;
                    z__[i__4].i = z__1.i; // , expr subst
                    i__4 = jr + (j + 1) * z_dim1;
                    z__[i__4].r = ctemp.r;
                    z__[i__4].i = ctemp.i; // , expr subst
                    /* L140: */
                }
            }
            /* L150: */
        }
#endif /* FLA_ENABLE_AMD_OPT */

    L160: /* L170: */
          ;
    }
    /* Drop-through = non-convergence */
L180:
    *info = ilast;
    goto L210;
    /* Successful completion of all QZ steps */
L190: /* Set Eigenvalues 1:ILO-1 */
#ifdef FLA_ENABLE_AMD_OPT
    if(enable_opt)
    {
        if(ilq)
        {
            apply_grots_q(qidx, qrots_per_swp, qcols, qrots_st_ptr, q, *n, q_dim1);
            qidx = 0;
            qrots_ptr = qrots;
        }
        if(ilz)
        {
            apply_grots_z(zidx, zrots_per_swp, zcols, zrots_st_ptr, z__, *n, z_dim1, sidx,
                          scal_cols, scal_ptr);
            zidx = sidx = 0;
            zrots_ptr = zrots;
        }
        num_swps = 0;
    }
#endif /* FLA_ENABLE_AMD_OPT */
    i__1 = *ilo - 1;
    for(j = 1; j <= i__1; ++j)
    {
        absb = z_abs(&t[j + j * t_dim1]);
        if(absb > safmin)
        {
            i__2 = j + j * t_dim1;
            z__2.r = t[i__2].r / absb;
            z__2.i = t[i__2].i / absb; // , expr subst
            signbc.r = z__2.r;
            signbc.i = -z__2.i; // , expr subst
            i__2 = j + j * t_dim1;
            t[i__2].r = absb;
            t[i__2].i = 0.; // , expr subst
            if(ilschr)
            {
                i__2 = j - 1;
                zscal_(&i__2, &signbc, &t[j * t_dim1 + 1], &c__1);
                zscal_(&j, &signbc, &h__[j * h_dim1 + 1], &c__1);
            }
            else
            {
                zscal_(&c__1, &signbc, &h__[j + j * h_dim1], &c__1);
            }
            if(ilz)
            {
                zscal_(n, &signbc, &z__[j * z_dim1 + 1], &c__1);
            }
        }
        else
        {
            i__2 = j + j * t_dim1;
            t[i__2].r = 0.;
            t[i__2].i = 0.; // , expr subst
        }
        i__2 = j;
        i__3 = j + j * h_dim1;
        alpha[i__2].r = h__[i__3].r;
        alpha[i__2].i = h__[i__3].i; // , expr subst
        i__2 = j;
        i__3 = j + j * t_dim1;
        beta[i__2].r = t[i__3].r;
        beta[i__2].i = t[i__3].i; // , expr subst
        /* L200: */
    }
    /* Normal Termination */
    *info = 0;
    /* Exit (other than argument error) -- return optimal workspace size */
L210:
#ifdef FLA_ENABLE_AMD_OPT
    if(enable_opt && blk_enable)
    {
        free(umem);
    }
#endif /* FLA_ENABLE_AMD_OPT */

    z__1.r = (doublereal)(*n);
    z__1.i = 0.; // , expr subst
    work[1].r = z__1.r;
    work[1].i = z__1.i; // , expr subst
    return;
    /* End of ZHGEQZ */
}
/* zhgeqz_ */

#ifdef FLA_ENABLE_AMD_OPT
void apply_grots_q(integer num_swps, integer *rots_per_swp, integer *rcol, doublereal **rots_sptr,
                   doublecomplex *q, integer n, integer ldq)
{
    integer swp, col, rot;
    integer i, j;
    doublereal c;
    doublecomplex s;
    doublereal *crot;

    for(swp = 0; swp < num_swps; swp++)
    {
        col = rcol[swp];
        crot = rots_sptr[swp];
        for(rot = 0; rot < rots_per_swp[swp]; rot++, col++)
        {
            i = 1 + (col)*ldq;
            j = 1 + (col + 1) * ldq;

            c = *crot++;
            s.r = *crot++;
            s.i = *crot++;

            zrot_(&n, &q[i], &c__1, &q[j], &c__1, &c, &s);
        }
    }
    return;
}

void apply_grots_z(integer num_swps, integer *rots_per_swp, integer *rcol, doublereal **rots_sptr,
                   doublecomplex *z, integer n, integer ldz, integer num_scal, integer *scol,
                   doublecomplex *scalv)
{
    integer swp, col, rot;
    integer i, j;
    doublereal c;
    doublecomplex s;
    doublereal *crot;

    for(swp = 0; swp < num_swps; swp++)
    {
        col = rcol[swp];
        crot = rots_sptr[swp];
        for(rot = 0; rot < rots_per_swp[swp]; rot++, col++)
        {
            i = 1 + (col)*ldz;
            j = 1 + (col + 1) * ldz;

            c = *crot++;
            s.r = *crot++;
            s.i = *crot++;

            zrot_(&n, &z[j], &c__1, &z[i], &c__1, &c, &s);
        }
    }

    for(i = 0; i < num_scal; i++)
    {
        j = 1 + *scol++ * ldz;
        zscal_(&n, &scalv[i], &z[j], &c__1);
    }
    return;
}
#endif /* FLA_ENABLE_AMD_OPT */
