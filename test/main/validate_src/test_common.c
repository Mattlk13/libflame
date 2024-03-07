/*
    Copyright (C) 2022-2024, Advanced Micro Devices, Inc.  All rights reserved.
*/

#include "test_common.h"

// Global variables
integer i_zero = 0, i_one = 1, i_n_one = -1;
float s_zero = 0, s_one = 1, s_n_one = -1;
double d_zero = 0, d_one = 1, d_n_one = -1;
scomplex c_zero = {0, 0}, c_one = {1, 0}, c_n_one = {-1, 0};
dcomplex z_zero = {0, 0}, z_one = {1, 0}, z_n_one = {-1, 0};

/* Integer absolute function */
integer fla_i_abs(integer *x)
{
    return (*x >= 0 ? (*x) : (-*x));
}
/* Allocate dynamic memory. If FLA_MEM_UNALIGNED is set, unaligned memory is allocated */
char *fla_mem_alloc(size_t size)
{
    char *buff = NULL;
#ifdef FLA_MEM_UNALIGNED
    buff = (char *)malloc(size + 1);
    if(buff == NULL)
    {
        fprintf(stderr, "malloc() returned NULL pointer\n");
        abort();
    }
    /* making aligned address to byte aligned */
    buff = buff + 1;
#else
    buff = (char *)malloc(size);
    if(buff == NULL)
    {
        fprintf(stderr, "malloc() returned NULL pointer\n");
        abort();
    }
#endif
    return buff;
}
/* create vector of given datatype*/
void create_vector(integer datatype, void **A, integer M)
{
    *A = NULL;

    switch(datatype)
    {
    case INTEGER:
    {
        *A = (integer *)fla_mem_alloc(fla_max(1, M) * sizeof(integer));
        break;
    }

    case FLOAT:
    {
        *A = (float *)fla_mem_alloc(fla_max(1, M) * sizeof(float));
        break;
    }

    case DOUBLE:
    {
        *A = (double *)fla_mem_alloc(fla_max(1, M) * sizeof(double));
        break;
    }

    case COMPLEX:
    {
        *A = (scomplex *)fla_mem_alloc(fla_max(1, M) * sizeof(scomplex));
        break;
    }

    case DOUBLE_COMPLEX:
    {
        *A = (dcomplex *)fla_mem_alloc(fla_max(1, M) * sizeof(dcomplex));
        break;
    }
    }

    return;
}

void create_realtype_vector(integer datatype, void **A, integer M)
{
    *A = NULL;

    if(datatype == FLOAT || datatype == COMPLEX)
        *A = (float *)fla_mem_alloc(fla_max(1, M) * sizeof(float));
    else
        *A = (double *)fla_mem_alloc(fla_max(1, M) * sizeof(double));

    return;
}

/* free vector */
void free_vector(void *A)
{
    if(!A)
        return;
#ifdef FLA_MEM_UNALIGNED
    /* reset the incremented address to normal to proper freeing of memory */
    char *temp = (char *)A;
    A = (void *)(temp - 1);
#endif
    free(A);
}

/* initialize to zero */
void reset_vector(integer datatype, void *A, integer M, integer incA)
{
    integer i;

    switch(datatype)
    {
    case INTEGER:
    {
        for(i = 0; i < M; i++)
        {
            ((integer *)A)[i * incA] = 0;
        }
        break;
    }
    case FLOAT:
    {
        for(i = 0; i < M; i++)
        {
            ((float *)A)[i * incA] = 0.f;
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < M; i++)
        {
            ((double *)A)[i * incA] = 0.;
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < M; i++)
        {
            ((scomplex *)A)[i * incA].real = 0.f;
            ((scomplex *)A)[i * incA].imag = 0.f;
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < M; i++)
        {
            ((dcomplex *)A)[i * incA].real = 0.;
            ((dcomplex *)A)[i * incA].imag = 0.;
        }
        break;
    }
    }

    return;
}

/* Initialize vector with random values */
void rand_vector(integer datatype, void *A, integer M, integer LDA)
{
    integer i;

    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 0; i < M; i++)
        {
            ((float *)A)[i * LDA] = SRAND();
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < M; i++)
        {
            ((double *)A)[i * LDA] = DRAND();
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < M; i++)
        {
            ((scomplex *)A)[i * LDA].real = SRAND();
            ((scomplex *)A)[i * LDA].imag = SRAND();
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < M; i++)
        {
            ((dcomplex *)A)[i * LDA].real = DRAND();
            ((dcomplex *)A)[i * LDA].imag = DRAND();
        }
        break;
    }
    }

    return;
}

/* Copy a vector */
void copy_vector(integer datatype, integer M, void *A, integer LDA, void *B, integer LDB)
{
    switch(datatype)
    {
    case INTEGER:
    {
        integer i;

        for(i = 0; i < M; i++)
        {
            ((integer *)B)[i * LDB] = ((integer *)A)[i * LDA];
        }
        break;
    }
    case FLOAT:
    {
        scopy_(&M, A, &LDA, B, &LDB);
        break;
    }
    case DOUBLE:
    {
        dcopy_(&M, A, &LDA, B, &LDB);
        break;
    }
    case COMPLEX:
    {
        ccopy_(&M, A, &LDA, B, &LDB);
        break;
    }
    case DOUBLE_COMPLEX:
    {
        zcopy_(&M, A, &LDA, B, &LDB);
        break;
    }
    }

    return;
}

void copy_realtype_vector(integer datatype, integer M, void *A, integer LDA, void *B, integer LDB)
{
    if(datatype == FLOAT || datatype == COMPLEX)
        scopy_(&M, A, &LDA, B, &LDB);
    else
        dcopy_(&M, A, &LDA, B, &LDB);

    return;
}

/* create matrix of given datatype*/
void create_matrix(integer datatype, void **A, integer M, integer N)
{
    *A = NULL;

    switch(datatype)
    {
    case INTEGER:
    {
        *A = (integer *)fla_mem_alloc(fla_max(1, M) * fla_max(1, N) * sizeof(integer));
        break;
    }

    case FLOAT:
    {
        *A = (float *)fla_mem_alloc(fla_max(1, M) * fla_max(1, N) * sizeof(float));
        break;
    }

    case DOUBLE:
    {
        *A = (double *)fla_mem_alloc(fla_max(1, M) * fla_max(1, N) * sizeof(double));
        break;
    }

    case COMPLEX:
    {
        *A = (scomplex *)fla_mem_alloc(fla_max(1, M) * fla_max(1, N) * sizeof(scomplex));
        break;
    }

    case DOUBLE_COMPLEX:
    {
        *A = (dcomplex *)fla_mem_alloc(fla_max(1, M) * fla_max(1, N) * sizeof(dcomplex));
        break;
    }
    }

    return;
}

void create_realtype_matrix(integer datatype, void **A, integer M, integer N)
{
    *A = NULL;

    if(datatype == FLOAT || datatype == COMPLEX)
        *A = (float *)fla_mem_alloc(fla_max(1, M) * fla_max(1, N) * sizeof(float));
    else
        *A = (double *)fla_mem_alloc(fla_max(1, M) * fla_max(1, N) * sizeof(double));

    return;
}

void *get_m_ptr(integer datatype, void *A, integer M, integer N, integer LDA)
{
    void *mat = NULL;

    switch(datatype)
    {
    case FLOAT:
    {
        mat = ((float *)A) + M + N * LDA;
        break;
    }
    case DOUBLE:
    {
        mat = ((double *)A) + M + N * LDA;
        break;
    }
    case COMPLEX:
    {
        mat = ((scomplex *)A) + M + N * LDA;
        break;
    }
    case DOUBLE_COMPLEX:
    {
        mat = ((dcomplex *)A) + M + N * LDA;
        break;
    }
    }

    return mat;
}

/* free matrix */
void free_matrix(void *A)
{
    if(!A)
        return;
#ifdef FLA_MEM_UNALIGNED
    /* reset the incremented address to normal to proper freeing of memory */
    char *temp = (char *)A;
    A = (void *)(temp - 1);
#endif
    free(A);
}

/* Initialize matrix with random values */
void rand_matrix(integer datatype, void *A, integer M, integer N, integer LDA)
{
    integer i, j;
    if(LDA < M)
        return;
    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((float *)A)[i * LDA + j] = SRAND();
            }
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((double *)A)[i * LDA + j] = DRAND();
            }
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((scomplex *)A)[i * LDA + j].real = SRAND();
                ((scomplex *)A)[i * LDA + j].imag = SRAND();
            }
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((dcomplex *)A)[i * LDA + j].real = DRAND();
                ((dcomplex *)A)[i * LDA + j].imag = DRAND();
            }
        }
        break;
    }
    }

    return;
}

/* Initialize symmetric matrix with random values */
void rand_sym_matrix(integer datatype, void *A, integer M, integer N, integer LDA)
{
    integer i, j;
    if(LDA < M)
        return;
    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j < M; j++)
            {
                ((float *)A)[i * LDA + j] = SRAND();
                ((float *)A)[j * LDA + i] = ((float *)A)[i * LDA + j];
            }
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j < M; j++)
            {
                ((double *)A)[i * LDA + j] = DRAND();
                ((double *)A)[j * LDA + i] = ((double *)A)[i * LDA + j];
            }
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j < M; j++)
            {
                ((scomplex *)A)[i * LDA + j].real = SRAND();
                ((scomplex *)A)[i * LDA + j].imag = SRAND();
                ((scomplex *)A)[j * LDA + i].real = ((scomplex *)A)[i * LDA + j].real;
                ((scomplex *)A)[j * LDA + i].imag = ((scomplex *)A)[i * LDA + j].imag;
            }
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j < M; j++)
            {
                ((dcomplex *)A)[i * LDA + j].real = DRAND();
                ((dcomplex *)A)[i * LDA + j].imag = DRAND();
                ((dcomplex *)A)[j * LDA + i].real = ((dcomplex *)A)[i * LDA + j].real;
                ((dcomplex *)A)[j * LDA + i].imag = ((dcomplex *)A)[i * LDA + j].imag;
            }
        }
        break;
    }
    }

    return;
}

