/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Tridiag_UT_realify_subdiagonal_check( FLA_Obj b, FLA_Obj d )
{
  FLA_Error e_val;
  fla_dim_t     m_d;

  e_val = FLA_Check_floating_object( d );
  FLA_Check_error_code( e_val );

  e_val = FLA_Check_if_vector( d );
  FLA_Check_error_code( e_val );

  m_d = FLA_Obj_vector_dim( d );
  
  if ( m_d > 1 ) 
  {
    e_val = FLA_Check_identical_object_datatype( b, d );
    FLA_Check_error_code( e_val );

    e_val = FLA_Check_if_vector( b );
    FLA_Check_error_code( e_val );

    e_val = FLA_Check_vector_dim( b, m_d - 1 );
    FLA_Check_error_code( e_val );
  }

  return FLA_SUCCESS;
}

