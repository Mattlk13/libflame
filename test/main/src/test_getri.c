/*
    Copyright (C) 2022-2024, Advanced Micro Devices, Inc. All rights reserved.
*/

#include "test_lapack.h"

#define GETRI_VL 0.1
#define GETRI_VU 10

/* Local prototypes */
void fla_test_getri_experiment(test_params_t *params, integer datatype, integer p_cur,
                               integer q_cur, integer pci, integer n_repeats, integer einfo,
                               double *perf, double *t, double *residual);
void prepare_getri_run(integer m_A, integer n_A, void *A, integer lda, integer *ipiv,
                       integer datatype, integer n_repeats, double *time_min_, integer *info);
void invoke_getri(integer datatype, integer *n, void *a, integer *lda, integer *ipiv, void *work,
                  integer *lwork, integer *info);
void fla_test_getri(integer argc, char **argv, test_params_t *params)
{
    char *op_str = "Inverse through LU factorization";
    char *front_str = "GETRI";
    integer tests_not_run = 1, invalid_dtype = 0, einfo = 0;
    params->imatrix_char = '\0';

    if(argc == 1)
    {
        g_lwork = -1;
        config_data = 1;
        fla_test_output_info("--- %s ---\n", op_str);
        fla_test_output_info("\n");
        fla_test_op_driver(front_str, SQUARE_INPUT, params, LIN, fla_test_getri_experiment);
        tests_not_run = 0;
    }
    if(argc == 8)
    {
        FLA_TEST_PARSE_LAST_ARG(argv[7]);
    }
    if(argc >= 7 && argc <= 8)
    {
        integer i, num_types, N;
        integer datatype, n_repeats;
        double perf, time_min, residual;
        char stype, type_flag[4] = {0};
        char *endptr;

        /* Parse the arguments */
        num_types = strlen(argv[2]);
        N = strtoimax(argv[3], &endptr, CLI_DECIMAL_BASE);
        params->lin_solver_paramslist[0].lda = strtoimax(argv[4], &endptr, CLI_DECIMAL_BASE);
        g_lwork = strtoimax(argv[5], &endptr, CLI_DECIMAL_BASE);

        n_repeats = strtoimax(argv[6], &endptr, CLI_DECIMAL_BASE);

        if(n_repeats > 0)
        {
            params->lin_solver_paramslist[0].solver_threshold = CLI_NORM_THRESH;

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
                fla_test_getri_experiment(params, datatype, N, N, 0, n_repeats, einfo, &perf,
                                          &time_min, &residual);
                /* Print the results */
                fla_test_print_status(front_str, stype, SQUARE_INPUT, N, N, residual,
                                      params->lin_solver_paramslist[0].solver_threshold, time_min,
                                      perf);
                tests_not_run = 0;
            }
        }
    }

    /* Print error messages */
    if(tests_not_run)
    {
        printf("\nIllegal arguments for getri\n");
        printf("./<EXE> getri <precisions - sdcz> <N> <LDA> <LWORK> <repeats>\n");
    }
    if(invalid_dtype)
    {
        printf("\nInvalid datatypes specified, choose valid datatypes from 'sdcz'\n\n");
    }
    if(g_ext_fptr != NULL)
    {
        fclose(g_ext_fptr);
        g_ext_fptr = NULL;
    }

    return;
}

