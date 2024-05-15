/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Gemm_nh_blk_var3( FLA_Obj alpha, FLA_Obj A, FLA_Obj B, FLA_Obj beta, FLA_Obj C, fla_gemm_t* cntl )
{
  FLA_Obj BT,              B0,
          BB,              B1,
                           B2;

  FLA_Obj CL,    CR,       C0,  C1,  C2;

  fla_dim_t b;

  FLA_Part_2x1( B,    &BT, 
                      &BB,            0, FLA_TOP );

  FLA_Part_1x2( C,    &CL,  &CR,      0, FLA_LEFT );

  while ( FLA_Obj_length( BT ) < FLA_Obj_length( B ) ){

    b = FLA_Determine_blocksize( BB, FLA_BOTTOM, FLA_Cntl_blocksize( cntl ) );

    FLA_Repart_2x1_to_3x1( BT,                &B0, 
                        /* ** */            /* ** */
                                              &B1, 
                           BB,                &B2,        b, FLA_BOTTOM );

    FLA_Repart_1x2_to_1x3( CL,  /**/ CR,        &C0, /**/ &C1, &C2,
                           b, FLA_RIGHT );

    /*------------------------------------------------------------*/

    /* C1 = alpha * A * B1' + C1; */
    FLA_Gemm_internal( FLA_NO_TRANSPOSE, FLA_CONJ_TRANSPOSE,
                       alpha, A, B1, beta, C1,
                       FLA_Cntl_sub_gemm( cntl ) );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x1_to_2x1( &BT,                B0, 
                                                  B1, 
                            /* ** */           /* ** */
                              &BB,                B2,     FLA_TOP );

    FLA_Cont_with_1x3_to_1x2( &CL,  /**/ &CR,        C0, C1, /**/ C2,
                              FLA_LEFT );

  }

  return FLA_SUCCESS;
}

