#include "FLA_f2c.h"
#include "FLA_lapack2flame_return_defs.h"
int cunml2_check(char *side, char *trans, integer *m, integer *n, integer *k, scomplex *a,
                 integer *lda, scomplex *tau, scomplex *c__, integer *ldc, scomplex *work,
                 integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, c_dim1, c_offset, i__1;
    /* Builtin functions */
    /* Local variables */
    integer nq;
    logical left;
    logical notran;

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --tau;
    c_dim1 = *ldc;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    --work;
    /* Function Body */
    *info = 0;
    left = lsame_(side, "L", 1, 1);
    notran = lsame_(trans, "N", 1, 1);
    /* NQ is the order of Q */
    if(left)
    {
        nq = *m;
    }
    else
    {
        nq = *n;
    }
    if(!left && !lsame_(side, "R", 1, 1))
    {
        *info = -1;
    }
    else if(!notran && !lsame_(trans, "C", 1, 1))
    {
        *info = -2;
    }
    else if(*m < 0)
    {
        *info = -3;
    }
    else if(*n < 0)
    {
        *info = -4;
    }
    else if(*k < 0 || *k > nq)
    {
        *info = -5;
    }
    else if(*lda < fla_max(1, *k))
    {
        *info = -7;
    }
    else if(*ldc < fla_max(1, *m))
    {
        *info = -10;
    }
    if(*info != 0)
    {
        i__1 = -(*info);
        xerbla_("CUNML2", &i__1, (ftnlen)6);
        return LAPACK_FAILURE;
    }
    /* Quick return if possible */
    if(*m == 0 || *n == 0 || *k == 0)
    {
        return LAPACK_QUICK_RETURN;
    }

    return LAPACK_SUCCESS;
}
