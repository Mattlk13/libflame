/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Axpyt_h_blk_var3( FLA_Obj alpha, FLA_Obj A, FLA_Obj B, fla_axpyt_t* cntl )
{
  FLA_Obj AT,              A0,
          AB,              A1,
                           A2;

  FLA_Obj BL,    BR,       B0,  B1,  B2;

  fla_dim_t b;

  FLA_Part_2x1( A,    &AT, 
                      &AB,            0, FLA_TOP );

  FLA_Part_1x2( B,    &BL,  &BR,      0, FLA_LEFT );

  while ( FLA_Obj_length( AT ) < FLA_Obj_length( A ) ){

    b = FLA_Determine_blocksize( AB, FLA_BOTTOM, FLA_Cntl_blocksize( cntl ) );

    FLA_Repart_2x1_to_3x1( AT,                &A0, 
                        /* ** */            /* ** */
                                              &A1, 
                           AB,                &A2,        b, FLA_BOTTOM );

    FLA_Repart_1x2_to_1x3( BL,  /**/ BR,        &B0, /**/ &B1, &B2,
                           b, FLA_RIGHT );

    /*------------------------------------------------------------*/

    FLA_Axpyt_internal( FLA_CONJ_TRANSPOSE, alpha, A1, B1,
                        FLA_Cntl_sub_axpyt( cntl ) );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x1_to_2x1( &AT,                A0, 
                                                  A1, 
                            /* ** */           /* ** */
                              &AB,                A2,     FLA_TOP );

    FLA_Cont_with_1x3_to_1x2( &BL,  /**/ &BR,        B0, B1, /**/ B2,
                              FLA_LEFT );
  }

  return FLA_SUCCESS;
}

