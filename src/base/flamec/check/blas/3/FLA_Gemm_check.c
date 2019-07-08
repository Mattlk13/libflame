/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_THREAD_SAFE_INTERFACES
FLA_Error FLA_Gemm_check_ts( FLA_cntl_init_s *FLA_cntl_init_i, FLA_Trans transa, FLA_Trans transb, FLA_Obj alpha, FLA_Obj A, FLA_Obj B, FLA_Obj beta, FLA_Obj C )
{
  FLA_Error e_val;

  e_val = FLA_Check_valid_trans( transa );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_valid_trans( transb );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_floating_object_ts( FLA_cntl_init_i, A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object_ts( FLA_cntl_init_i, A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_datatype_ts( FLA_cntl_init_i, A, B );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_datatype_ts( FLA_cntl_init_i, A, C );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_consistent_object_datatype_ts( FLA_cntl_init_i, A, alpha );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_consistent_object_datatype_ts( FLA_cntl_init_i, A, beta );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_scalar( alpha );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_scalar( beta );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_matrix_matrix_dims( transa, transb, A, B, C );
  FLA_Check_error_code( e_val );

  return FLA_SUCCESS;
}
#endif

FLA_Error FLA_Gemm_check( FLA_Trans transa, FLA_Trans transb, FLA_Obj alpha, FLA_Obj A, FLA_Obj B, FLA_Obj beta, FLA_Obj C )
{
  FLA_Error e_val;

  e_val = FLA_Check_valid_trans( transa );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_valid_trans( transb );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_floating_object( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_datatype( A, B );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_datatype( A, C );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_consistent_object_datatype( A, alpha );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_consistent_object_datatype( A, beta );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_scalar( alpha );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_scalar( beta );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_matrix_matrix_dims( transa, transb, A, B, C );
  FLA_Check_error_code( e_val );

  return FLA_SUCCESS;
}