/* Copy a matrix */
void copy_matrix(
    integer datatype, char *uplo, integer M, integer N, void *A, integer LDA, void *B, integer LDB)
{
    if((LDA < M) || (LDB < M))
        return;

    switch(datatype)
    {
    case INTEGER:
    {
        integer i, j;

        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((integer *)B)[i * LDB + j] = ((integer *)A)[i * LDA + j];
            }
        }
        break;
    }
    case FLOAT:
    {
        fla_lapack_slacpy(uplo, &M, &N, A, &LDA, B, &LDB);
        break;
    }
    case DOUBLE:
    {
        fla_lapack_dlacpy(uplo, &M, &N, A, &LDA, B, &LDB);
        break;
    }
    case COMPLEX:
    {
        fla_lapack_clacpy(uplo, &M, &N, A, &LDA, B, &LDB);
        break;
    }
    case DOUBLE_COMPLEX:
    {
        fla_lapack_zlacpy(uplo, &M, &N, A, &LDA, B, &LDB);
        break;
    }
    }

    return;
}

void copy_realtype_matrix(
    integer datatype, char *uplo, integer M, integer N, void *A, integer LDA, void *B, integer LDB)
{
    if(datatype == FLOAT || datatype == COMPLEX)
        fla_lapack_slacpy(uplo, &M, &N, A, &LDA, B, &LDB);
    else
        fla_lapack_dlacpy(uplo, &M, &N, A, &LDA, B, &LDB);

    return;
}

/* Initialize a matrix with zeros */
void reset_matrix(integer datatype, integer M, integer N, void *A, integer LDA)
{
    integer i, j;
    if(LDA < M)
        return;
    switch(datatype)
    {
    case INTEGER:
    {
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((integer *)A)[i * LDA + j] = 0;
            }
        }
        break;
    }

    case FLOAT:
    {
        fla_lapack_slaset("A", &M, &N, &s_zero, &s_zero, A, &LDA);
        break;
    }

    case DOUBLE:
    {
        fla_lapack_dlaset("A", &M, &N, &d_zero, &d_zero, A, &LDA);
        break;
    }

    case COMPLEX:
    {
        fla_lapack_claset("A", &M, &N, &c_zero, &c_zero, A, &LDA);
        break;
    }

    case DOUBLE_COMPLEX:
    {
        fla_lapack_zlaset("A", &M, &N, &z_zero, &z_zero, A, &LDA);
        break;
    }
    }

    return;
}

/* Set a matrix to identity */
void set_identity_matrix(integer datatype, integer M, integer N, void *A, integer LDA)
{
    if(LDA < M)
        return;

    switch(datatype)
    {
    case FLOAT:
    {
        fla_lapack_slaset("A", &M, &N, &s_zero, &s_one, A, &LDA);
        break;
    }

    case DOUBLE:
    {
        fla_lapack_dlaset("A", &M, &N, &d_zero, &d_one, A, &LDA);
        break;
    }

    case COMPLEX:
    {
        fla_lapack_claset("A", &M, &N, &c_zero, &c_one, A, &LDA);
        break;
    }

    case DOUBLE_COMPLEX:
    {
        fla_lapack_zlaset("A", &M, &N, &z_zero, &z_one, A, &LDA);
        break;
    }
    }

    return;
}

void z_div_t(dcomplex *cp, dcomplex *ap, dcomplex *bp)
{
    dcomplex a = *ap;
    dcomplex b = *bp;
    double temp;

    temp = b.real * b.real + b.imag * b.imag;
    if(!temp)
    {
        fprintf(stderr, "z_div_t : temp is zero. Abort\n");
        abort();
    }

    cp->real = (a.real * b.real + a.imag * b.imag) / temp;
    cp->imag = (a.imag * b.real - a.real * b.imag) / temp;
}

/* Division of complex types */
void c_div_t(scomplex *cp, scomplex *ap, scomplex *bp)
{
    scomplex a = *ap;
    scomplex b = *bp;
    float temp;

    temp = b.real * b.real + b.imag * b.imag;
    if(!temp)
    {
        fprintf(stderr, "z_div_t : temp is zero. Abort\n");
        abort();
    }

    cp->real = (a.real * b.real + a.imag * b.imag) / temp;
    cp->imag = (a.imag * b.real - a.real * b.imag) / temp;
}

/* work value calculation */
integer get_work_value(integer datatype, void *work)
{
    integer value;

    if(!work)
        return 0;

    switch(datatype)
    {
    case INTEGER:
    {
        value = (*(integer *)work);
        break;
    }
    case FLOAT:
    {
        value = (integer)(*(float *)work);
        break;
    }
    case DOUBLE:
    {
        value = (integer)(*(double *)work);
        break;
    }
    case COMPLEX:
    {
        value = (integer)(((scomplex *)work)->real);
        break;
    }
    case DOUBLE_COMPLEX:
    {
        value = (integer)(((dcomplex *)work)->real);
        break;
    }
    default:
    {
        value = 0;
        break;
    }
    }
    return value;
}

void diagmv(integer datatype,
            integer m,
            integer n,
            void *x,
            integer incx,
            void *a,
            integer a_rs,
            integer a_cs)
{
    integer inca, lda;
    integer n_iter;
    integer n_elem;
    integer j;

    if(m <= 0 || n <= 0)
        return;

    // Initialize with optimal values for column-major storage.
    inca = a_rs;
    lda = a_cs;
    n_iter = n;
    n_elem = m;

    switch(datatype)
    {
    case FLOAT:
    {
        float *a_begin;
        for(j = 0; j < n_iter; j++)
        {
            a_begin = (float *)a + j * lda;
            scalv(datatype, n_elem, x, incx, a_begin, inca);
        }
        break;
    }

    case DOUBLE:
    {
        double *a_begin;
        for(j = 0; j < n_iter; j++)
        {
            a_begin = (double *)a + j * lda;
            scalv(datatype, n_elem, x, incx, a_begin, inca);
        }
        break;
    }

    case COMPLEX:
    {
        scomplex *a_begin;
        for(j = 0; j < n_iter; j++)
        {
            a_begin = (scomplex *)a + j * lda;
            scalv(datatype, n_elem, x, incx, a_begin, inca);
        }
        break;
    }

    case DOUBLE_COMPLEX:
    {
        dcomplex *a_begin;
        for(j = 0; j < n_iter; j++)
        {
            a_begin = (dcomplex *)a + j * lda;
            scalv(datatype, n_elem, x, incx, a_begin, inca);
        }
        break;
    }
    }
}

void scalv(integer datatype, integer n, void *x, integer incx, void *y, integer incy)
{
    integer i;

    switch(datatype)
    {
    case FLOAT:
    {
        float *chi, *psi;
        for(i = 0; i < n; ++i)
        {
            chi = (float *)x + i * incx;
            psi = (float *)y + i * incy;

            (*psi) = (*chi) * (*psi);
        }
        break;
    }

    case DOUBLE:
    {
        double *chi, *psi;
        for(i = 0; i < n; ++i)
        {
            chi = (double *)x + i * incx;
            psi = (double *)y + i * incy;

            (*psi) = (*chi) * (*psi);
        }
        break;
    }

    case COMPLEX:
    {
        float *chi;
        scomplex *psi;

        for(i = 0; i < n; ++i)
        {
            chi = (float *)x + i * incx;
            psi = (scomplex *)y + i * incy;

            psi->real = (*chi) * (psi)->real;
            psi->imag = (*chi) * (psi)->imag;
        }
        break;
    }

    case DOUBLE_COMPLEX:
    {
        double *chi;
        dcomplex *psi;
        for(i = 0; i < n; ++i)
        {
            chi = (double *)x + i * incx;
            psi = (dcomplex *)y + i * incy;

            psi->real = (*chi) * (psi)->real;
            psi->imag = (*chi) * (psi)->imag;
        }
        break;
    }
    }
}

void set_transpose(integer datatype, char *uplo, char *trans_A, char *trans_B)
{
    if(*uplo == 'L')
    {
        *trans_A = 'N';
        *trans_B = 'C';
    }
    else
    {
        *trans_A = 'C';
        *trans_B = 'N';
    }
}

void rand_spd_matrix(integer datatype, char *uplo, void **A, integer m, integer lda)
{
    void *sample = NULL;
    void *buff_A = NULL, *buff_B = NULL;
    void *a_temp = NULL;
    char trans_A, trans_B;
    if(lda < m)
        return;

    create_matrix(datatype, &sample, lda, m);
    create_matrix(datatype, &buff_A, lda, m);
    create_matrix(datatype, &buff_B, lda, m);

    reset_matrix(datatype, m, m, buff_A, lda);
    reset_matrix(datatype, m, m, buff_B, lda);

    create_matrix(datatype, &a_temp, lda, m);
    set_identity_matrix(datatype, m, m, a_temp, lda);

    /* Generate random symmetric matrix */
    rand_sym_matrix(datatype, sample, m, m, lda);

    /* Based on uplo set the transpose flag */
    set_transpose(datatype, uplo, &trans_A, &trans_B);

    copy_matrix(datatype, uplo, m, m, sample, lda, buff_A, lda);
    copy_matrix(datatype, uplo, m, m, sample, lda, buff_B, lda);

    switch(datatype)
    {
    case FLOAT:
    {
        float beta = m;
        sgemm_(&trans_A,
               &trans_B,
               &m,
               &m,
               &m,
               &s_one,
               buff_A,
               &lda,
               buff_B,
               &lda,
               &beta,
               a_temp,
               &lda);
        break;
    }
    case DOUBLE:
    {
        double beta = m;
        dgemm_(&trans_A,
               &trans_B,
               &m,
               &m,
               &m,
               &d_one,
               buff_A,
               &lda,
               buff_B,
               &lda,
               &beta,
               a_temp,
               &lda);
        break;
    }
    case COMPLEX:
    {
        scomplex beta = {m, 0};
        cgemm_(&trans_A,
               &trans_B,
               &m,
               &m,
               &m,
               &c_one,
               buff_A,
               &lda,
               buff_B,
               &lda,
               &beta,
               a_temp,
               &lda);
        break;
    }
    case DOUBLE_COMPLEX:
    {
        dcomplex beta = {m, 0};
        zgemm_(&trans_A,
               &trans_B,
               &m,
               &m,
               &m,
               &z_one,
               buff_A,
               &lda,
               buff_B,
               &lda,
               &beta,
               a_temp,
               &lda);
        break;
    }
    }
    copy_matrix(datatype, "full", m, m, a_temp, lda, *A, lda);

    /* free buffers */
    free_matrix(sample);
    free_matrix(buff_A);
    free_matrix(buff_B);
    free_matrix(a_temp);

    return;
}

