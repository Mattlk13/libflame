/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Sort_svd_check( FLA_Direct direct, FLA_Obj s, FLA_Obj U, FLA_Obj V )
{
  FLA_Error e_val;

  e_val = FLA_Check_valid_direct( direct );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_real_object( s );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( s );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_floating_object( U );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( U );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_datatype( U, V );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_precision( s, U );
  FLA_Check_error_code( e_val );

  //e_val = FLA_Check_square( U );
  //FLA_Check_error_code( e_val );

  //e_val = FLA_Check_square( V );
  //FLA_Check_error_code( e_val );

  e_val = FLA_Check_vector_dim( s, fla_min( FLA_Obj_length( U ), FLA_Obj_length( V ) ) );
  FLA_Check_error_code( e_val );

  return FLA_SUCCESS;
}

