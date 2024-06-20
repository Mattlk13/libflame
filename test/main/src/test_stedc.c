/*
    Copyright (C) 2022-2024, Advanced Micro Devices, Inc. All rights reserved.
*/

#include "test_lapack.h"

/* Local prototypes. */
void fla_test_stedc_experiment(test_params_t *params, integer datatype, integer p_cur,
                               integer q_cur, integer pci, integer n_repeats, integer einfo,
                               double *perf, double *t, double *residual);
void prepare_stedc_run(char *compz, integer n, void *D, void *E, void *Z, integer ldz,
                       integer datatype, integer n_repeats, double *time_min_, integer *info);
void invoke_stedc(integer datatype, char *compz, integer *n, void *D, void *E, void *Z,
                  integer *ldz, void *work, integer *lwork, void *rwork, integer *lrwork,
                  integer *iwork, integer *liwork, integer *info);

void fla_test_stedc(integer argc, char **argv, test_params_t *params)
{
    char *op_str = "Eigenvalues/eigenvectors of symmetric tridiagonal matrix";
    char *front_str = "STEDC";
    integer tests_not_run = 1, invalid_dtype = 0, einfo = 0;

    if(argc == 1)
    {
        g_lwork = -1;
        g_liwork = -1;
        g_lrwork = -1;
        config_data = 1;
        fla_test_output_info("--- %s ---\n", op_str);
        fla_test_output_info("\n");
        fla_test_op_driver(front_str, SQUARE_INPUT, params, EIG_SYM, fla_test_stedc_experiment);
        tests_not_run = 0;
    }
    if(argc == 11)
    {
        FLA_TEST_PARSE_LAST_ARG(argv[10]);
    }
    if(argc >= 10 && argc <= 11)
    {
        /* Test with parameters from commandline */
        integer i, num_types, N;
        integer datatype, n_repeats;
        double perf, time_min, residual;
        char stype, type_flag[4] = {0};
        char *endptr;

        /* Parse the arguments */
        num_types = strlen(argv[2]);
        params->eig_sym_paramslist[0].compz = argv[3][0];
        N = strtoimax(argv[4], &endptr, CLI_DECIMAL_BASE);
        params->eig_sym_paramslist[0].lda = strtoimax(argv[5], &endptr, CLI_DECIMAL_BASE);

        g_lwork = strtoimax(argv[6], &endptr, CLI_DECIMAL_BASE);
        g_liwork = strtoimax(argv[7], &endptr, CLI_DECIMAL_BASE);
        g_lrwork = strtoimax(argv[8], &endptr, CLI_DECIMAL_BASE);
        n_repeats = strtoimax(argv[9], &endptr, CLI_DECIMAL_BASE);

        if(n_repeats > 0)
        {
            params->eig_sym_paramslist[0].threshold_value = CLI_NORM_THRESH;

            for(i = 0; i < num_types; i++)
            {
                stype = argv[2][i];
                datatype = get_datatype(stype);

                /* Check for invalide dataype */
                if(datatype == INVALID_TYPE)
                {
                    invalid_dtype = 1;
                    continue;
                }

                /* Check for duplicate datatype presence */
                if(type_flag[datatype - FLOAT] == 1)
                    continue;
                type_flag[datatype - FLOAT] = 1;

                /* Call the test code */
                fla_test_stedc_experiment(params, datatype, N, N, 0, n_repeats, einfo, &perf,
                                          &time_min, &residual);
                /* Print the results */
                fla_test_print_status(front_str, stype, SQUARE_INPUT, N, N, residual,
                                      params->eig_sym_paramslist[0].threshold_value, time_min,
                                      perf);
                tests_not_run = 0;
            }
        }
    }

    /* Print error messages */
    if(tests_not_run)
    {
        printf("Invalid arguments for stedc\n");
        printf("Usage: ./<EXE> stedc <precisions - sdcz> <COMPZ> <N> <LDZ> <LWORk> <LIWORK> "
               "<LRWORK> <repeats>\n");
    }
    else if(invalid_dtype)
    {
        printf("\nInvalid datatypes specified, choose valid datatypes from 'sdcz'\n");
    }
    if(g_ext_fptr != NULL)
    {
        fclose(g_ext_fptr);
        g_ext_fptr = NULL;
    }
}

