/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_NON_CRITICAL_CODE

FLA_Error FLA_Sylv_nh_blk_var1( FLA_Obj isgn, FLA_Obj A, FLA_Obj B, FLA_Obj C, FLA_Obj scale, fla_sylv_t* cntl )
{
  FLA_Obj ATL,   ATR,      A00, A01, A02, 
          ABL,   ABR,      A10, A11, A12,
                           A20, A21, A22;

  FLA_Obj BTL,   BTR,      B00, B01, B02, 
          BBL,   BBR,      B10, B11, B12,
                           B20, B21, B22;

  FLA_Obj CTL,   CTR,      C00, C01, C02, 
          CBL,   CBR,      C10, C11, C12,
                           C20, C21, C22;

  fla_dim_t b;

  FLA_Part_2x2( A,    &ATL, &ATR,
                      &ABL, &ABR,     0, 0, FLA_BR );

  FLA_Part_2x2( B,    &BTL, &BTR,
                      &BBL, &BBR,     0, 0, FLA_BR );

  FLA_Part_2x2( C,    &CTL, &CTR,
                      &CBL, &CBR,     0, 0, FLA_BR );

  while ( FLA_Obj_length( ABR ) < FLA_Obj_length( A ) ){

    b = FLA_Determine_blocksize( CTL, FLA_TL, FLA_Cntl_blocksize( cntl ) );

    FLA_Repart_2x2_to_3x3( ATL, /**/ ATR,       &A00, &A01, /**/ &A02,
                                                &A10, &A11, /**/ &A12,
                        /* ************* */   /* ******************** */
                           ABL, /**/ ABR,       &A20, &A21, /**/ &A22,
                           b, b, FLA_TL );

    FLA_Repart_2x2_to_3x3( BTL, /**/ BTR,       &B00, &B01, /**/ &B02,
                                                &B10, &B11, /**/ &B12,
                        /* ************* */   /* ******************** */
                           BBL, /**/ BBR,       &B20, &B21, /**/ &B22,
                           b, b, FLA_TL );

    FLA_Repart_2x2_to_3x3( CTL, /**/ CTR,       &C00, &C01, /**/ &C02,
                                                &C10, &C11, /**/ &C12,
                        /* ************* */   /* ******************** */
                           CBL, /**/ CBR,       &C20, &C21, /**/ &C22,
                           b, b, FLA_TL );

    // Loop Invariant:
    // CTL = 
    // CTR = 
    // CBL = 
    // CBR = 

    /*------------------------------------------------------------*/

    // C12 = sylv( A11, B22', C12 - A12 * C22 );
    FLA_Gemm_internal( FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE,
                       FLA_MINUS_ONE, A12, C22, FLA_ONE, C12,
                       FLA_Cntl_sub_gemm1( cntl ) );

    FLA_Sylv_internal( FLA_NO_TRANSPOSE, FLA_CONJ_TRANSPOSE, 
                       isgn, A11, B22, C12, scale,
                       FLA_Cntl_sub_sylv1( cntl ) );

    // C21 = sylv( A22, B11', C21 -/+ C22 * B12' );
    FLA_Gemm_internal( FLA_NO_TRANSPOSE, FLA_CONJ_TRANSPOSE,
                       FLA_NEGATE( isgn ), C22, B12, FLA_ONE, C21,
                       FLA_Cntl_sub_gemm2( cntl ) );

    FLA_Sylv_internal( FLA_NO_TRANSPOSE, FLA_CONJ_TRANSPOSE, 
                       isgn, A22, B11, C21, scale,
                       FLA_Cntl_sub_sylv2( cntl ) );

    // C11 = sylv( A11, B11', C11 - A12 * C21 -/+ C12 * B12' );
    FLA_Gemm_internal( FLA_NO_TRANSPOSE, FLA_CONJ_TRANSPOSE,
                       FLA_NEGATE( isgn ), C12, B12, FLA_ONE, C11,
                       FLA_Cntl_sub_gemm3( cntl ) );

    FLA_Gemm_internal( FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE,
                       FLA_MINUS_ONE, A12, C21, FLA_ONE, C11,
                       FLA_Cntl_sub_gemm4( cntl ) );

    FLA_Sylv_internal( FLA_NO_TRANSPOSE, FLA_CONJ_TRANSPOSE, 
                       isgn, A11, B11, C11, scale,
                       FLA_Cntl_sub_sylv3( cntl ) );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &ATL, /**/ &ATR,       A00, /**/ A01, A02,
                            /* ************** */  /* ****************** */
                                                     A10, /**/ A11, A12,
                              &ABL, /**/ &ABR,       A20, /**/ A21, A22,
                              FLA_BR );

    FLA_Cont_with_3x3_to_2x2( &BTL, /**/ &BTR,       B00, /**/ B01, B02,
                            /* ************** */  /* ****************** */
                                                     B10, /**/ B11, B12,
                              &BBL, /**/ &BBR,       B20, /**/ B21, B22,
                              FLA_BR );

    FLA_Cont_with_3x3_to_2x2( &CTL, /**/ &CTR,       C00, /**/ C01, C02,
                            /* ************** */  /* ****************** */
                                                     C10, /**/ C11, C12,
                              &CBL, /**/ &CBR,       C20, /**/ C21, C22,
                              FLA_BR );

  }

  return FLA_SUCCESS;
}

#endif