/* Create diagonal matrix by copying elements from vector to matrix */
void diagonalize_vector(integer datatype, void *s, void *sigma, integer m, integer n, integer LDA)
{
    integer incr, i, j, min_m_n;

    incr = m + 1;
    min_m_n = fla_min(m, n);

    reset_matrix(datatype, m, n, sigma, m);

    switch(datatype)
    {
    case FLOAT:
    {
        scopy_(&min_m_n, s, &i_one, sigma, &incr);
        break;
    }
    case DOUBLE:
    {
        dcopy_(&min_m_n, s, &i_one, sigma, &incr);
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < n; i++)
        {
            for(j = i; j < m; j++)
            {
                if(i == j)
                    ((scomplex *)sigma)[i * LDA + j].real = ((float *)s)[i];
            }
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < n; i++)
        {
            for(j = i; j < m; j++)
            {
                if(i == j)
                    ((dcomplex *)sigma)[i * LDA + j].real = ((double *)s)[i];
            }
        }
        break;
    }
    }
    return;
}

/* Generate random Hermitian matrix */
void rand_hermitian_matrix(integer datatype, integer n, void **A, integer lda)
{
    void *B = NULL;
    if(lda < n)
        return;

    create_matrix(datatype, &B, n, n);
    reset_matrix(datatype, n, n, B, n);
    rand_matrix(datatype, B, n, n, n);

    switch(datatype)
    {
    case COMPLEX:
    {
        cgemm_("N", "C", &n, &n, &n, &c_one, B, &n, B, &n, &c_zero, *A, &lda);
        break;
    }
    case DOUBLE_COMPLEX:
    {
        zgemm_("N", "C", &n, &n, &n, &z_one, B, &n, B, &n, &z_zero, *A, &lda);
        break;
    }
    }
    free_matrix(B);
    return;
}
/* block diagonal matrix is required for computing eigen decomposition of non symmetric matrix.
   W is a block diagonal matrix, with a 1x1 block for each
   real eigenvalue and a 2x2 block for each complex conjugate
   pair.then the 2 x 2 block corresponding to the pair will be:

              (  wr  wi  )
              ( -wi  wr  )
*/
void create_block_diagonal_matrix(
    integer datatype, void *wr, void *wi, void *lambda, integer m, integer n, integer lda)
{
    integer i, j;

    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 0; i < m; i++)
        {
            j = i;
            if(((float *)wi)[i] != 0.f)
            {
                ((float *)lambda)[i * lda + j] = ((float *)wr)[i];
                ((float *)lambda)[i * lda + (j + 1)] = -((float *)wi)[i];
                i++;
                j++;
                ((float *)lambda)[i * lda + j] = ((float *)wr)[i];
                ((float *)lambda)[i * lda + (j - 1)] = -((float *)wi)[i];
            }
            else
            {
                ((float *)lambda)[i * lda + j] = ((float *)wr)[i];
            }
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < m; i++)
        {
            j = i;
            if(((double *)wi)[i] != 0.)
            {
                ((double *)lambda)[i * lda + j] = ((double *)wr)[i];
                ((double *)lambda)[i * lda + (j + 1)] = -((double *)wi)[i];
                i++;
                j++;
                ((double *)lambda)[i * lda + j] = ((double *)wr)[i];
                ((double *)lambda)[i * lda + (j - 1)] = -((double *)wi)[i];
            }
            else
            {
                ((double *)lambda)[i * lda + j] = ((double *)wr)[i];
            }
        }
        break;
    }
    }
}

/* Checks whether A**T * A == I */
double check_orthogonality(integer datatype, void *A, integer m, integer n, integer lda)
{
    void *a_temp = NULL, *work = NULL;
    double resid = 0.;
    integer k;

    /* Create Identity matrix to validate orthogonal property of matrix A*/
    if(m <= n)
    {
        create_matrix(datatype, &a_temp, m, m);
        k = m;
    }
    else
    {
        create_matrix(datatype, &a_temp, n, n);
        k = n;
    }
    switch(datatype)
    {
    case FLOAT:
    {
        float eps, norm;
        eps = fla_lapack_slamch("P");

        fla_lapack_slaset("full", &k, &k, &s_zero, &s_one, a_temp, &k);
        sgemm_("T", "N", &k, &k, &m, &s_one, A, &lda, A, &lda, &s_n_one, a_temp, &k);
        norm = fla_lapack_slange("1", &k, &k, a_temp, &k, work);
        resid = (double)(norm / (eps * (float)k));
        break;
    }
    case DOUBLE:
    {
        double eps, norm;
        eps = fla_lapack_dlamch("P");

        fla_lapack_dlaset("full", &k, &k, &d_zero, &d_one, a_temp, &k);
        dgemm_("T", "N", &k, &k, &m, &d_one, A, &lda, A, &lda, &d_n_one, a_temp, &k);
        norm = fla_lapack_dlange("1", &k, &k, a_temp, &k, work);
        resid = (double)(norm / (eps * (float)k));
        break;
    }
    case COMPLEX:
    {
        float eps, norm;
        eps = fla_lapack_slamch("P");

        fla_lapack_claset("full", &k, &k, &c_zero, &c_one, a_temp, &k);
        cgemm_("C", "N", &k, &k, &m, &c_one, A, &lda, A, &lda, &c_n_one, a_temp, &k);
        norm = fla_lapack_clange("1", &k, &k, a_temp, &k, work);
        resid = (double)(norm / (eps * (float)k));
        break;
    }
    case DOUBLE_COMPLEX:
    {
        double eps, norm;
        eps = fla_lapack_dlamch("P");

        fla_lapack_zlaset("full", &k, &k, &z_zero, &z_one, a_temp, &k);
        zgemm_("C", "N", &k, &k, &m, &z_one, A, &lda, A, &lda, &z_n_one, a_temp, &k);
        norm = fla_lapack_zlange("1", &k, &k, a_temp, &k, work);
        resid = (double)(norm / (eps * (float)k));
        break;
    }
    }
    free_matrix(a_temp);
    return resid;
}

/* copy submatrix from a matrix
 * (m, n) - dimensions of the sub-matrix to be copied
 * A - original matirx
 * B - destination matrix
 * (srow, scol) - start location of the original matrix from where the value has to be copied */

void copy_submatrix(integer datatype,
                    integer m,
                    integer n,
                    void *A,
                    integer lda,
                    void *B,
                    integer ldb,
                    integer srow,
                    integer scol)
{
    integer i, j;

    switch(datatype)
    {
    case FLOAT:
    {
        float *float_A, *float_B;
        for(i = scol, j = 0; j < n; i++, j++)
        {
            float_A = ((float *)A + (i * lda + srow));
            float_B = ((float *)B + (j * ldb));
            copy_vector(datatype, m, float_A, 1, float_B, 1);
        }
        break;
    }
    case DOUBLE:
    {
        double *double_A, *double_B;
        for(i = scol, j = 0; j < n; i++, j++)
        {
            double_A = ((double *)A + (i * lda + srow));
            double_B = ((double *)B + (j * ldb));
            copy_vector(datatype, m, double_A, 1, double_B, 1);
        }
        break;
    }
    case COMPLEX:
    {
        scomplex *scomplex_A, *scomplex_B;
        for(i = scol, j = 0; j < n; i++, j++)
        {
            scomplex_A = ((scomplex *)A + (i * lda + srow));
            scomplex_B = ((scomplex *)B + (j * ldb));
            copy_vector(datatype, m, scomplex_A, 1, scomplex_B, 1);
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        dcomplex *dcomplex_A, *dcomplex_B;
        for(i = scol, j = 0; j < n; i++, j++)
        {
            dcomplex_A = ((dcomplex *)A + (i * lda + srow));
            dcomplex_B = ((dcomplex *)B + (j * ldb));
            copy_vector(datatype, m, dcomplex_A, 1, dcomplex_B, 1);
        }
        break;
    }
    }
}

void scgemv(char TRANS,
            integer real_alpha,
            integer m,
            integer n,
            scomplex *alpha,
            float *a,
            integer lda,
            scomplex *v,
            integer incv,
            float beta,
            scomplex *c,
            integer inc)
{
    integer i, j;
    float real, imag;
    float rl, ig;
    float alphar;
    void *A = NULL;

    create_matrix(FLOAT, &A, lda, n);

    if(TRANS == 'T')
    {
        /* Transpose of a matrix A */
        for(i = 0; i < n; i++)
        {
            for(j = 0; j < n; j++)
            {
                ((float *)A)[i * lda + j] = a[i + j * lda];
            }
        }
    }
    else
    {
        copy_matrix(FLOAT, "full", n, n, a, lda, A, lda);
    }

    if(real_alpha)
    {
        alphar = alpha->real;
        for(i = 0; i < m; i++)
        {
            real = 0;
            imag = 0;
            for(j = 0; j < n; j++)
            {
                real = real + ((float *)A)[i + j * lda] * v[j * incv].real;
                imag = imag + ((float *)A)[i + j * lda] * v[j * incv].imag;
            }
            c[i * inc].real = alphar * real + beta * c[i * inc].real;
            c[i * inc].imag = alphar * imag + beta * c[i * inc].imag;
        }
    }
    else
    {
        for(i = 0; i < m; i++)
        {
            real = 0;
            imag = 0;
            for(j = 0; j < n; j++)
            {
                real = real + ((float *)A)[i + j * lda] * v[j * incv].real;
                imag = imag + ((float *)A)[i + j * lda] * v[j * incv].imag;
            }

            rl = alpha->real * real - alpha->imag * imag;
            ig = alpha->real * imag + alpha->imag * real;

            c[i * inc].real = rl + beta * c[i * inc].real;
            c[i * inc].imag = ig + beta * c[i * inc].imag;
        }
    }

    free_matrix(A);
}

/* Get datatype from string. */
integer get_datatype(char stype)
{
    integer datatype;
    if(stype == 's' || stype == 'S')
        datatype = FLOAT;
    else if(stype == 'd' || stype == 'D')
        datatype = DOUBLE;
    else if(stype == 'c' || stype == 'C')
        datatype = COMPLEX;
    else if(stype == 'z' || stype == 'Z')
        datatype = DOUBLE_COMPLEX;
    else
        datatype = INVALID_TYPE;

    return datatype;
}

/* Get realtype of given datatype. */
integer get_realtype(integer datatype)
{
    if(datatype == FLOAT || datatype == COMPLEX)
    {
        return FLOAT;
    }
    else if(datatype == DOUBLE || datatype == DOUBLE_COMPLEX)
    {
        return DOUBLE;
    }
    else
    {
        fprintf(stderr, "Invalid datatype is passed.\n");
        return -1;
    }
}

/* Initialize symmetric tridiagonal matrix with random values.
   Initializes random values only for diagonal and off diagonal elements.*/
void rand_sym_tridiag_matrix(integer datatype, void *A, integer M, integer N, integer LDA)
{
    integer i, j;
    if(LDA < M)
        return;
    reset_matrix(datatype, M, N, A, LDA);

    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j <= i + 1; j++)
            {
                if(j < N)
                {
                    ((float *)A)[i * LDA + j] = SRAND();
                    ((float *)A)[j * LDA + i] = ((float *)A)[i * LDA + j];
                }
            }
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j <= i + 1; j++)
            {
                if(j < N)
                {
                    ((double *)A)[i * LDA + j] = DRAND();
                    ((double *)A)[j * LDA + i] = ((double *)A)[i * LDA + j];
                }
            }
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j <= i + 1; j++)
            {
                if(j < N)
                {
                    ((scomplex *)A)[i * LDA + j].real = SRAND();
                    ((scomplex *)A)[i * LDA + j].imag = SRAND();
                    ((scomplex *)A)[j * LDA + i].real = ((scomplex *)A)[i * LDA + j].real;
                    ((scomplex *)A)[j * LDA + i].imag = ((scomplex *)A)[i * LDA + j].imag;
                }
            }
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j <= i + 1; j++)
            {
                if(j < N)
                {
                    ((dcomplex *)A)[i * LDA + j].real = DRAND();
                    ((dcomplex *)A)[i * LDA + j].imag = DRAND();
                    ((dcomplex *)A)[j * LDA + i].real = ((dcomplex *)A)[i * LDA + j].real;
                    ((dcomplex *)A)[j * LDA + i].imag = ((dcomplex *)A)[i * LDA + j].imag;
                }
            }
        }
        break;
    }
    }
}

