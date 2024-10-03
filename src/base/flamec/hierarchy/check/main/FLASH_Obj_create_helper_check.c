/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLASH_Obj_create_helper_check( FLA_Bool without_buffer, FLA_Datatype datatype, fla_dim_t m, fla_dim_t n, fla_dim_t depth, fla_dim_t* b_m, fla_dim_t* b_n, FLA_Obj* H )
{
  FLA_Error e_val;

  e_val = FLA_Check_valid_datatype( datatype );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_null_pointer( b_m );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_null_pointer( b_n );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_null_pointer( H );
  FLA_Check_error_code( e_val );

  // A value of depth < 0 should cause an error.

  // Values of m < 1, n < 1 should cause an error. (or < 0?)

  // First depth entries in blocksize_m, _n should be checked; values < 1 should cause error.

  return FLA_SUCCESS;
}

