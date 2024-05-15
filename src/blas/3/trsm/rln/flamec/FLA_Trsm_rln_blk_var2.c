/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Trsm_rln_blk_var2( FLA_Diag diagA, FLA_Obj alpha, FLA_Obj A, FLA_Obj B, fla_trsm_t* cntl )
{
  FLA_Obj ATL,   ATR,      A00, A01, A02, 
          ABL,   ABR,      A10, A11, A12,
                           A20, A21, A22;

  FLA_Obj BL,    BR,       B0,  B1,  B2;

  fla_dim_t b;

  FLA_Scal_internal( alpha, B,
                     FLA_Cntl_sub_scal( cntl ) );

  FLA_Part_2x2( A,    &ATL, &ATR,
                      &ABL, &ABR,     0, 0, FLA_BR );

  FLA_Part_1x2( B,    &BL,  &BR,      0, FLA_RIGHT );

  while ( FLA_Obj_length( ABR ) < FLA_Obj_length( A ) ){

    b = FLA_Determine_blocksize( ATL, FLA_TL, FLA_Cntl_blocksize( cntl ) );

    FLA_Repart_2x2_to_3x3( ATL, /**/ ATR,       &A00, &A01, /**/ &A02,
                                                &A10, &A11, /**/ &A12,
                        /* ************* */   /* ******************** */
                           ABL, /**/ ABR,       &A20, &A21, /**/ &A22,
                           b, b, FLA_TL );

    FLA_Repart_1x2_to_1x3( BL,  /**/ BR,        &B0, &B1, /**/ &B2,
                           b, FLA_LEFT );

    /*------------------------------------------------------------*/

    /* B1 = B1 / tril( A11 ); */
    FLA_Trsm_internal( FLA_RIGHT, FLA_LOWER_TRIANGULAR, FLA_NO_TRANSPOSE, diagA,
                       FLA_ONE, A11, B1,
                       FLA_Cntl_sub_trsm( cntl ) );

    /* B0 = B0 - B1 * A10; */
    FLA_Gemm_internal( FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, 
                       FLA_MINUS_ONE, B1, A10, FLA_ONE, B0,
                       FLA_Cntl_sub_gemm( cntl ) );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &ATL, /**/ &ATR,       A00, /**/ A01, A02,
                            /* ************** */  /* ****************** */
                                                     A10, /**/ A11, A12,
                              &ABL, /**/ &ABR,       A20, /**/ A21, A22,
                              FLA_BR );

    FLA_Cont_with_1x3_to_1x2( &BL,  /**/ &BR,        B0, /**/ B1, B2,
                              FLA_RIGHT );

  }

  return FLA_SUCCESS;
}

