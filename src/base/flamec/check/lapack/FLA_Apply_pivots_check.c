/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Apply_pivots_check( FLA_Side side, FLA_Trans trans, FLA_Obj p, FLA_Obj A )
{
  FLA_Error    e_val;

  e_val = FLA_Check_valid_leftright_side( side );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_valid_trans( trans );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_floating_object( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_int_object( p );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( p );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_vector( p );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( A );
  FLA_Check_error_code( e_val );

  if ( trans == FLA_NO_TRANSPOSE )
  {
    if ( side == FLA_RIGHT )
      FLA_Check_error_code( FLA_NOT_YET_IMPLEMENTED );
  }
  else if ( trans == FLA_TRANSPOSE )
  {
    if      ( side == FLA_LEFT )
      FLA_Check_error_code( FLA_NOT_YET_IMPLEMENTED );
    else if ( side == FLA_RIGHT )
      FLA_Check_error_code( FLA_NOT_YET_IMPLEMENTED );
  }

  return FLA_SUCCESS;
}

