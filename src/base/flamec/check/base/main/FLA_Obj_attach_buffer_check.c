/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Obj_attach_buffer_check( void *buffer, fla_dim_t rs, fla_dim_t cs, FLA_Obj *obj )
{
  FLA_Error e_val;

  e_val = FLA_Check_null_pointer( obj );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_matrix_strides( FLA_Obj_length( *obj ), FLA_Obj_width( *obj ), rs, cs );
  FLA_Check_error_code( e_val );

  return FLA_SUCCESS;
}

