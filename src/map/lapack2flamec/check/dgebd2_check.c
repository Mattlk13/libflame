#include "FLA_f2c.h"
#include "FLA_lapack2flame_return_defs.h"

int dgebd2_check(integer *m, integer *n, double *a, integer *lda, double *d__, double *e,
                 double *tauq, double *taup, double *work, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --d__;
    --e;
    --tauq;
    --taup;
    --work;
    /* Function Body */
    *info = 0;
    if(*m < 0)
    {
        *info = -1;
    }
    else if(*n < 0)
    {
        *info = -2;
    }
    else if(*lda < fla_max(1, *m))
    {
        *info = -4;
    }
    if(*info < 0)
    {
        i__1 = -(*info);
        xerbla_("DGEBD2", &i__1, (ftnlen)6);
        return LAPACK_FAILURE;
    }
    return LAPACK_SUCCESS;
}