/* Get diagonal elements of matrix A into Diag vector. */
void get_diagonal(integer datatype, void *A, integer m, integer n, integer lda, void *Diag)
{
    integer i, j;
    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 0, j = 0; i < m; i++, j++)
        {
            ((float *)Diag)[i] = ((float *)A)[i * lda + j];
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0, j = 0; i < m; i++, j++)
        {
            ((double *)Diag)[i] = ((double *)A)[i * lda + j];
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0, j = 0; i < m; i++, j++)
        {
            ((scomplex *)Diag)[i].real = ((scomplex *)A)[i * lda + j].real;
            ((scomplex *)Diag)[i].imag = ((scomplex *)A)[i * lda + j].imag;
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0, j = 0; i < m; i++, j++)
        {
            ((dcomplex *)Diag)[i].real = ((dcomplex *)A)[i * lda + j].real;
            ((dcomplex *)Diag)[i].imag = ((dcomplex *)A)[i * lda + j].imag;
        }
        break;
    }
    }
}

/* Get subdiagonal elements of matrix A into Subdiag vector.*/
void get_subdiagonal(integer datatype, void *A, integer m, integer n, integer lda, void *Subdiag)
{
    integer i, j;
    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 1, j = 0; i < m; i++, j++)
        {
            ((float *)Subdiag)[j] = ((float *)A)[i * lda + j];
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 1, j = 0; i < m; i++, j++)
        {
            ((double *)Subdiag)[j] = ((double *)A)[i * lda + j];
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 1, j = 0; i < m; i++, j++)
        {
            ((scomplex *)Subdiag)[j].real = ((scomplex *)A)[i * lda + j].real;
            ((scomplex *)Subdiag)[j].imag = ((scomplex *)A)[i * lda + j].imag;
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 1, j = 0; i < m; i++, j++)
        {
            ((dcomplex *)Subdiag)[j].real = ((dcomplex *)A)[i * lda + j].real;
            ((dcomplex *)Subdiag)[j].imag = ((dcomplex *)A)[i * lda + j].imag;
        }
        break;
    }
    }
}
void copy_sym_tridiag_matrix(
    integer datatype, void *D, void *E, integer M, integer N, void *B, integer LDA)
{
    integer i, j;
    if(LDA < M)
        return;
    reset_matrix(datatype, M, N, B, LDA);

    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j <= i + 1 && j < N; j++)
            {
                if(j == i)
                {
                    ((float *)B)[i * LDA + j] = ((float *)D)[i];
                }
                else
                {
                    ((float *)B)[i * LDA + j] = ((float *)E)[i];
                    ((float *)B)[j * LDA + i] = ((float *)E)[i];
                }
            }
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j <= i + 1 && j < N; j++)
            {
                if(j == i)
                {
                    ((double *)B)[i * LDA + j] = ((double *)D)[i];
                }
                else
                {
                    ((double *)B)[i * LDA + j] = ((double *)E)[i];
                    ((double *)B)[j * LDA + i] = ((double *)E)[i];
                }
            }
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j <= i + 1 && j < N; j++)
            {
                if(j == i)
                {
                    ((scomplex *)B)[i * LDA + j].real = ((float *)D)[i];
                }
                else
                {
                    ((scomplex *)B)[i * LDA + j].real = ((float *)E)[i];
                    ((scomplex *)B)[j * LDA + i].real = ((float *)E)[i];
                }
            }
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < N; i++)
        {
            for(j = i; j <= i + 1 && j < N; j++)
            {
                if(j == i)
                {
                    ((dcomplex *)B)[i * LDA + j].real = ((double *)D)[i];
                }
                else
                {
                    ((dcomplex *)B)[i * LDA + j].real = ((double *)E)[i];
                    ((dcomplex *)B)[j * LDA + i].real = ((double *)E)[i];
                }
            }
        }
        break;
    }
    }
}
/* Get the maximum value from the array */
void get_max(integer datatype, void *arr, void *max_val, integer n)
{
    integer i;

    switch(datatype)
    {
    case INTEGER:
    {
        integer *ptr = arr;
        integer *maxVal = (integer *)max_val;
        integer maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i] > maxlocal)
            {
                maxlocal = ptr[i];
            }
        }

        *maxVal = maxlocal;
        break;
    }

    case FLOAT:
    {
        float *ptr = arr;
        float *maxVal = (float *)max_val;
        float maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i] > maxlocal)
            {
                maxlocal = ptr[i];
            }
        }

        *maxVal = maxlocal;
        break;
    }

    case DOUBLE:
    {
        double *ptr = arr;
        double *maxVal = (double *)max_val;
        double maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i] > maxlocal)
            {
                maxlocal = ptr[i];
            }
        }

        *maxVal = maxlocal;
        break;
    }

    /* Implementation of complex needs to be relook*/
    case COMPLEX:
    {
        scomplex *ptr = arr;
        scomplex *maxVal = (scomplex *)max_val;
        scomplex maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i].real > maxlocal.real)
            {
                maxlocal = ptr[i];
            }
            else if(ptr[i].real == maxlocal.real)
            {
                if(ptr[i].imag > maxlocal.imag)
                {
                    maxlocal = ptr[i];
                }
            }
        }

        *maxVal = maxlocal;
        break;
    }

    /* Implementation of complex needs to be relook*/
    case DOUBLE_COMPLEX:
    {
        dcomplex *ptr = arr;
        dcomplex *maxVal = (dcomplex *)max_val;
        dcomplex maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i].real > maxlocal.real)
            {
                maxlocal = ptr[i];
            }
            else if(ptr[i].real == maxlocal.real)
            {
                if(ptr[i].imag > maxlocal.imag)
                {
                    maxlocal = ptr[i];
                }
            }
        }

        *maxVal = maxlocal;
        break;
    }
    }
}

/* Get the minimum value from the array */
void get_min(integer datatype, void *arr, void *min_val, integer n)
{
    integer i;

    switch(datatype)
    {
    case INTEGER:
    {
        integer *ptr = arr;
        integer *maxVal = (integer *)min_val;
        integer maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i] < maxlocal)
            {
                maxlocal = ptr[i];
            }
        }

        *maxVal = maxlocal;
        break;
    }

    case FLOAT:
    {
        float *ptr = arr;
        float *maxVal = (float *)min_val;
        float maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i] < maxlocal)
            {
                maxlocal = ptr[i];
            }
        }

        *maxVal = maxlocal;
        break;
    }

    case DOUBLE:
    {
        double *ptr = arr;
        double *maxVal = (double *)min_val;
        double maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i] < maxlocal)
            {
                maxlocal = ptr[i];
            }
        }

        *maxVal = maxlocal;
        break;
    }

    /* Implementation of complex needs to be relook*/
    case COMPLEX:
    {
        scomplex *ptr = arr;
        scomplex *maxVal = (scomplex *)min_val;
        scomplex maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i].real < maxlocal.real)
            {
                maxlocal = ptr[i];
            }
            else if(ptr[i].real == maxlocal.real)
            {
                if(ptr[i].imag < maxlocal.imag)
                {
                    maxlocal = ptr[i];
                }
            }
        }

        *maxVal = maxlocal;
        break;
    }

    /* Implementation of complex needs to be relook*/
    case DOUBLE_COMPLEX:
    {
        dcomplex *ptr = arr;
        dcomplex *maxVal = (dcomplex *)min_val;
        dcomplex maxlocal = ptr[0];

        for(i = 1; i < n; i++)
        {
            if(ptr[i].real < maxlocal.real)
            {
                maxlocal = ptr[i];
            }
            else if(ptr[i].real == maxlocal.real)
            {
                if(ptr[i].imag < maxlocal.imag)
                {
                    maxlocal = ptr[i];
                }
            }
        }

        *maxVal = maxlocal;
        break;
    }
    }
}
/* Reading matrix input data from a file */
void init_matrix_from_file(integer datatype, void *A, integer m, integer n, integer lda, FILE *fptr)
{
    int i, j;
    if(lda < m)
        return;
    switch(datatype)
    {
    case FLOAT:
    {
        float num;

        for(i = 0; i < m; i++)
        {
            for(j = 0; j < n; j++)
            {
                fscanf(fptr, "%f", &num);
                ((float *)A)[i * lda + j] = num;
            }
        }
        break;
    }

    case DOUBLE:
    {
        double num;

        for(i = 0; i < m; i++)
        {
            for(j = 0; j < n; j++)
            {
                fscanf(fptr, "%lf", &num);
                ((double *)A)[i * lda + j] = num;
            }
        }
        break;
    }

    case COMPLEX:
    {
        float num;

        for(i = 0; i < m; i++)
        {
            for(j = 0; j < n; j++)
            {
                fscanf(fptr, "%f", &num);
                ((scomplex *)A)[i * lda + j].real = num;
                fscanf(fptr, "%f", &num);
                ((scomplex *)A)[i * lda + j].imag = num;
            }
        }
        break;
    }

    case DOUBLE_COMPLEX:
    {
        double num;

        for(i = 0; i < m; i++)
        {
            for(j = 0; j < n; j++)
            {
                fscanf(fptr, "%lf", &num);
                ((dcomplex *)A)[i * lda + j].real = num;
                fscanf(fptr, "%lf", &num);
                ((dcomplex *)A)[i * lda + j].imag = num;
            }
        }
        break;
    }
    }
}
/* Reading vector input data from a file */
void init_vector_from_file(integer datatype, void *A, integer m, integer inc, FILE *fptr)
{
    int i;

    switch(datatype)
    {
    case FLOAT:
    {
        float num;

        for(i = 0; i < m; i++)
        {
            fscanf(fptr, "%f", &num);
            ((float *)A)[i * inc] = num;
        }
        break;
    }

    case DOUBLE:
    {
        double num;

        for(i = 0; i < m; i++)
        {
            fscanf(fptr, "%lf", &num);
            ((double *)A)[i * inc] = num;
        }
        break;
    }

    case COMPLEX:
    {
        float num;

        for(i = 0; i < m; i++)
        {
            fscanf(fptr, "%f", &num);
            ((scomplex *)A)[i * inc].real = num;
            fscanf(fptr, "%f", &num);
            ((scomplex *)A)[i * inc].imag = num;
        }
        break;
    }

    case DOUBLE_COMPLEX:
    {
        double num;

        for(i = 0; i < m; i++)
        {
            fscanf(fptr, "%lf", &num);
            ((dcomplex *)A)[i * inc].real = num;
            fscanf(fptr, "%lf", &num);
            ((dcomplex *)A)[i * inc].imag = num;
        }
        break;
    }
    }
}