void fla_test_getri_experiment(test_params_t *params, integer datatype, integer p_cur,
                               integer q_cur, integer pci, integer n_repeats, integer einfo,
                               double *perf, double *t, double *residual)
{
    integer n, lda, info = 0, vinfo = 0;
    void *IPIV = NULL, *A = NULL, *A_test = NULL, *s_test = NULL;
    char range = 'U';
    double time_min = 1e9;

    /* Determine the dimensions*/
    n = p_cur;
    lda = params->lin_solver_paramslist[pci].lda;
    *residual = params->lin_solver_paramslist[pci].solver_threshold;

    /* If leading dimensions = -1, set them to default value
       when inputs are from config files */
    if(config_data)
    {
        if(lda == -1)
        {
            lda = fla_max(1, n);
        }
    }

    /* Create the matrices for the current operation*/
    create_matrix(datatype, &A, lda, n);
    create_vector(INTEGER, &IPIV, n);
    create_realtype_vector(datatype, &s_test, n);

    /* Initialize the test matrices*/
    if(params->imatrix_char == NULL && g_ext_fptr == NULL)
    {
        /* Generate input matrix with condition number <= 10 */
        create_svd_matrix(datatype, range, n, n, A, lda, s_test, GETRI_VL, GETRI_VU, i_zero, i_zero,
                          info);
    }
    else
    {
        init_matrix(datatype, A, n, n, lda, g_ext_fptr, params->imatrix_char);
    }

    /* Save the original matrix*/
    create_matrix(datatype, &A_test, lda, n);
    copy_matrix(datatype, "full", n, n, A, lda, A_test, lda);

    /* call to API */
    prepare_getri_run(n, n, A_test, lda, IPIV, datatype, n_repeats, &time_min, &info);

    /* execution time */
    *t = time_min;

    /* performance computation */
    /* 2mn^2 - (2/3)n^3 flops */
    *perf
        = (double)((2.0 * n * n * n) - ((2.0 / 3.0) * n * n * n)) / time_min / FLOPS_PER_UNIT_PERF;
    if(datatype == COMPLEX || datatype == DOUBLE_COMPLEX)
        *perf *= 4.0;

    /* output validation */
    if(!params->imatrix_char && info == 0)
        validate_getri(n, n, A, A_test, lda, IPIV, datatype, residual, &vinfo);
    /* check for output matrix when inputs as extreme values */
    else if(FLA_EXTREME_CASE_TEST)
    {
        if(!check_extreme_value(datatype, n, n, A_test, lda, params->imatrix_char))
        {
            *residual = DBL_MAX;
        }
    }
    else
        FLA_TEST_CHECK_EINFO(residual, info, einfo);

    /* Free up the buffers */
    free_matrix(A);
    free_matrix(A_test);
    free_vector(IPIV);
    free_vector(s_test);
}

void prepare_getri_run(integer m_A, integer n_A, void *A, integer lda, integer *IPIV,
                       integer datatype, integer n_repeats, double *time_min_, integer *info)
{
    integer lwork;
    integer i;
    void *A_save, *work;
    double time_min = 1e9, exe_time;
    lwork = i_n_one;

    /* Save the original matrix */
    create_matrix(datatype, &A_save, lda, n_A);
    copy_matrix(datatype, "full", m_A, n_A, A, lda, A_save, lda);
    if(g_lwork <= 0)
    {
        lwork = -1;
        create_vector(datatype, &work, 1);

        // call to  getri API
        invoke_getri(datatype, &n_A, NULL, &lda, NULL, work, &lwork, info);
        if(*info == 0)
        {
            // Get work siz`e
            lwork = get_work_value(datatype, work);
        }

        /* Output buffers will be freshly allocated for each iterations, free up
        the current output buffers.*/
        free_vector(work);
    }
    else
    {
        lwork = g_lwork;
    }

    *info = 0;
    for(i = 0; i < n_repeats && *info == 0; ++i)
    {

        /* Copy original input data */
        copy_matrix(datatype, "full", m_A, n_A, A, lda, A_save, lda);
        // create work buffer
        create_matrix(datatype, &work, lwork, 1);
        exe_time = fla_test_clock();

        /*  call to API getrf to get AFACT */
        invoke_getrf(datatype, &m_A, &n_A, A_save, &lda, IPIV, info);

        /*  call  getri API with AFACT to get A INV */
        invoke_getri(datatype, &n_A, A_save, &lda, IPIV, work, &lwork, info);

        exe_time = fla_test_clock() - exe_time;

        /* Get the best execution time */
        time_min = fla_min(time_min, exe_time);
        // Free up the output buffers
        free_matrix(work);
    }

    *time_min_ = time_min;
    /*  Save the final result to A matrix*/
    copy_matrix(datatype, "full", m_A, n_A, A_save, lda, A, lda);
    free_matrix(A_save);
}

/*
 *  GETRI_API calls LAPACK interface of
 *  Singular value decomposition - gesvd
 *  */
void invoke_getri(integer datatype, integer *n, void *a, integer *lda, integer *ipiv, void *work,
                  integer *lwork, integer *info)
{
    switch(datatype)
    {
        case FLOAT:
        {
            fla_lapack_sgetri(n, a, lda, ipiv, work, lwork, info);
            break;
        }

        case DOUBLE:
        {
            fla_lapack_dgetri(n, a, lda, ipiv, work, lwork, info);
            break;
        }

        case COMPLEX:
        {
            fla_lapack_cgetri(n, a, lda, ipiv, work, lwork, info);
            break;
        }

        case DOUBLE_COMPLEX:
        {
            fla_lapack_zgetri(n, a, lda, ipiv, work, lwork, info);
            break;
        }
    }
}