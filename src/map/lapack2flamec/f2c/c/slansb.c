/* ../netlib/slansb.f -- translated by f2c (version 20160102). You must link the resulting object file with libf2c: on Microsoft Windows system, link with libf2c.lib;
 on Linux or Unix systems, link with .../path/to/libf2c.a -lm or, if you install libf2c.a in a standard place, with -lf2c -lm -- in that order, at the end of the command line, as in cc *.o -lf2c -lm Source for libf2c is in /netlib/f2c/libf2c.zip, e.g., http://www.netlib.org/f2c/libf2c.zip */
 #include "FLA_f2c.h" /* Table of constant values */
 static integer c__1 = 1;
 /* > \brief \b SLANSB returns the value of the 1-norm, or the Frobenius norm, or the infinity norm, or the ele ment of largest absolute value of a symmetric band matrix. */
 /* =========== DOCUMENTATION =========== */
 /* Online html documentation available at */
 /* http://www.netlib.org/lapack/explore-html/ */
 /* > \htmlonly */
 /* > Download SLANSB + dependencies */
 /* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/slansb. f"> */
 /* > [TGZ]</a> */
 /* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/slansb. f"> */
 /* > [ZIP]</a> */
 /* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/slansb. f"> */
 /* > [TXT]</a> */
 /* > \endhtmlonly */
 /* Definition: */
 /* =========== */
 /* REAL FUNCTION SLANSB( NORM, UPLO, N, K, AB, LDAB, */
 /* WORK ) */
 /* .. Scalar Arguments .. */
 /* CHARACTER NORM, UPLO */
 /* INTEGER K, LDAB, N */
 /* .. */
 /* .. Array Arguments .. */
 /* REAL AB( LDAB, * ), WORK( * ) */
 /* .. */
 /* > \par Purpose: */
 /* ============= */
 /* > */
 /* > \verbatim */
 /* > */
 /* > SLANSB returns the value of the one norm, or the Frobenius norm, or */
 /* > the infinity norm, or the element of largest absolute value of an */
 /* > n by n symmetric band matrix A, with k super-diagonals. */
 /* > \endverbatim */
 /* > */
 /* > \return SLANSB */
 /* > \verbatim */
 /* > */
 /* > SLANSB = ( max(f2c_abs(A(i,j))), NORM = 'M' or 'm' */
 /* > ( */
 /* > ( norm1(A), NORM = '1', 'O' or 'o' */
 /* > ( */
 /* > ( normI(A), NORM = 'I' or 'i' */
 /* > ( */
 /* > ( normF(A), NORM = 'F', 'f', 'E' or 'e' */
 /* > */
 /* > where norm1 denotes the one norm of a matrix (maximum column sum), */
 /* > normI denotes the infinity norm of a matrix (maximum row sum) and */
 /* > normF denotes the Frobenius norm of a matrix (square root of sum of */
 /* > squares). Note that max(f2c_abs(A(i,j))) is not a consistent matrix norm. */
 /* > \endverbatim */
 /* Arguments: */
 /* ========== */
 /* > \param[in] NORM */
 /* > \verbatim */
 /* > NORM is CHARACTER*1 */
 /* > Specifies the value to be returned in SLANSB as described */
 /* > above. */
 /* > \endverbatim */
 /* > */
 /* > \param[in] UPLO */
 /* > \verbatim */
 /* > UPLO is CHARACTER*1 */
 /* > Specifies whether the upper or lower triangular part of the */
 /* > band matrix A is supplied. */
 /* > = 'U': Upper triangular part is supplied */
 /* > = 'L': Lower triangular part is supplied */
 /* > \endverbatim */
 /* > */
 /* > \param[in] N */
 /* > \verbatim */
 /* > N is INTEGER */
 /* > The order of the matrix A. N >= 0. When N = 0, SLANSB is */
 /* > set to zero. */
 /* > \endverbatim */
 /* > */
 /* > \param[in] K */
 /* > \verbatim */
 /* > K is INTEGER */
 /* > The number of super-diagonals or sub-diagonals of the */
 /* > band matrix A. K >= 0. */
 /* > \endverbatim */
 /* > */
 /* > \param[in] AB */
 /* > \verbatim */
 /* > AB is REAL array, dimension (LDAB,N) */
 /* > The upper or lower triangle of the symmetric band matrix A, */
 /* > stored in the first K+1 rows of AB. The j-th column of A is */
 /* > stored in the j-th column of the array AB as follows: */
 /* > if UPLO = 'U', AB(k+1+i-j,j) = A(i,j) for max(1,j-k)<=i<=j;
 */
 /* > if UPLO = 'L', AB(1+i-j,j) = A(i,j) for j<=i<=min(n,j+k). */
 /* > \endverbatim */
 /* > */
 /* > \param[in] LDAB */
 /* > \verbatim */
 /* > LDAB is INTEGER */
 /* > The leading dimension of the array AB. LDAB >= K+1. */
 /* > \endverbatim */
 /* > */
 /* > \param[out] WORK */
 /* > \verbatim */
 /* > WORK is REAL array, dimension (MAX(1,LWORK)), */
 /* > where LWORK >= N when NORM = 'I' or '1' or 'O';
 otherwise, */
 /* > WORK is not referenced. */
 /* > \endverbatim */
 /* Authors: */
 /* ======== */
 /* > \author Univ. of Tennessee */
 /* > \author Univ. of California Berkeley */
 /* > \author Univ. of Colorado Denver */
 /* > \author NAG Ltd. */
 /* > \date December 2016 */
 /* > \ingroup realOTHERauxiliary */
 /* ===================================================================== */
 real slansb_(char *norm, char *uplo, integer *n, integer *k, real *ab, integer *ldab, real *work) {
 /* System generated locals */
 integer ab_dim1, ab_offset, i__1, i__2, i__3, i__4;
 real ret_val, r__1;
 /* Builtin functions */
 double sqrt(doublereal);
 /* Local variables */
 extern /* Subroutine */
 int scombssq_(real *, real *);
 integer i__, j, l;
 real sum, ssq[2], absa;
 extern logical lsame_(char *, char *);
 real value;
 extern logical sisnan_(real *);
 real colssq[2];
 extern /* Subroutine */
 int slassq_(integer *, real *, integer *, real *, real *);
 /* -- LAPACK auxiliary routine (version 3.7.0) -- */
 /* -- LAPACK is a software package provided by Univ. of Tennessee, -- */
 /* -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
 /* December 2016 */
 /* .. Scalar Arguments .. */
 /* .. */
 /* .. Array Arguments .. */
 /* .. */
 /* ===================================================================== */
 /* .. Parameters .. */
 /* .. */
 /* .. Local Scalars .. */
 /* .. */
 /* .. Local Arrays .. */
 /* .. */
 /* .. External Functions .. */
 /* .. */
 /* .. External Subroutines .. */
 /* .. */
 /* .. Intrinsic Functions .. */
 /* .. */
 /* .. Executable Statements .. */
 /* Parameter adjustments */
 ab_dim1 = *ldab;
 ab_offset = 1 + ab_dim1;
 ab -= ab_offset;
 --work;
 /* Function Body */
 if (*n == 0) {
 value = 0.f;
 }
 else if (lsame_(norm, "M")) {
 /* Find max(f2c_abs(A(i,j))). */
 value = 0.f;
 if (lsame_(uplo, "U")) {
 i__1 = *n;
 for (j = 1;
 j <= i__1;
 ++j) {
 /* Computing MAX */
 i__2 = *k + 2 - j;
 i__3 = *k + 1;
 for (i__ = max(i__2,1);
 i__ <= i__3;
 ++i__) {
 sum = (r__1 = ab[i__ + j * ab_dim1], f2c_abs(r__1));
 if (value < sum || sisnan_(&sum)) {
 value = sum;
 }
 /* L10: */
 }
 /* L20: */
 }
 }
 else {
 i__1 = *n;
 for (j = 1;
 j <= i__1;
 ++j) {
 /* Computing MIN */
 i__2 = *n + 1 - j; i__4 = *k + 1; // , expr subst  
 i__3 = min(i__2,i__4);
 for (i__ = 1;
 i__ <= i__3;
 ++i__) {
 sum = (r__1 = ab[i__ + j * ab_dim1], f2c_abs(r__1));
 if (value < sum || sisnan_(&sum)) {
 value = sum;
 }
 /* L30: */
 }
 /* L40: */
 }
 }
 }
 else if (lsame_(norm, "I") || lsame_(norm, "O") || *(unsigned char *)norm == '1') {
 /* Find normI(A) ( = norm1(A), since A is symmetric). */
 value = 0.f;
 if (lsame_(uplo, "U")) {
 i__1 = *n;
 for (j = 1;
 j <= i__1;
 ++j) {
 sum = 0.f;
 l = *k + 1 - j;
 /* Computing MAX */
 i__3 = 1; i__2 = j - *k; // , expr subst  
 i__4 = j - 1;
 for (i__ = max(i__3,i__2);
 i__ <= i__4;
 ++i__) {
 absa = (r__1 = ab[l + i__ + j * ab_dim1], f2c_abs(r__1));
 sum += absa;
 work[i__] += absa;
 /* L50: */
 }
 work[j] = sum + (r__1 = ab[*k + 1 + j * ab_dim1], f2c_abs(r__1));
 /* L60: */
 }
 i__1 = *n;
 for (i__ = 1;
 i__ <= i__1;
 ++i__) {
 sum = work[i__];
 if (value < sum || sisnan_(&sum)) {
 value = sum;
 }
 /* L70: */
 }
 }
 else {
 i__1 = *n;
 for (i__ = 1;
 i__ <= i__1;
 ++i__) {
 work[i__] = 0.f;
 /* L80: */
 }
 i__1 = *n;
 for (j = 1;
 j <= i__1;
 ++j) {
 sum = work[j] + (r__1 = ab[j * ab_dim1 + 1], f2c_abs(r__1));
 l = 1 - j;
 /* Computing MIN */
 i__3 = *n; i__2 = j + *k; // , expr subst  
 i__4 = min(i__3,i__2);
 for (i__ = j + 1;
 i__ <= i__4;
 ++i__) {
 absa = (r__1 = ab[l + i__ + j * ab_dim1], f2c_abs(r__1));
 sum += absa;
 work[i__] += absa;
 /* L90: */
 }
 if (value < sum || sisnan_(&sum)) {
 value = sum;
 }
 /* L100: */
 }
 }
 }
 else if (lsame_(norm, "F") || lsame_(norm, "E")) {
 /* Find normF(A). */
 /* SSQ(1) is scale */
 /* SSQ(2) is sum-of-squares */
 /* For better accuracy, sum each column separately. */
 ssq[0] = 0.f;
 ssq[1] = 1.f;
 /* Sum off-diagonals */
 if (*k > 0) {
 if (lsame_(uplo, "U")) {
 i__1 = *n;
 for (j = 2;
 j <= i__1;
 ++j) {
 colssq[0] = 0.f;
 colssq[1] = 1.f;
 /* Computing MIN */
 i__3 = j - 1;
 i__4 = min(i__3,*k);
 /* Computing MAX */
 i__2 = *k + 2 - j;
 slassq_(&i__4, &ab[max(i__2,1) + j * ab_dim1], &c__1, colssq, &colssq[1]);
 scombssq_(ssq, colssq);
 /* L110: */
 }
 l = *k + 1;
 }
 else {
 i__1 = *n - 1;
 for (j = 1;
 j <= i__1;
 ++j) {
 colssq[0] = 0.f;
 colssq[1] = 1.f;
 /* Computing MIN */
 i__3 = *n - j;
 i__4 = min(i__3,*k);
 slassq_(&i__4, &ab[j * ab_dim1 + 2], &c__1, colssq, & colssq[1]);
 scombssq_(ssq, colssq);
 /* L120: */
 }
 l = 1;
 }
 ssq[1] *= 2;
 }
 else {
 l = 1;
 }
 /* Sum diagonal */
 colssq[0] = 0.f;
 colssq[1] = 1.f;
 slassq_(n, &ab[l + ab_dim1], ldab, colssq, &colssq[1]);
 scombssq_(ssq, colssq);
 value = ssq[0] * sqrt(ssq[1]);
 }
 ret_val = value;
 return ret_val;
 /* End of SLANSB */
 }
 /* slansb_ */
 