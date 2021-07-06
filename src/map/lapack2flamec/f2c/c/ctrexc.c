/* ../netlib/ctrexc.f -- translated by f2c (version 20160102). You must link the resulting object file with libf2c: on Microsoft Windows system, link with libf2c.lib;
 on Linux or Unix systems, link with .../path/to/libf2c.a -lm or, if you install libf2c.a in a standard place, with -lf2c -lm -- in that order, at the end of the command line, as in cc *.o -lf2c -lm Source for libf2c is in /netlib/f2c/libf2c.zip, e.g., http://www.netlib.org/f2c/libf2c.zip */
 #include "FLA_f2c.h" /* Table of constant values */
 static integer c__1 = 1;
 /* > \brief \b CTREXC */
 /* =========== DOCUMENTATION =========== */
 /* Online html documentation available at */
 /* http://www.netlib.org/lapack/explore-html/ */
 /* > \htmlonly */
 /* > Download CTREXC + dependencies */
 /* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/ctrexc. f"> */
 /* > [TGZ]</a> */
 /* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/ctrexc. f"> */
 /* > [ZIP]</a> */
 /* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/ctrexc. f"> */
 /* > [TXT]</a> */
 /* > \endhtmlonly */
 /* Definition: */
 /* =========== */
 /* SUBROUTINE CTREXC( COMPQ, N, T, LDT, Q, LDQ, IFST, ILST, INFO ) */
 /* .. Scalar Arguments .. */
 /* CHARACTER COMPQ */
 /* INTEGER IFST, ILST, INFO, LDQ, LDT, N */
 /* .. */
 /* .. Array Arguments .. */
 /* COMPLEX Q( LDQ, * ), T( LDT, * ) */
 /* .. */
 /* > \par Purpose: */
 /* ============= */
 /* > */
 /* > \verbatim */
 /* > */
 /* > CTREXC reorders the Schur factorization of a complex matrix */
 /* > A = Q*T*Q**H, so that the diagonal element of T with row index IFST */
 /* > is moved to row ILST. */
 /* > */
 /* > The Schur form T is reordered by a unitary similarity transformation */
 /* > Z**H*T*Z, and optionally the matrix Q of Schur vectors is updated by */
 /* > postmultplying it with Z. */
 /* > \endverbatim */
 /* Arguments: */
 /* ========== */
 /* > \param[in] COMPQ */
 /* > \verbatim */
 /* > COMPQ is CHARACTER*1 */
 /* > = 'V': update the matrix Q of Schur vectors;
 */
 /* > = 'N': do not update Q. */
 /* > \endverbatim */
 /* > */
 /* > \param[in] N */
 /* > \verbatim */
 /* > N is INTEGER */
 /* > The order of the matrix T. N >= 0. */
 /* > If N == 0 arguments ILST and IFST may be any value. */
 /* > \endverbatim */
 /* > */
 /* > \param[in,out] T */
 /* > \verbatim */
 /* > T is COMPLEX array, dimension (LDT,N) */
 /* > On entry, the upper triangular matrix T. */
 /* > On exit, the reordered upper triangular matrix. */
 /* > \endverbatim */
 /* > */
 /* > \param[in] LDT */
 /* > \verbatim */
 /* > LDT is INTEGER */
 /* > The leading dimension of the array T. LDT >= max(1,N). */
 /* > \endverbatim */
 /* > */
 /* > \param[in,out] Q */
 /* > \verbatim */
 /* > Q is COMPLEX array, dimension (LDQ,N) */
 /* > On entry, if COMPQ = 'V', the matrix Q of Schur vectors. */
 /* > On exit, if COMPQ = 'V', Q has been postmultiplied by the */
 /* > unitary transformation matrix Z which reorders T. */
 /* > If COMPQ = 'N', Q is not referenced. */
 /* > \endverbatim */
 /* > */
 /* > \param[in] LDQ */
 /* > \verbatim */
 /* > LDQ is INTEGER */
 /* > The leading dimension of the array Q. LDQ >= 1, and if */
 /* > COMPQ = 'V', LDQ >= max(1,N). */
 /* > \endverbatim */
 /* > */
 /* > \param[in] IFST */
 /* > \verbatim */
 /* > IFST is INTEGER */
 /* > \endverbatim */
 /* > */
 /* > \param[in] ILST */
 /* > \verbatim */
 /* > ILST is INTEGER */
 /* > */
 /* > Specify the reordering of the diagonal elements of T: */
 /* > The element with row index IFST is moved to row ILST by a */
 /* > sequence of transpositions between adjacent elements. */
 /* > 1 <= IFST <= N;
 1 <= ILST <= N. */
 /* > \endverbatim */
 /* > */
 /* > \param[out] INFO */
 /* > \verbatim */
 /* > INFO is INTEGER */
 /* > = 0: successful exit */
 /* > < 0: if INFO = -i, the i-th argument had an illegal value */
 /* > \endverbatim */
 /* Authors: */
 /* ======== */
 /* > \author Univ. of Tennessee */
 /* > \author Univ. of California Berkeley */
 /* > \author Univ. of Colorado Denver */
 /* > \author NAG Ltd. */
 /* > \date December 2016 */
 /* > \ingroup complexOTHERcomputational */
 /* ===================================================================== */
 /* Subroutine */
 int ctrexc_(char *compq, integer *n, complex *t, integer * ldt, complex *q, integer *ldq, integer *ifst, integer *ilst, integer * info) {
 /* System generated locals */
 integer q_dim1, q_offset, t_dim1, t_offset, i__1, i__2, i__3;
 complex q__1;
 /* Builtin functions */
 void r_cnjg(complex *, complex *);
 /* Local variables */
 integer k, m1, m2, m3;
 real cs;
 complex t11, t22, sn, temp;
 extern /* Subroutine */
 int crot_(integer *, complex *, integer *, complex *, integer *, real *, complex *);
 extern logical lsame_(char *, char *);
 logical wantq;
 extern /* Subroutine */
 int clartg_(complex *, complex *, real *, complex *, complex *), xerbla_(char *, integer *);
 /* -- LAPACK computational routine (version 3.7.0) -- */
 /* -- LAPACK is a software package provided by Univ. of Tennessee, -- */
 /* -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
 /* December 2016 */
 /* .. Scalar Arguments .. */
 /* .. */
 /* .. Array Arguments .. */
 /* .. */
 /* ===================================================================== */
 /* .. Local Scalars .. */
 /* .. */
 /* .. External Functions .. */
 /* .. */
 /* .. External Subroutines .. */
 /* .. */
 /* .. Intrinsic Functions .. */
 /* .. */
 /* .. Executable Statements .. */
 /* Decode and test the input parameters. */
 /* Parameter adjustments */
 t_dim1 = *ldt;
 t_offset = 1 + t_dim1;
 t -= t_offset;
 q_dim1 = *ldq;
 q_offset = 1 + q_dim1;
 q -= q_offset;
 /* Function Body */
 *info = 0;
 wantq = lsame_(compq, "V");
 if (! lsame_(compq, "N") && ! wantq) {
 *info = -1;
 }
 else if (*n < 0) {
 *info = -2;
 }
 else if (*ldt < max(1,*n)) {
 *info = -4;
 }
 else if (*ldq < 1 || wantq && *ldq < max(1,*n)) {
 *info = -6;
 }
 else if ((*ifst < 1 || *ifst > *n) && *n > 0) {
 *info = -7;
 }
 else if ((*ilst < 1 || *ilst > *n) && *n > 0) {
 *info = -8;
 }
 if (*info != 0) {
 i__1 = -(*info);
 xerbla_("CTREXC", &i__1);
 return 0;
 }
 /* Quick return if possible */
 if (*n <= 1 || *ifst == *ilst) {
 return 0;
 }
 if (*ifst < *ilst) {
 /* Move the IFST-th diagonal element forward down the diagonal. */
 m1 = 0;
 m2 = -1;
 m3 = 1;
 }
 else {
 /* Move the IFST-th diagonal element backward up the diagonal. */
 m1 = -1;
 m2 = 0;
 m3 = -1;
 }
 i__1 = *ilst + m2;
 i__2 = m3;
 for (k = *ifst + m1;
 i__2 < 0 ? k >= i__1 : k <= i__1;
 k += i__2) {
 /* Interchange the k-th and (k+1)-th diagonal elements. */
 i__3 = k + k * t_dim1;
 t11.r = t[i__3].r; t11.i = t[i__3].i; // , expr subst  
 i__3 = k + 1 + (k + 1) * t_dim1;
 t22.r = t[i__3].r; t22.i = t[i__3].i; // , expr subst  
 /* Determine the transformation to perform the interchange. */
 q__1.r = t22.r - t11.r; q__1.i = t22.i - t11.i; // , expr subst  
 clartg_(&t[k + (k + 1) * t_dim1], &q__1, &cs, &sn, &temp);
 /* Apply transformation to the matrix T. */
 if (k + 2 <= *n) {
 i__3 = *n - k - 1;
 crot_(&i__3, &t[k + (k + 2) * t_dim1], ldt, &t[k + 1 + (k + 2) * t_dim1], ldt, &cs, &sn);
 }
 i__3 = k - 1;
 r_cnjg(&q__1, &sn);
 crot_(&i__3, &t[k * t_dim1 + 1], &c__1, &t[(k + 1) * t_dim1 + 1], & c__1, &cs, &q__1);
 i__3 = k + k * t_dim1;
 t[i__3].r = t22.r; t[i__3].i = t22.i; // , expr subst  
 i__3 = k + 1 + (k + 1) * t_dim1;
 t[i__3].r = t11.r; t[i__3].i = t11.i; // , expr subst  
 if (wantq) {
 /* Accumulate transformation in the matrix Q. */
 r_cnjg(&q__1, &sn);
 crot_(n, &q[k * q_dim1 + 1], &c__1, &q[(k + 1) * q_dim1 + 1], & c__1, &cs, &q__1);
 }
 /* L10: */
 }
 return 0;
 /* End of CTREXC */
 }
 /* ctrexc_ */
 