/* Convert matrix according to ILO and IHI values */
void get_generic_triangular_matrix(
    integer datatype, integer N, void *A, integer LDA, integer ilo, integer ihi)
{
    if(LDA < N)
        return;
    /* Intialize matrix with random values */
    rand_matrix(datatype, A, N, N, LDA);
    integer i;

    switch(datatype)
    {
    case FLOAT:
    {
        /* Making elements below diagonal for columns 0 to ilo-1 to Zero*/
        for(i = 0; i < ilo - 1; i++)
        {
            float *p = &((float *)A)[(i + 1) + i * LDA];
            reset_vector(datatype, (void *)p, N - i - 1, 1);
        }
        /* Making elements below diagonal for rows ihi+1 to N to Zero*/
        for(i = ihi; i < N; i++)
        {
            float *p = &((float *)A)[i];
            reset_vector(datatype, (void *)p, i, LDA);
        }
        break;
    }
    case DOUBLE:
    {
        /* Making elements below diagonal for columns 0 to ilo-1 to Zero*/
        for(i = 0; i < ilo - 1; i++)
        {
            double *p = &((double *)A)[(i + 1) + i * LDA];
            reset_vector(datatype, (void *)p, N - i - 1, 1);
        }
        /* Making elements below diagonal for rows ihi+1 to N to Zero*/
        for(i = ihi; i < N; i++)
        {
            double *p = &((double *)A)[i];
            reset_vector(datatype, (void *)p, i, LDA);
        }
        break;
    }
    case COMPLEX:
    {
        /* Making elements below diagonal for columns 0 to ilo-1 to Zero*/
        for(i = 0; i < ilo - 1; i++)
        {
            scomplex *p = &((scomplex *)A)[(i + 1) + i * LDA];
            reset_vector(datatype, (void *)p, N - i - 1, 1);
        }
        /* Making elements below diagonal for rows ihi+1 to N to Zero*/
        for(i = ihi; i < N; i++)
        {
            scomplex *p = &((scomplex *)A)[i];
            reset_vector(datatype, (void *)p, i, LDA);
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        /* Making elements below diagonal for columns 0 to ilo-1 to Zero*/
        for(i = 0; i < ilo - 1; i++)
        {
            dcomplex *p = &((dcomplex *)A)[(i + 1) + i * LDA];
            reset_vector(datatype, (void *)p, N - i - 1, 1);
        }
        /* Making elements below diagonal for rows ihi+1 to N to Zero*/
        for(i = ihi; i < N; i++)
        {
            dcomplex *p = &((dcomplex *)A)[i];
            reset_vector(datatype, (void *)p, i, LDA);
        }
        break;
    }
    }
}

/* Generate Hessenberg matrix */
void get_hessenberg_matrix(integer datatype,
                           integer n,
                           void *A,
                           integer lda,
                           void *Z,
                           integer ldz,
                           integer *ilo,
                           integer *ihi,
                           void *scale,
                           integer *info)
{
    static integer g_lwork;
    void *A_save = NULL;
    void *tau = NULL, *work = NULL;
    integer lwork;
    if((lda < n) || (ldz < n))
        return;
    create_matrix(datatype, &A_save, lda, n);
    create_vector(datatype, &tau, n - 1);

    /* Convert matrix according to ILO and IHI values */
    get_generic_triangular_matrix(datatype, n, A, lda, *ilo, *ihi);

    switch(datatype)
    {
    case FLOAT:
    {
        /* Make a workspace query the first time through. This will provide us with
                and ideal workspace size based on an internal block size.*/
        if(g_lwork <= 0)
        {
            create_vector(datatype, &work, 1);
            lwork = -1;
            fla_lapack_sgehrd(&n, ilo, ihi, NULL, &lda, NULL, work, &lwork, info);
            if(*info == 0)
            {
                lwork = get_work_value(datatype, work);
                free_vector(work);
            }
        }
        else
        {
            lwork = g_lwork;
        }
        create_vector(datatype, &work, lwork);

        /* Call to SGEHRD API to generate hessenberg matrix*/
        fla_lapack_sgehrd(&n, ilo, ihi, A, &lda, tau, work, &lwork, info);
        reset_vector(datatype, work, lwork, 1);
        copy_matrix(datatype, "full", n, n, A, lda, A_save, lda);

        /* Call to SORGHR API to generate orthogonal matrix*/
        fla_lapack_sorghr(&n, ilo, ihi, A_save, &lda, tau, work, &lwork, info);
        copy_matrix(datatype, "full", n, n, A_save, lda, Z, ldz);

        /* Convert matrix from SGEHRD to upper hessenberg matrix */
        convert_upper_hessenberg(datatype, n, A, lda);

        free_vector(work);
        break;
    }
    case DOUBLE:
    {
        /* Make a workspace query the first time through. This will provide us with
                and ideal workspace size based on an internal block size.*/
        if(g_lwork <= 0)
        {
            create_vector(datatype, &work, 1);
            lwork = -1;
            fla_lapack_dgehrd(&n, ilo, ihi, NULL, &lda, NULL, work, &lwork, info);
            if(*info == 0)
            {
                lwork = get_work_value(datatype, work);
                free_vector(work);
            }
        }
        else
        {
            lwork = g_lwork;
        }

        create_vector(datatype, &work, lwork);

        /* Call to DGEHRD API to generate hessenberg matrix*/
        fla_lapack_dgehrd(&n, ilo, ihi, A, &lda, tau, work, &lwork, info);
        reset_vector(datatype, work, lwork, 1);
        copy_matrix(datatype, "full", n, n, A, lda, A_save, lda);

        /* Call to DORGHR API to generate orthogonal matrix*/
        fla_lapack_dorghr(&n, ilo, ihi, A_save, &lda, tau, work, &lwork, info);
        copy_matrix(datatype, "full", n, n, A_save, lda, Z, ldz);

        /* Convert matrix from DGEHRD to upper hessenberg matrix */
        convert_upper_hessenberg(datatype, n, A, lda);

        free_vector(work);
        break;
    }
    case COMPLEX:
    {
        /* Make a workspace query the first time through. This will provide us with
                and ideal workspace size based on an internal block size.*/
        if(g_lwork <= 0)
        {
            create_vector(datatype, &work, 1);
            lwork = -1;
            fla_lapack_cgehrd(&n, ilo, ihi, NULL, &lda, NULL, work, &lwork, info);
            if(*info == 0)
            {
                lwork = get_work_value(datatype, work);
                free_vector(work);
            }
        }
        else
        {
            lwork = g_lwork;
        }
        create_vector(datatype, &work, lwork);

        /* Call to CGEHRD API to generate hessenberg matrix*/
        fla_lapack_cgehrd(&n, ilo, ihi, A, &lda, tau, work, &lwork, info);
        reset_vector(datatype, work, lwork, 1);
        copy_matrix(datatype, "full", n, n, A, lda, A_save, lda);

        /* Call to CUNGHR API to generate orthogonal matrix*/
        fla_lapack_cunghr(&n, ilo, ihi, A_save, &lda, tau, work, &lwork, info);
        copy_matrix(datatype, "full", n, n, A_save, lda, Z, ldz);

        /* Convert matrix from CGEHRD to upper hessenberg matrix */
        convert_upper_hessenberg(datatype, n, A, lda);

        free_vector(work);
        break;
    }
    case DOUBLE_COMPLEX:
    {
        /* Make a workspace query the first time through. This will provide us with
                and ideal workspace size based on an internal block size.*/
        if(g_lwork <= 0)
        {
            create_vector(datatype, &work, 1);
            lwork = -1;
            fla_lapack_zgehrd(&n, ilo, ihi, NULL, &lda, NULL, work, &lwork, info);
            if(*info == 0)
            {
                lwork = get_work_value(datatype, work);
                free_vector(work);
            }
        }
        else
        {
            lwork = g_lwork;
        }

        create_vector(datatype, &work, lwork);

        /* Call to ZGEHRD API to generate hessenberg matrix*/
        fla_lapack_zgehrd(&n, ilo, ihi, A, &lda, tau, work, &lwork, info);
        reset_vector(datatype, work, lwork, 1);
        copy_matrix(datatype, "full", n, n, A, lda, A_save, lda);

        /* Call to ZUNGHR API to generate orthogonal matrix*/
        fla_lapack_zunghr(&n, ilo, ihi, A_save, &lda, tau, work, &lwork, info);
        copy_matrix(datatype, "full", n, n, A_save, lda, Z, ldz);

        /* Convert matrix from ZGEHRD to upper hessenberg matrix */
        convert_upper_hessenberg(datatype, n, A, lda);

        free_vector(work);
        break;
    }
    }
    free_matrix(A_save);
    free_vector(tau);
}

/* Convert matrix to upper hessenberg form */
void convert_upper_hessenberg(integer datatype, integer n, void *A, integer lda)
{
    integer i;
    switch(datatype)
    {
    case FLOAT:
    {
        for(i = 0; i < n; i++)
        {
            float *p = &((float *)A)[(i + 2) + i * lda];
            reset_vector(datatype, (void *)p, n - i - 2, 1);
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < n; i++)
        {
            double *p = &((double *)A)[(i + 2) + i * lda];
            reset_vector(datatype, (void *)p, n - i - 2, 1);
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < n; i++)
        {
            scomplex *p = &((scomplex *)A)[(i + 2) + i * lda];
            reset_vector(datatype, (void *)p, n - i - 2, 1);
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < n; i++)
        {
            dcomplex *p = &((dcomplex *)A)[(i + 2) + i * lda];
            reset_vector(datatype, (void *)p, n - i - 2, 1);
        }
        break;
    }
    }
}

/* Pack a symmetric matrix in column first order */
void pack_matrix_lt(integer datatype, void *A, void *B, integer N, integer lda)
{
    integer i, j;

    switch(datatype)
    {
    case FLOAT:
    {
        float *bptr = (float *)B;

        for(i = 0; i < N; i++)
        {
            for(j = i; j < N; j++)
            {
                *bptr++ = ((float *)A)[i * lda + j];
            }
        }
        break;
    }
    case DOUBLE:
    {
        double *bptr = B;

        for(i = 0; i < N; i++)
        {
            for(j = i; j < N; j++)
            {
                *bptr++ = ((double *)A)[i * lda + j];
            }
        }
        break;
    }
    case COMPLEX:
    {
        scomplex *bptr = B;

        for(i = 0; i < N; i++)
        {
            for(j = i; j < N; j++)
            {
                bptr->real = ((scomplex *)A)[i * lda + j].real;
                bptr->imag = ((scomplex *)A)[i * lda + j].imag;
                bptr++;
            }
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        dcomplex *bptr = B;

        for(i = 0; i < N; i++)
        {
            for(j = i; j < N; j++)
            {
                bptr->real = ((dcomplex *)A)[i * lda + j].real;
                bptr->imag = ((dcomplex *)A)[i * lda + j].imag;
                bptr++;
            }
        }
        break;
    }
    }

    return;
}

/* Convert matrix to upper hessenberg form */
void extract_upper_hessenberg_matrix(integer datatype, integer n, void *A, integer lda)
{
    integer i;
    switch(datatype)
    {
    case FLOAT:
    {
        /* Making elements below sub diagonal to Zero */
        for(i = 0; i < n; i++)
        {
            float *p = &((float *)A)[(i + 2) + i * lda];
            reset_vector(datatype, (void *)p, n - i - 2, 1);
        }
        break;
    }
    case DOUBLE:
    {
        /* Making elements below sub diagonal to Zero */
        for(i = 0; i < n; i++)
        {
            double *p = &((double *)A)[(i + 2) + i * lda];
            reset_vector(datatype, (void *)p, n - i - 2, 1);
        }
        break;
    }
    case COMPLEX:
    {
        /* Making elements below sub diagonal to Zero */
        for(i = 0; i < n; i++)
        {
            scomplex *p = &((scomplex *)A)[(i + 2) + i * lda];
            reset_vector(datatype, (void *)p, n - i - 2, 1);
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        /* Making elements below sub diagonal to Zero */
        for(i = 0; i < n; i++)
        {
            dcomplex *p = &((dcomplex *)A)[(i + 2) + i * lda];
            reset_vector(datatype, (void *)p, n - i - 2, 1);
        }
        break;
    }
    }
}

/* Decompose matrix A in to QR and store orthogonal matrix in Q and R in A*/
void get_orthogonal_matrix_from_QR(
    integer datatype, integer n, void *A, integer lda, void *Q, integer ldq, integer *info)
{
    void *tau = NULL, *work = NULL;
    integer lwork = -1;
    if((lda < n) || (ldq < n))
        return;
    /* Intializing matrix for the call to GGHRD */
    create_vector(datatype, &work, 1);
    create_vector(datatype, &tau, n);

    switch(datatype)
    {
    case FLOAT:
    {
        /* Generating orthogonal matrix Q by QR reduction of A */
        copy_matrix(datatype, "full", n, n, A, lda, Q, ldq);
        fla_lapack_sgeqrf(&n, &n, NULL, &ldq, NULL, work, &lwork, info);
        if(*info < 0)
            break;
        else
            lwork = get_work_value(datatype, work);
        free_vector(work);
        create_vector(datatype, &work, lwork);
        /* Call to SGEQRF to decompose matrix to QR form */
        fla_lapack_sgeqrf(&n, &n, Q, &ldq, tau, work, &lwork, info);
        if(*info < 0)
            break;
        reset_matrix(datatype, n, n, A, lda);
        copy_matrix(datatype, "Upper", n, n, Q, ldq, A, lda);
        reset_vector(datatype, work, lwork, 1);
        /* Call to SORGQR to calculate matrix to Q */
        fla_lapack_sorgqr(&n, &n, &n, Q, &ldq, tau, work, &lwork, info);
        if(*info < 0)
            break;
        break;
    }
    case DOUBLE:
    {
        /* Generating orthogonal matrix Q by QR reduction of A */
        copy_matrix(datatype, "full", n, n, A, lda, Q, ldq);
        fla_lapack_dgeqrf(&n, &n, NULL, &ldq, NULL, work, &lwork, info);
        if(*info < 0)
            break;
        else
            lwork = get_work_value(datatype, work);
        free_vector(work);
        create_vector(datatype, &work, lwork);
        /* Call to DGEQRF to decompose matrix to QR form */
        fla_lapack_dgeqrf(&n, &n, Q, &ldq, tau, work, &lwork, info);
        if(*info < 0)
            break;
        reset_matrix(datatype, n, n, A, lda);
        copy_matrix(datatype, "Upper", n, n, Q, ldq, A, lda);
        reset_vector(datatype, work, lwork, 1);
        /* Call to DORGQR to calculate matrix to Q */
        fla_lapack_dorgqr(&n, &n, &n, Q, &ldq, tau, work, &lwork, info);
        if(*info < 0)
            break;
        break;
    }
    case COMPLEX:
    {
        /* Generating orthogonal matrix Q by QR reduction of A */
        copy_matrix(datatype, "full", n, n, A, lda, Q, ldq);
        fla_lapack_cgeqrf(&n, &n, NULL, &ldq, NULL, work, &lwork, info);
        if(*info < 0)
            break;
        else
            lwork = get_work_value(datatype, work);
        free_vector(work);
        create_vector(datatype, &work, lwork);
        /* Call to CGEQRF to decompose matrix to QR form */
        fla_lapack_cgeqrf(&n, &n, Q, &ldq, tau, work, &lwork, info);
        if(*info < 0)
            break;
        reset_matrix(datatype, n, n, A, lda);
        copy_matrix(datatype, "Upper", n, n, Q, ldq, A, lda);
        reset_vector(datatype, work, lwork, 1);
        /* Call to CUNGQR to calculate matrix to Q */
        fla_lapack_cungqr(&n, &n, &n, Q, &ldq, tau, work, &lwork, info);
        if(*info < 0)
            break;
        break;
    }
    case DOUBLE_COMPLEX:
    {
        /* Generating orthogonal matrix Q by QR reduction of A */
        copy_matrix(datatype, "full", n, n, A, lda, Q, ldq);
        fla_lapack_zgeqrf(&n, &n, NULL, &ldq, NULL, work, &lwork, info);
        if(*info < 0)
            break;
        else
            lwork = get_work_value(datatype, work);
        free_vector(work);
        create_vector(datatype, &work, lwork);
        /* Call to ZGEQRF to decompose matrix to QR form */
        fla_lapack_zgeqrf(&n, &n, Q, &ldq, tau, work, &lwork, info);
        if(*info < 0)
            break;
        reset_matrix(datatype, n, n, A, lda);
        copy_matrix(datatype, "Upper", n, n, Q, ldq, A, lda);
        reset_vector(datatype, work, lwork, 1);
        /* Call to ZUNGQR to calculate matrix to Q */
        fla_lapack_zungqr(&n, &n, &n, Q, &ldq, tau, work, &lwork, info);
        if(*info < 0)
            break;
        break;
    }
    }
    free_vector(tau);
    free_vector(work);
}

/* Print matrix contents for visual inspection */
void print_matrix(char *desc, integer datatype, integer M, integer N, void *A, integer lda)
{
    integer i, j;

    printf("\n %s:\n", desc);
    switch(datatype)
    {
    case INTEGER:
    {
        for(i = 0; i < M; i++)
        {
            for(j = 0; j < N; j++)
            {
                printf(" %" FT_IS " ", ((integer *)A)[i + j * lda]);
            }
            printf("\n");
        }
        break;
    }
    case FLOAT:
    {
        for(i = 0; i < M; i++)
        {
            for(j = 0; j < N; j++)
            {
                printf(" %e", ((float *)A)[i + j * lda]);
            }
            printf("\n");
        }
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < M; i++)
        {
            for(j = 0; j < N; j++)
            {
                printf(" %e", ((double *)A)[i + j * lda]);
            }
            printf("\n");
        }
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < M; i++)
        {
            for(j = 0; j < N; j++)
            {
                printf(" (%e + j %e)",
                       ((scomplex *)A)[i + j * lda].real,
                       ((scomplex *)A)[i + j * lda].imag);
            }
            printf("\n");
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < M; i++)
        {
            for(j = 0; j < N; j++)
            {
                printf(" (%e + j %e)",
                       ((dcomplex *)A)[i + j * lda].real,
                       ((dcomplex *)A)[i + j * lda].imag);
            }
            printf("\n");
        }
        break;
    }
    }
}

/* Get upper triangular matrix or lower triangular matrix based on UPLO */
void get_triangular_matrix(char *uplo, integer datatype, integer m, integer n, void *A, integer lda)
{
    rand_matrix(datatype, A, m, n, lda);
    integer i;

    switch(datatype)
    {
    case FLOAT:
    {
        if(*uplo == 'U')
        {
            for(i = 0; i < n; i++)
            {
                float *p = &((float *)A)[(i + 1) + i * lda];
                reset_vector(datatype, (void *)p, m - i - 1, 1);
            }
        }
        if(*uplo == 'L')
        {
            for(i = 0; i < n; i++)
            {
                float *p = &((float *)A)[i * lda];
                reset_vector(datatype, (void *)p, i + 1, 1);
            }
        }
        break;
    }
    case DOUBLE:
    {
        if(*uplo == 'U')
        {
            for(i = 0; i < n; i++)
            {
                double *p = &((double *)A)[(i + 1) + i * lda];
                reset_vector(datatype, (void *)p, m - i - 1, 1);
            }
        }
        if(*uplo == 'L')
        {
            for(i = 0; i < n; i++)
            {
                double *p = &((double *)A)[i * lda];
                reset_vector(datatype, (void *)p, i + 1, 1);
            }
        }
        break;
    }
    case COMPLEX:
    {
        if(*uplo == 'U')
        {
            for(i = 0; i < n; i++)
            {
                scomplex *p = &((scomplex *)A)[(i + 1) + i * lda];
                reset_vector(datatype, (void *)p, m - i - 1, 1);
            }
        }
        if(*uplo == 'L')
        {
            for(i = 0; i < n; i++)
            {
                scomplex *p = &((scomplex *)A)[i * lda];
                reset_vector(datatype, (void *)p, i + 1, 1);
            }
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        if(*uplo == 'U')
        {
            for(i = 0; i < n; i++)
            {
                dcomplex *p = &((dcomplex *)A)[(i + 1) + i * lda];
                reset_vector(datatype, (void *)p, m - i - 1, 1);
            }
        }
        if(*uplo == 'L')
        {
            for(i = 0; i < n; i++)
            {
                dcomplex *p = &((dcomplex *)A)[i * lda];
                reset_vector(datatype, (void *)p, i + 1, 1);
            }
        }
        break;
    }
    }
}

/*Test to Check order of Singular values of SVD (positive and non-decreasing)*/
double svd_check_order(integer datatype, void *s, integer m, integer n, double residual)
{
    integer min_m_n, i;
    min_m_n = fla_min(m, n);
    double resid = 0.;

    switch(datatype)
    {
    case INTEGER:
    {
        for(i = 0; i < (min_m_n - 1); i++)
        {
            if((((int *)s)[i] < 0) || (((int *)s)[i] < ((int *)s)[i + 1]))
            {
                resid = residual * 2;
                break;
            }
        }
        if(((int *)s)[min_m_n - 1] < 0)
            resid = residual * 2;
        break;
    }
    case FLOAT:
    {
        for(i = 0; i < (min_m_n - 1); i++)
        {
            if((((float *)s)[i] < 0.f) || (((float *)s)[i] < ((float *)s)[i + 1]))
            {
                resid = residual * 2;
                break;
            }
        }
        if(((float *)s)[min_m_n - 1] < 0.f)
            resid = residual * 2;
        break;
    }
    case DOUBLE:
    {
        for(i = 0; i < (min_m_n - 1); i++)
        {
            if((((double *)s)[i] < 0.) || (((double *)s)[i] < ((double *)s)[i + 1]))
            {
                resid = residual * 2;
                break;
            }
        }
        if(((double *)s)[min_m_n - 1] < 0.)
            resid = residual * 2;
        break;
    }
    case COMPLEX:
    {
        for(i = 0; i < (min_m_n - 1); i++)
        {
            if((((float *)s)[i] < 0.f) || (((float *)s)[i] < ((float *)s)[i + 1]))
            {
                resid = residual * 2;
                break;
            }
        }
        if(((float *)s)[min_m_n - 1] < 0.f)
            resid = residual * 2;
        break;
    }
    case DOUBLE_COMPLEX:
    {
        for(i = 0; i < (min_m_n - 1); i++)
        {
            if((((double *)s)[i] < 0.) || (((double *)s)[i] < ((double *)s)[i + 1]))
            {
                resid = residual * 2;
                break;
            }
        }
        if(((double *)s)[min_m_n - 1] < 0.)
            resid = residual * 2;
        break;
    }
    default:
        break;
    }
    return resid;
}

/* Intialize matrix with special values*/
void init_matrix_spec_in(integer datatype, void *A, integer M, integer N, integer LDA, char type)
{
    integer i, j;
    if(LDA < M)
        return;
    switch(datatype)
    {
    case FLOAT:
    {
        float value = 0.f;
        if(type == 'I')
            value = INFINITY;
        else if(type == 'N')
            value = NAN;
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((float *)A)[i * LDA + j] = value;
            }
        }
        break;
    }
    case DOUBLE:
    {
        double value = 0.;
        if(type == 'I')
            value = INFINITY;
        else if(type == 'N')
            value = NAN;
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((double *)A)[i * LDA + j] = value;
            }
        }
        break;
    }
    case COMPLEX:
    {
        float value = 0.f;
        if(type == 'I')
            value = INFINITY;
        else if(type == 'N')
            value = NAN;
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((scomplex *)A)[i * LDA + j].real = value;
                ((scomplex *)A)[i * LDA + j].imag = value;
            }
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        double value = 0.;
        if(type == 'I')
            value = INFINITY;
        else if(type == 'N')
            value = NAN;
        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                ((dcomplex *)A)[i * LDA + j].real = value;
                ((dcomplex *)A)[i * LDA + j].imag = value;
            }
        }
        break;
    }
    }

    return;
}