void fla_test_stedc_experiment(test_params_t *params, integer datatype, integer p_cur,
                               integer q_cur, integer pci, integer n_repeats, integer einfo,
                               double *perf, double *t, double *residual)
{
    integer n, info = 0, realtype, lda, ldz, vinfo = 0;
    void *D = NULL, *D_test = NULL, *E = NULL, *E_test = NULL, *Z_test = NULL;
    void *Z_input = NULL, *A = NULL;
    double time_min = 1e9;
    char compz, uplo;

    /* Get input matrix dimensions. */
    n = q_cur;

    /* Initialize parameter needed for STEDC() call. */
    compz = params->eig_sym_paramslist[pci].compz;
    lda = params->eig_sym_paramslist[pci].lda;
    *residual = params->eig_sym_paramslist[pci].threshold_value;

    /* If leading dimensions = -1, set them to default value
       when inputs are from config files */
    if(config_data)
    {
        if(lda == -1)
        {
            lda = fla_max(1, n);
        }
    }

    create_matrix(datatype, &A, lda, n);
    reset_matrix(datatype, lda, n, A, lda);

    realtype = get_realtype(datatype);
    create_vector(realtype, &D, n);
    create_vector(realtype, &E, n - 1);

    /* Initialize input matrix with random/custom data */
    init_matrix(realtype, D, 1, n, 1, g_ext_fptr, params->imatrix_char);
    init_matrix(realtype, E, 1, n - 1, 1, g_ext_fptr, params->imatrix_char);
    
    if(compz == 'I')
    {
        copy_sym_tridiag_matrix(datatype, D, E, n, n, A, lda);
    }
    else if (compz == 'V')
    {
        init_matrix(datatype, A, n, n, lda, g_ext_fptr, params->imatrix_char);
        /* Get the symmetric/hermitian matrix.*/
        form_symmetric_matrix(datatype, n, A, lda, "C");
    }

    ldz = lda;
    create_matrix(datatype, &Z_input, ldz, n);
    if (compz != 'N')
    {
        /* Make a copy of input matrices. This is required to validate the API functionality. */
        copy_matrix(datatype, "full", n, n, A, lda, Z_input, ldz);
    }
    create_matrix(datatype, &Z_test, ldz, n);

    /* Call SYTRD(), ORGTR() to get tridiagonal, orthogonal/unitary matrix when compz = V. */
    if((compz == 'V') && (params->imatrix_char == NULL) && (g_ext_fptr == NULL))
    {
        /* Initialize parameter needed for SYTRD() call. */
        uplo = 'U';
        /* invoke_sytrd() internally calls ORGTR() to get orthogonal matrix.*/
        invoke_sytrd(datatype, &uplo, compz, n, A, lda, D, E, &info);
    }

    /* Copy orthogonal/unitary matrix to Z_test. */
    if(compz == 'V')
    {
        copy_matrix(datatype, "full", n, n, A, lda, Z_test, ldz);
    }
    
    create_vector(realtype, &D_test, n);
    copy_vector(realtype, n, D, 1, D_test, 1);
    create_vector(realtype, &E_test, n - 1);
    copy_vector(realtype, n - 1, E, 1, E_test, 1);

    prepare_stedc_run(&compz, n, D_test, E_test, Z_test, ldz, datatype, n_repeats, &time_min,
                      &info);

    /* Execution time. */
    *t = time_min;
    /* Performance computation
       (6)n^3 flops for eigen vectors
       (4/3)n^3 flops for eigen values. */
    *perf = (double)((4.0 / 3.0) * n * n * n) / *t / FLOPS_PER_UNIT_PERF;
    if(compz != 'N')
    {
        *perf += (double)(6 * n * n * n) / *t / FLOPS_PER_UNIT_PERF;
    }
    if(datatype == COMPLEX || datatype == DOUBLE_COMPLEX)
    {
        *perf *= 2.0;
    }

    /* Output validation. */
    if(!params->imatrix_char && (info == 0))
    {
        validate_stedc(compz, n, D_test, Z_input, Z_test, ldz, datatype, residual, &vinfo);
    }
    /* Check for output matrix & vectors when inputs are extreme values */
    else if(FLA_EXTREME_CASE_TEST)
    {
        if(!check_extreme_value(datatype, n, n, Z_test, lda, params->imatrix_char)
           && !check_extreme_value(datatype, n, 1, D_test, 1, params->imatrix_char))
        {
            *residual = DBL_MAX;
        }
    }
    else
    {
        FLA_TEST_CHECK_EINFO(residual, info, einfo);
    }

    /* Free up buffers. */
    free_matrix(Z_input);
    free_matrix(Z_test);
    free_vector(D_test);
    free_vector(E_test);
    free_matrix(A);
    free_vector(D);
    free_vector(E);
}

