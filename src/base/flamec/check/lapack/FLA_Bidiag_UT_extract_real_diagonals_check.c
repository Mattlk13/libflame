/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Bidiag_UT_extract_real_diagonals_check( FLA_Obj A, FLA_Obj d, FLA_Obj e )
{
  FLA_Error e_val;
  fla_dim_t     min_m_n;

  e_val = FLA_Check_floating_object( A );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_nonconstant_object( A );
  FLA_Check_error_code( e_val );

  min_m_n = FLA_Obj_min_dim( A );

  e_val = FLA_Check_nonconstant_object( d );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_real_object( d );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_identical_object_precision( A, d );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_vector( d );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_vector_dim( d, min_m_n );
  FLA_Check_error_code( e_val );

  if ( min_m_n != 1 ) 
  {
    e_val = FLA_Check_nonconstant_object( e );
    FLA_Check_error_code( e_val );

    e_val = FLA_Check_real_object( e );
    FLA_Check_error_code( e_val );

    e_val = FLA_Check_identical_object_precision( A, e );
    FLA_Check_error_code( e_val );
    
    e_val = FLA_Check_if_vector( e );
    FLA_Check_error_code( e_val );
    
    e_val = FLA_Check_vector_dim( e, min_m_n - 1 );
    FLA_Check_error_code( e_val );
  }

  return FLA_SUCCESS;
}