/* Intialize matrix with special values in random locations */
void init_matrix_spec_rand_in(
    integer datatype, void *A, integer M, integer N, integer LDA, char type)
{
    integer rows, cols, upspan, lowspan, span;
    if(LDA < M)
        return;
    rand_matrix(datatype, A, M, N, LDA);
    /* when M*N less than 2 there is no need of randomness*/
    if(M * N < 2)
        return;
    /*
    Add random extreme values:
    for small size matrices, when M*N less than 10 adding one extreme value in upper triangular
    matrix, other one extreme value in lower triangular matrix
    for medium/large sizes, when M*N greater than 10 adding 10% of input values as extreme values
    in upper triangular matrix, other 10% of input values as exterme values in lower triangular matrix
    */
    if(M * N > 10)
    {
        lowspan = (M * N) * 0.1;
        upspan = (M * N) * 0.1;
    }
    else
    {
        lowspan = 1;
        upspan = 1;
    }
    span = lowspan + upspan;
    switch(datatype)
    {
    case FLOAT:
    {
        float value = 0.f;
        if(type == 'F')
            value = INFINITY;
        else if(type == 'A')
            value = NAN;
        while(span > 0)
        {
            rows = rand() % M;
            cols = rand() % N;
            /* Replace 10 percent of special values in upper triangular matrix */
            if(upspan > 0)
            {
                if(rows <= cols)
                {
                    if(!isnan(((float *)A)[cols * LDA + rows]))
                    {
                        ((float *)A)[cols * LDA + rows] = value;
                        upspan = upspan - 1;
                    }
                }
            }
            /* Replace 10 percent of special values in lower triangular matrix */
            else if(lowspan > 0)
            {
                if(rows >= cols)
                {
                    if(!isnan(((float *)A)[cols * LDA + rows]))
                    {
                        ((float *)A)[cols * LDA + rows] = value;
                        lowspan = lowspan - 1;
                    }
                }
            }
            span = lowspan + upspan;
        }
        break;
    }
    case DOUBLE:
    {
        double value = 0.;
        if(type == 'F')
            value = INFINITY;
        else if(type == 'A')
            value = NAN;
        while(span > 0)
        {
            rows = rand() % M;
            cols = rand() % N;
            if(upspan > 0)
            {
                if(rows <= cols)
                {
                    if(!isnan(((double *)A)[cols * LDA + rows]))
                    {
                        ((double *)A)[cols * LDA + rows] = value;
                        upspan = upspan - 1;
                    }
                }
            }
            else if(lowspan > 0)
            {
                if(rows >= cols)
                {
                    if(!isnan(((double *)A)[cols * LDA + rows]))
                    {
                        ((double *)A)[cols * LDA + rows] = value;
                        lowspan = lowspan - 1;
                    }
                }
            }
            span = lowspan + upspan;
        }
        break;
    }
    case COMPLEX:
    {
        float value = 0.f;
        if(type == 'F')
            value = INFINITY;
        else if(type == 'A')
            value = NAN;
        while(span > 0)
        {
            rows = rand() % M;
            cols = rand() % N;
            if(upspan > 0)
            {
                if(rows <= cols)
                {
                    if(!isnan(((scomplex *)A)[cols * LDA + rows].real))
                    {
                        ((scomplex *)A)[cols * LDA + rows].real = value;
                        ((scomplex *)A)[cols * LDA + rows].imag = value;
                        upspan = upspan - 1;
                    }
                }
            }
            else if(lowspan > 0)
            {
                if(rows >= cols)
                {
                    if(!isnan(((scomplex *)A)[cols * LDA + rows].real))
                    {
                        ((scomplex *)A)[cols * LDA + rows].real = value;
                        ((scomplex *)A)[cols * LDA + rows].imag = value;
                        lowspan = lowspan - 1;
                    }
                }
            }
            span = lowspan + upspan;
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        double value = 0.;
        if(type == 'F')
            value = INFINITY;
        else if(type == 'A')
            value = NAN;
        while(span > 0)
        {
            rows = rand() % M;
            cols = rand() % N;
            if(upspan > 0)
            {
                if(rows <= cols)
                {
                    if(!isnan(((dcomplex *)A)[cols * LDA + rows].real))
                    {
                        ((dcomplex *)A)[cols * LDA + rows].real = value;
                        ((dcomplex *)A)[cols * LDA + rows].imag = value;
                        upspan = upspan - 1;
                    }
                }
            }
            else if(lowspan > 0)
            {
                if(rows >= cols)
                {
                    if(!isnan(((dcomplex *)A)[cols * LDA + rows].real))
                    {
                        ((dcomplex *)A)[cols * LDA + rows].real = value;
                        ((dcomplex *)A)[cols * LDA + rows].imag = value;
                        lowspan = lowspan - 1;
                    }
                }
            }
            span = lowspan + upspan;
        }
        break;
    }
    }

    return;
}

/* Test to check the extreme values propagation in output matrix */
bool check_extreme_value(integer datatype, integer M, integer N, void *A, integer LDA, char type)
{
    if(!A)
        return false;
    integer i, j;
    switch(datatype)
    {
    case FLOAT:
    {
        if(type == 'A' || type == 'N')
        {
            for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)
                {
                    if(isnan(((float *)A)[i * LDA + j]))
                    {
                        return true;
                    }
                }
            }
        }

        else if(type == 'F' || type == 'I')
        {
            for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)
                {
                    if((isinf(((float *)A)[i * LDA + j])) || (isnan(((float *)A)[i * LDA + j])))
                    {
                        return true;
                    }
                }
            }
        }
        break;
    }

    case DOUBLE:
    {
        if(type == 'A' || type == 'N')
        {
            for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)
                {
                    if(isnan(((double *)A)[i * LDA + j]))
                    {
                        return true;
                    }
                }
            }
        }

        if(type == 'F' || type == 'I')
        {
            for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)
                {
                    if((isinf(((double *)A)[i * LDA + j])) || (isnan(((double *)A)[i * LDA + j])))
                    {
                        return true;
                    }
                }
            }
        }
        break;
    }

    case COMPLEX:
    {
        if(type == 'A' || type == 'N')
        {
            for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)
                {
                    if(isnan(((scomplex *)A)[i * LDA + j].real)
                       && isnan(((scomplex *)A)[i * LDA + j].imag))
                    {
                        return true;
                    }
                }
            }
        }

        else if(type == 'F' || type == 'I')
        {
            for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)
                {
                    if((isinf(((scomplex *)A)[i * LDA + j].real)
                        && isinf(((scomplex *)A)[i * LDA + j].imag))
                       || (isnan(((scomplex *)A)[i * LDA + j].real)
                           && isnan(((scomplex *)A)[i * LDA + j].imag)))
                    {
                        return true;
                    }
                }
            }
        }
        break;
    }

    case DOUBLE_COMPLEX:
    {
        if(type == 'A' || type == 'N')
        {
            for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)
                {
                    if(isnan(((dcomplex *)A)[i * LDA + j].real)
                       && isnan(((dcomplex *)A)[i * LDA + j].imag))
                    {
                        return true;
                    }
                }
            }
        }

        else if(type == 'F' || type == 'I')
        {
            for(i = 0; i < N; i++)
            {
                for(j = 0; j < M; j++)
                {
                    if((isinf(((dcomplex *)A)[i * LDA + j].real)
                        && isinf(((dcomplex *)A)[i * LDA + j].imag))
                       || (isnan(((dcomplex *)A)[i * LDA + j].real)
                           && isnan(((dcomplex *)A)[i * LDA + j].imag)))
                    {
                        return true;
                    }
                }
            }
        }
        break;
    }
    }
    return 0;
}

