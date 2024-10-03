/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_NON_CRITICAL_CODE

FLA_Error FLA_Eig_gest_iu_blk_var5( FLA_Obj A, FLA_Obj Y, FLA_Obj B, fla_eig_gest_t* cntl )
{
  FLA_Obj ATL,   ATR,      A00, A01, A02,
          ABL,   ABR,      A10, A11, A12,
                           A20, A21, A22;

  FLA_Obj BTL,   BTR,      B00, B01, B02,
          BBL,   BBR,      B10, B11, B12,
                           B20, B21, B22;

  FLA_Obj YL,    YR,       Y10, Y11, Y12;

  FLA_Obj Y12_t,
          Y12_b;

  fla_dim_t b;

  FLA_Part_2x2( A,    &ATL, &ATR,
                      &ABL, &ABR,     0, 0, FLA_TL );

  FLA_Part_2x2( B,    &BTL, &BTR,
                      &BBL, &BBR,     0, 0, FLA_TL );

  FLA_Part_1x2( Y,    &YL,  &YR,      0, FLA_LEFT );

  while ( FLA_Obj_length( ATL ) < FLA_Obj_length( A ) ){

    b = FLA_Determine_blocksize( ABR, FLA_BR, FLA_Cntl_blocksize( cntl ) );

    FLA_Repart_2x2_to_3x3( ATL, /**/ ATR,       &A00, /**/ &A01, &A02,
                        /* ************* */   /* ******************** */
                                                &A10, /**/ &A11, &A12,
                           ABL, /**/ ABR,       &A20, /**/ &A21, &A22,
                           b, b, FLA_BR );

    FLA_Repart_2x2_to_3x3( BTL, /**/ BTR,       &B00, /**/ &B01, &B02,
                        /* ************* */   /* ******************** */
                                                &B10, /**/ &B11, &B12,
                           BBL, /**/ BBR,       &B20, /**/ &B21, &B22,
                           b, b, FLA_BR );

    FLA_Repart_1x2_to_1x3( YL,  /**/ YR,        &Y10, /**/ &Y11, &Y12,
                           b, FLA_RIGHT );

    /*------------------------------------------------------------*/

    FLA_Part_2x1( Y12,    &Y12_t,
                          &Y12_b,    b, FLA_TOP );

    // A11 = inv( triu( B11 )' ) * A11 * inv( triu( B11 ) );
    FLA_Eig_gest_internal( FLA_INVERSE, FLA_UPPER_TRIANGULAR,
                           A11, Y11, B11,
                           FLA_Cntl_sub_eig_gest( cntl ) );

    // Y12 = A11 * B12;
    FLA_Hemm_internal( FLA_LEFT, FLA_UPPER_TRIANGULAR,
                       FLA_ONE, A11, B12, FLA_ZERO, Y12_t,
                       FLA_Cntl_sub_hemm( cntl ) );

    // A12 = inv( triu( B11 )' ) * A12;
    FLA_Trsm_internal( FLA_LEFT, FLA_UPPER_TRIANGULAR,
                       FLA_CONJ_TRANSPOSE, FLA_NONUNIT_DIAG,
                       FLA_ONE, B11, A12,
                       FLA_Cntl_sub_trsm1( cntl ) );

    // A12 = A12 - 1/2 * Y12;
    FLA_Axpy_internal( FLA_MINUS_ONE_HALF, Y12_t, A12,
                       FLA_Cntl_sub_axpy1( cntl ) );

    // A22 = A22 - A12' * B12 - B12' * A12;
    FLA_Her2k_internal( FLA_UPPER_TRIANGULAR, FLA_CONJ_TRANSPOSE,
                        FLA_MINUS_ONE, A12, B12, FLA_ONE, A22,
                        FLA_Cntl_sub_her2k( cntl ) );

    // A12 = A12 - 1/2 * Y12;
    FLA_Axpy_internal( FLA_MINUS_ONE_HALF, Y12_t, A12,
                       FLA_Cntl_sub_axpy2( cntl ) );

    // A12 = A12 * inv( triu( B22 ) );
    FLA_Trsm_internal( FLA_RIGHT, FLA_UPPER_TRIANGULAR,
                       FLA_NO_TRANSPOSE, FLA_NONUNIT_DIAG,
                       FLA_ONE, B22, A12,
                       FLA_Cntl_sub_trsm2( cntl ) );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &ATL, /**/ &ATR,       A00, A01, /**/ A02,
                                                     A10, A11, /**/ A12,
                            /* ************** */  /* ****************** */
                              &ABL, /**/ &ABR,       A20, A21, /**/ A22,
                              FLA_TL );

    FLA_Cont_with_3x3_to_2x2( &BTL, /**/ &BTR,       B00, B01, /**/ B02,
                                                     B10, B11, /**/ B12,
                            /* ************** */  /* ****************** */
                              &BBL, /**/ &BBR,       B20, B21, /**/ B22,
                              FLA_TL );

    FLA_Cont_with_1x3_to_1x2( &YL,  /**/ &YR,        Y10, Y11, /**/ Y12,
                              FLA_LEFT );
  }

  return FLA_SUCCESS;
}

#endif