void prepare_stedc_run(char *compz, integer n, void *D, void *E, void *Z, integer ldz,
                       integer datatype, integer n_repeats, double *time_min_, integer *info)
{
    integer index, lwork, liwork, lrwork, realtype;
    void *D_save = NULL, *E_save = NULL, *E_test = NULL, *Z_save = NULL;
    void *work = NULL, *iwork = NULL, *rwork = NULL;
    double time_min = 1e9, exe_time;

    /* Make a copy of the input matrices. Same input values will be passed in
       each itertaion.*/
    if(*compz == 'V')
    {
        create_matrix(datatype, &Z_save, ldz, n);
        copy_matrix(datatype, "full", n, n, Z, ldz, Z_save, ldz);
    }
    realtype = get_realtype(datatype);
    create_vector(realtype, &D_save, n);
    copy_vector(realtype, n, D, 1, D_save, 1);
    create_vector(realtype, &E_save, n - 1);
    copy_vector(realtype, n - 1, E, 1, E_save, 1);

    /* Call to STEDC() API to get work buffers size. */
    if(g_lwork == -1 || g_liwork == -1
       || ((datatype == COMPLEX || datatype == DOUBLE_COMPLEX) && g_lrwork == -1))
    {
        /* Make a workspace query the first time. This will provide us with
        and ideal workspace size based on internal block size.*/
        create_vector(datatype, &work, 1);
        create_vector(realtype, &rwork, 1);
        create_vector(INTEGER, &iwork, 1);
        lwork = g_lwork;
        liwork = g_liwork;
        lrwork = g_lrwork;
        invoke_stedc(datatype, compz, &n, D, E_test, Z, &ldz, work, &lwork, rwork, &lrwork, iwork,
                     &liwork, info);

        /* Get work buffers size. */
        if(*info == 0)
        {
            if(g_lwork == -1)
            {
                lwork = get_work_value(datatype, work);
            }
            if(g_liwork == -1)
            {
                liwork = get_work_value(INTEGER, iwork);
            }
            if((datatype == COMPLEX || datatype == DOUBLE_COMPLEX) && g_lrwork == -1)
            {
                lrwork = get_work_value(realtype, rwork);
            }
        }

        free_vector(work);
        free_vector(rwork);
        free_vector(iwork);
    }
    else
    {
        lwork = g_lwork;
        liwork = g_liwork;
        lrwork = g_lrwork;
    }
    create_vector(datatype, &work, lwork);
    if((datatype == COMPLEX) || (datatype == DOUBLE_COMPLEX))
    {
        create_vector(realtype, &rwork, lrwork);
    }
    create_vector(INTEGER, &iwork, liwork);
    create_vector(realtype, &E_test, n - 1);

    *info = 0;
    for(index = 0; index < n_repeats && *info == 0; ++index)
    {
        /* Restore input matrices and allocate memory to output buffers
           for each iteration. */
        if(*compz == 'V')
        {
            copy_matrix(datatype, "full", n, n, Z_save, ldz, Z, ldz);
        }
        copy_vector(realtype, n, D_save, 1, D, 1);
        copy_vector(realtype, n - 1, E_save, 1, E_test, 1);
        reset_vector(datatype, work, lwork, 1);
        if((datatype == COMPLEX) || (datatype == DOUBLE_COMPLEX))
        {
            reset_vector(realtype, rwork, lrwork, 1);
        }
        reset_vector(INTEGER, iwork, liwork, 1);

        exe_time = fla_test_clock();

        /* Call to STEDC() API. */
        invoke_stedc(datatype, compz, &n, D, E_test, Z, &ldz, work, &lwork, rwork, &lrwork, iwork,
                     &liwork, info);

        exe_time = fla_test_clock() - exe_time;

        /* Get the best execution time. */
        time_min = fla_min(time_min, exe_time);
    }
    *time_min_ = time_min;

    /* Free up buffers. */
    if(*compz == 'V')
    {
        free_vector(Z_save);
    }
    free_vector(D_save);
    free_vector(E_save);
    free_vector(E_test);
    free_vector(work);
    if((datatype == COMPLEX) || (datatype == DOUBLE_COMPLEX))
    {
        free_vector(rwork);
    }
    free_vector(iwork);
}

void invoke_stedc(integer datatype, char *compz, integer *n, void *D, void *E, void *Z,
                  integer *ldz, void *work, integer *lwork, void *rwork, integer *lrwork,
                  integer *iwork, integer *liwork, integer *info)
{
    switch(datatype)
    {
        case FLOAT:
        {
            fla_lapack_sstedc(compz, n, D, E, Z, ldz, work, lwork, iwork, liwork, info);
            break;
        }
        case DOUBLE:
        {
            fla_lapack_dstedc(compz, n, D, E, Z, ldz, work, lwork, iwork, liwork, info);
            break;
        }
        case COMPLEX:
        {
            fla_lapack_cstedc(compz, n, D, E, Z, ldz, work, lwork, rwork, lrwork, iwork, liwork,
                              info);
            break;
        }
        case DOUBLE_COMPLEX:
        {
            fla_lapack_zstedc(compz, n, D, E, Z, ldz, work, lwork, rwork, lrwork, iwork, liwork,
                              info);
            break;
        }
    }
}