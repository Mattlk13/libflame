/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Gemv_n_blk_var1( FLA_Obj alpha, FLA_Obj A, FLA_Obj x, FLA_Obj beta, FLA_Obj y, fla_gemv_t* cntl )
{
  FLA_Obj AT,              A0,
          AB,              A1,
                           A2;

  FLA_Obj yT,              y0,
          yB,              y1,
                           y2;

  fla_dim_t b;

  FLA_Part_2x1( A,    &AT, 
                      &AB,            0, FLA_TOP );

  FLA_Part_2x1( y,    &yT, 
                      &yB,            0, FLA_TOP );

  while ( FLA_Obj_length( AT ) < FLA_Obj_length( A ) ){

    b = FLA_Determine_blocksize( AB, FLA_BOTTOM, FLA_Cntl_blocksize( cntl ) );

    FLA_Repart_2x1_to_3x1( AT,                &A0, 
                        /* ** */            /* ** */
                                              &A1, 
                           AB,                &A2,        b, FLA_BOTTOM );

    FLA_Repart_2x1_to_3x1( yT,                &y0, 
                        /* ** */            /* ** */
                                              &y1, 
                           yB,                &y2,        b, FLA_BOTTOM );

    /*------------------------------------------------------------*/

    /*   y1 = alpha * A1 * x + y1;   */
    FLA_Gemv_internal( FLA_NO_TRANSPOSE, 
                       alpha, A1, x, beta, y1, 
                       FLA_Cntl_sub_gemv( cntl ) );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x1_to_2x1( &AT,                A0, 
                                                  A1, 
                            /* ** */           /* ** */
                              &AB,                A2,     FLA_TOP );

    FLA_Cont_with_3x1_to_2x1( &yT,                y0, 
                                                  y1, 
                            /* ** */           /* ** */
                              &yB,                y2,     FLA_TOP );

  }

  return FLA_SUCCESS;
}

