/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Copy_buffer_to_object( FLA_Trans trans, fla_dim_t m, fla_dim_t n, void* A_buffer, fla_dim_t rs, fla_dim_t cs, fla_dim_t i, fla_dim_t j, FLA_Obj B )
{
  FLA_Obj  A;
  FLA_Obj  BTL, BTR, 
           BBL, Bij;

  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Copy_buffer_to_object_check( trans, m, n, A_buffer, rs, cs, i, j, B );

  FLA_Part_2x2( B,  &BTL, &BTR,
                    &BBL, &Bij,     i, j, FLA_TL );

  FLA_Obj_create_without_buffer( FLA_Obj_datatype( B ), m, n, &A );
  FLA_Obj_attach_buffer( A_buffer, rs, cs, &A );

  FLA_Copyt_external( trans, A, Bij );

  FLA_Obj_free_without_buffer( &A );

  return FLA_SUCCESS;
}



FLA_Error FLA_Copy_object_to_buffer( FLA_Trans trans, fla_dim_t i, fla_dim_t j, FLA_Obj A, fla_dim_t m, fla_dim_t n, void* B_buffer, fla_dim_t rs, fla_dim_t cs )
{
  FLA_Obj  B;
  FLA_Obj  ATL, ATR, 
           ABL, Aij;

  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Copy_object_to_buffer_check( trans, i, j, A, m, n, B_buffer, rs, cs );

  FLA_Part_2x2( A,  &ATL, &ATR,
                    &ABL, &Aij,     i, j, FLA_TL );

  FLA_Obj_create_without_buffer( FLA_Obj_datatype( A ), m, n, &B );
  FLA_Obj_attach_buffer( B_buffer, rs, cs, &B );

  FLA_Copyt_external( trans, Aij, B );

  FLA_Obj_free_without_buffer( &B );

  return FLA_SUCCESS;
}

