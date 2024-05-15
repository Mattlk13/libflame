/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_CAQR_UT_inc_copy_triangles( fla_dim_t nb_part, FLA_Obj A, FLA_Obj R )
{
  FLA_Obj AT,              A0, 
          AB,              A1,
                           A2;

  FLA_Obj RT,              R0, 
          RB,              R1,
                           R2;

  fla_dim_t b;

  FLA_Part_2x1( A,    &AT, 
                      &AB,            0, FLA_TOP );

  FLA_Part_2x1( R,    &RT, 
                      &RB,            0, FLA_TOP );

  while ( FLA_Obj_length( AB ) > 0 ){

    b = fla_min( nb_part, FLA_Obj_length( AB ) );

    FLA_Repart_2x1_to_3x1( AT,                &A0, 
                        /* ** */            /* ** */
                                              &A1, 
                           AB,                &A2,        b, FLA_BOTTOM );

    FLA_Repart_2x1_to_3x1( RT,                &R0, 
                        /* ** */             /* ** */
                                              &R1, 
                           RB,                &R2,        b, FLA_BOTTOM );

    /*------------------------------------------------------------*/

	// Copy the individual upper triangles in A into R.
    FLASH_Copyr( FLA_UPPER_TRIANGULAR, A1, R1 );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x1_to_2x1( &AT,               A0, 
                                                 A1, 
                            /* ** */          /* ** */
                              &AB,               A2,      FLA_TOP );

    FLA_Cont_with_3x1_to_2x1( &RT,               R0, 
                                                 R1, 
                            /* ** */           /* ** */
                              &RB,               R2,      FLA_TOP );
  }

  return FLA_SUCCESS;
}

