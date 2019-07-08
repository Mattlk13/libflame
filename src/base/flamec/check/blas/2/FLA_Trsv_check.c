/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_THREAD_SAFE_INTERFACES
FLA_Error FLA_Trsv_check_ts( FLA_cntl_init_s *FLA_cntl_init_i, FLA_Uplo uplo, FLA_Trans transa, FLA_Diag diag, FLA_Obj A, FLA_Obj x ) 
{
  FLA_Error e_val;

  e_val = FLA_Check_valid_uplo( uplo );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_valid_trans( transa );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_valid_diag( diag );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_floating_object_ts( FLA_cntl_init_i, A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object_ts( FLA_cntl_init_i, A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_datatype_ts( FLA_cntl_init_i, A, x );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_square( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_vector( x );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_matrix_vector_dims( FLA_NO_TRANSPOSE, A, x, x );
  FLA_Check_error_code( e_val );
 
  return FLA_SUCCESS;
}
#endif

FLA_Error FLA_Trsv_check( FLA_Uplo uplo, FLA_Trans transa, FLA_Diag diag, FLA_Obj A, FLA_Obj x ) 
{
  FLA_Error e_val;

  e_val = FLA_Check_valid_uplo( uplo );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_valid_trans( transa );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_valid_diag( diag );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_floating_object( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_datatype( A, x );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_square( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_vector( x );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_matrix_vector_dims( FLA_NO_TRANSPOSE, A, x, x );
  FLA_Check_error_code( e_val );
 
  return FLA_SUCCESS;
}

