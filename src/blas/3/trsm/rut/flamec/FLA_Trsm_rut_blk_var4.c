/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_NON_CRITICAL_CODE

FLA_Error FLA_Trsm_rut_blk_var4( FLA_Diag diagA, FLA_Obj alpha, FLA_Obj A, FLA_Obj B, fla_trsm_t* cntl )
{
  FLA_Obj BT,              B0,
          BB,              B1,
                           B2;

  fla_dim_t b;

  FLA_Part_2x1( B,    &BT, 
                      &BB,            0, FLA_BOTTOM );

  while ( FLA_Obj_length( BB ) < FLA_Obj_length( B ) ){

    b = FLA_Determine_blocksize( BT, FLA_TOP, FLA_Cntl_blocksize( cntl ) );

    FLA_Repart_2x1_to_3x1( BT,                &B0, 
                                              &B1, 
                        /* ** */            /* ** */
                           BB,                &B2,        b, FLA_TOP );

    /*------------------------------------------------------------*/

    /* B1 = B1 / triu( A' ); */
    FLA_Trsm_internal( FLA_RIGHT, FLA_UPPER_TRIANGULAR, FLA_TRANSPOSE, diagA,
                       alpha, A, B1,
                       FLA_Cntl_sub_trsm( cntl ) );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x1_to_2x1( &BT,                B0, 
                            /* ** */           /* ** */
                                                  B1, 
                              &BB,                B2,     FLA_BOTTOM );

  }

  return FLA_SUCCESS;
}

#endif
