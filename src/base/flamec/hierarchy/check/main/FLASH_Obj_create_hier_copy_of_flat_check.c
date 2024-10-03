/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLASH_Obj_create_hier_copy_of_flat_check( FLA_Obj F, fla_dim_t depth, fla_dim_t* blocksizes, FLA_Obj* H )
{
  FLA_Error e_val;

  e_val = FLA_Check_null_pointer( blocksizes );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_null_pointer( H );
  FLA_Check_error_code( e_val );

  // A value of depth < 0 should cause an error.

  // First depth entries in blocksize should be checked; values < 1 should cause error.

  return FLA_SUCCESS;
}

