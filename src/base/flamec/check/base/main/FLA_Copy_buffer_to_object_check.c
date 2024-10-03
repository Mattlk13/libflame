/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Copy_buffer_to_object_check( FLA_Trans trans, fla_dim_t m, fla_dim_t n, void* A_buffer, fla_dim_t rs, fla_dim_t cs, fla_dim_t i, fla_dim_t j, FLA_Obj B )
{
  FLA_Error e_val;

  e_val = FLA_Check_valid_real_trans( trans );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_floating_object( B );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( B );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_null_pointer( A_buffer );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_object_dims( trans, m, n, B );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_matrix_strides( m, n, rs, cs );
  FLA_Check_error_code( e_val );

  if ( trans == FLA_NO_TRANSPOSE )
  {
    e_val = FLA_Check_submatrix_dims_and_offset( m, n, i, j, B );
    FLA_Check_error_code( e_val );
  }
  else
  {
    e_val = FLA_Check_submatrix_dims_and_offset( n, m, i, j, B );
    FLA_Check_error_code( e_val );
  }

  e_val = FLA_Check_nonconstant_object( B );
  FLA_Check_error_code( e_val );

  return FLA_SUCCESS;
}

