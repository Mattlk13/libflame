/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_NON_CRITICAL_CODE

FLA_Error FLA_Trsm_llt_blk_var1( FLA_Diag diagA, FLA_Obj alpha, FLA_Obj A, FLA_Obj B, fla_trsm_t* cntl )
{
  FLA_Obj ATL,   ATR,      A00, A01, A02, 
          ABL,   ABR,      A10, A11, A12,
                           A20, A21, A22;

  FLA_Obj BT,              B0,
          BB,              B1,
                           B2;

  fla_dim_t b;

  FLA_Scal_internal( alpha, B,
                     FLA_Cntl_sub_scal( cntl ) );

  FLA_Part_2x2( A,    &ATL, &ATR,
                      &ABL, &ABR,     0, 0, FLA_BR );

  FLA_Part_2x1( B,    &BT, 
                      &BB,            0, FLA_BOTTOM );

  while ( FLA_Obj_length( ABR ) < FLA_Obj_length( A ) ){

    b = FLA_Determine_blocksize( ATL, FLA_TL, FLA_Cntl_blocksize( cntl ) );

    FLA_Repart_2x2_to_3x3( ATL, /**/ ATR,       &A00, &A01, /**/ &A02,
                                                &A10, &A11, /**/ &A12,
                        /* ************* */   /* ******************** */
                           ABL, /**/ ABR,       &A20, &A21, /**/ &A22,
                           b, b, FLA_TL );

    FLA_Repart_2x1_to_3x1( BT,                &B0, 
                                              &B1, 
                        /* ** */            /* ** */
                           BB,                &B2,        b, FLA_TOP );

    /*------------------------------------------------------------*/

    /* B1 = B1 - A21' * B2; */
    FLA_Gemm_internal( FLA_TRANSPOSE, FLA_NO_TRANSPOSE, 
                       FLA_MINUS_ONE, A21, B2, FLA_ONE, B1,
                       FLA_Cntl_sub_gemm( cntl ) );

    /* B1 = tril( A11' ) \ B1; */
    FLA_Trsm_internal( FLA_LEFT, FLA_LOWER_TRIANGULAR, FLA_TRANSPOSE, diagA,
                       FLA_ONE, A11, B1,
                       FLA_Cntl_sub_trsm( cntl ) );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &ATL, /**/ &ATR,       A00, /**/ A01, A02,
                            /* ************** */  /* ****************** */
                                                     A10, /**/ A11, A12,
                              &ABL, /**/ &ABR,       A20, /**/ A21, A22,
                              FLA_BR );

    FLA_Cont_with_3x1_to_2x1( &BT,                B0, 
                            /* ** */           /* ** */
                                                  B1, 
                              &BB,                B2,     FLA_BOTTOM );

  }

  return FLA_SUCCESS;
}

#endif
