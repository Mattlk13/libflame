/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Copy_check( FLA_Obj A, FLA_Obj B )
{
  FLA_Error e_val;

  e_val = FLA_Check_valid_object_datatype( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( B );
  FLA_Check_error_code( e_val );

  if ( FLA_Obj_is_vector( A ) && FLA_Obj_is_vector( B ) )
  {
    e_val = FLA_Check_equal_vector_dims( A, B );
    FLA_Check_error_code( e_val );
  }
  else
  {
    e_val = FLA_Check_conformal_dims( FLA_NO_TRANSPOSE, A, B );
    FLA_Check_error_code( e_val );
  }

  return FLA_SUCCESS;
}