/* Intialize vector with special values */
void init_vector_spec_in(integer datatype, void *A, integer M, integer incx, char type)
{
    integer i;
    switch(datatype)
    {
    case FLOAT:
    {
        float value = 0.f;
        if(type == 'I')
            value = INFINITY;
        else if(type == 'N')
            value = NAN;
        for(i = 0; i < M; i++)
        {
            ((float *)A)[i * incx] = value;
        }
        break;
    }
    case DOUBLE:
    {
        double value = 0.;
        if(type == 'I')
            value = INFINITY;
        else if(type == 'N')
            value = NAN;
        for(i = 0; i < M; i++)
        {
            ((double *)A)[i * incx] = value;
        }
        break;
    }
    case COMPLEX:
    {
        float value = 0.f;
        if(type == 'I')
            value = INFINITY;
        else if(type == 'N')
            value = NAN;
        for(i = 0; i < M; i++)
        {
            ((scomplex *)A)[i * incx].real = value;
            ((scomplex *)A)[i * incx].imag = value;
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        double value = 0.;
        if(type == 'I')
            value = INFINITY;
        else if(type == 'N')
            value = NAN;
        for(i = 0; i < M; i++)
        {
            ((dcomplex *)A)[i * incx].real = value;
            ((dcomplex *)A)[i * incx].imag = value;
        }
        break;
    }
    }

    return;
}

/*Intialize matrix according to given input*/
void init_matrix(integer datatype,
                 void *A,
                 integer M,
                 integer N,
                 integer LDA,
                 FILE *g_ext_fptr,
                 char imatrix_char)
{
    if(g_ext_fptr != NULL)
        init_matrix_from_file(datatype, A, M, N, LDA, g_ext_fptr);
    else if(imatrix_char == 'I' || imatrix_char == 'N')
        init_matrix_spec_in(datatype, A, M, N, LDA, imatrix_char);
    else if(imatrix_char == 'A' || imatrix_char == 'F')
        init_matrix_spec_rand_in(datatype, A, M, N, LDA, imatrix_char);
    else
        rand_matrix(datatype, A, M, N, LDA);
}

/* Checks whether the value is zero or not */
double is_value_zero(integer datatype, void *value, double residual)
{
    double resid = residual;
    switch(datatype)
    {
    case FLOAT:
    {
        if(*(float *)value != s_zero)
        {
            resid = residual * 2.0;
        }
        break;
    }
    case DOUBLE:
    {
        if(*(double *)value != d_zero)
        {
            resid = residual * 2.0;
        }
        break;
    }
    case COMPLEX:
    {
        if(((scomplex *)value)[0].real != s_zero || ((scomplex *)value)[0].imag != s_zero)
        {
            resid = residual * 2.0;
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        if(((dcomplex *)value)[0].real != d_zero || ((dcomplex *)value)[0].imag != d_zero)
        {
            resid = residual * 2.0;
        }
        break;
    }
    }
    return resid;
}

/*Intialize vector according to given input*/
void init_vector(
    integer datatype, void *A, integer M, integer incx, FILE *g_ext_fptr, char ivector_char)
{
    if(g_ext_fptr != NULL)
        init_vector_from_file(datatype, A, M, incx, g_ext_fptr);
    else if(ivector_char == 'I' || ivector_char == 'N')
        init_vector_spec_in(datatype, A, M, incx, ivector_char);
    else
        rand_vector(datatype, A, M, incx);
}

/* General matrix multiplication of tridiagonal matrix using LAGTM
 * C = tridiag_matrix[du, d, dl] * B */
void tridiag_matrix_multiply(integer datatype,
                             integer n,
                             integer nrhs,
                             void *dl,
                             void *d,
                             void *du,
                             void *B,
                             integer ldb,
                             void *C,
                             integer ldc)
{
    if(ldb < n || ldc < n)
        return;

    switch(datatype)
    {
    case FLOAT:
    {
        slagtm_("N", &n, &nrhs, &s_one, dl, d, du, B, &ldb, &s_zero, C, &ldc);
        break;
    }
    case DOUBLE:
    {
        dlagtm_("N", &n, &nrhs, &d_one, dl, d, du, B, &ldb, &d_zero, C, &ldc);
        break;
    }
    case COMPLEX:
    {
        clagtm_("N", &n, &nrhs, &s_one, dl, d, du, B, &ldb, &s_zero, C, &ldc);
        break;
    }
    case DOUBLE_COMPLEX:
    {
        zlagtm_("N", &n, &nrhs, &d_one, dl, d, du, B, &ldb, &d_zero, C, &ldc);
        break;
    }
    }
}

/* Calculate the difference between two matrix  A = A - B */
void matrix_difference(
    integer datatype, integer m, integer n, void *A, integer lda, void *B, integer ldb)
{
    integer i = 0;
    switch(datatype)
    {
    case FLOAT:
    {
        float *float_A, *float_B;
        for(i = 0; i < n; i++)
        {
            float_A = ((float *)A + (i * lda));
            float_B = ((float *)B + (i * ldb));
            saxpy_(&m, &s_n_one, float_B, &i_one, float_A, &i_one);
        }
        break;
    }
    case DOUBLE:
    {
        double *double_A, *double_B;
        for(i = 0; i < n; i++)
        {
            double_A = ((double *)A + (i * lda));
            double_B = ((double *)B + (i * ldb));
            daxpy_(&m, &d_n_one, double_B, &i_one, double_A, &i_one);
        }
        break;
    }
    case COMPLEX:
    {
        scomplex *scomplex_A, *scomplex_B;
        for(i = 0; i < n; i++)
        {
            scomplex_A = ((scomplex *)A + (i * lda));
            scomplex_B = ((scomplex *)B + (i * ldb));
            caxpy_(&m, &c_n_one, scomplex_B, &i_one, scomplex_A, &i_one);
        }
        break;
    }
    case DOUBLE_COMPLEX:
    {
        dcomplex *dcomplex_A, *dcomplex_B;
        for(i = 0; i < n; i++)
        {
            dcomplex_A = ((dcomplex *)A + (i * lda));
            dcomplex_B = ((dcomplex *)B + (i * ldb));
            zaxpy_(&m, &z_n_one, dcomplex_B, &i_one, dcomplex_A, &i_one);
        }
        break;
    }
    }
    return;
}

/* Copy tridiagonal matrix */
void copy_tridiag_matrix(
    integer datatype, void *dl, void *d, void *du, integer M, integer N, void *A, integer LDA)
{
    if(LDA < M)
        return;
    integer inc = LDA + 1;
    integer min_m_n, dl_size, du_size;
    min_m_n = fla_min(M, N);
    dl_size = N - 1, du_size = N - 1;

    reset_matrix(datatype, M, N, A, LDA);

    switch(datatype)
    {
    case FLOAT:
    {
        float *d_ptr = ((float *)A);
        float *du_ptr = ((float *)A + LDA);
        float *dl_ptr = ((float *)A + 1);
        scopy_(&min_m_n, d, &i_one, d_ptr, &inc);
        scopy_(&du_size, du, &i_one, du_ptr, &inc);
        scopy_(&dl_size, dl, &i_one, dl_ptr, &inc);
        break;
    }
    case DOUBLE:
    {
        double *d_ptr = ((double *)A);
        double *du_ptr = ((double *)A + LDA);
        double *dl_ptr = ((double *)A + 1);
        dcopy_(&min_m_n, d, &i_one, d_ptr, &inc);
        dcopy_(&du_size, du, &i_one, du_ptr, &inc);
        dcopy_(&dl_size, dl, &i_one, dl_ptr, &inc);
        break;
    }
    case COMPLEX:
    {
        scomplex *d_ptr = ((scomplex *)A);
        scomplex *du_ptr = ((scomplex *)A + LDA);
        scomplex *dl_ptr = ((scomplex *)A + 1);
        ccopy_(&min_m_n, d, &i_one, d_ptr, &inc);
        ccopy_(&du_size, du, &i_one, du_ptr, &inc);
        ccopy_(&dl_size, dl, &i_one, dl_ptr, &inc);
        break;
    }
    case DOUBLE_COMPLEX:
    {
        dcomplex *d_ptr = ((dcomplex *)A);
        dcomplex *du_ptr = ((dcomplex *)A + LDA);
        dcomplex *dl_ptr = ((dcomplex *)A + 1);
        zcopy_(&min_m_n, d, &i_one, d_ptr, &inc);
        zcopy_(&du_size, du, &i_one, du_ptr, &inc);
        zcopy_(&dl_size, dl, &i_one, dl_ptr, &inc);
        break;
    }
    }
}