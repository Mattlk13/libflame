/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Hevd( FLA_Evd_type jobz, FLA_Uplo uplo, FLA_Obj A, FLA_Obj l )
{
  FLA_Error r_val      = FLA_SUCCESS;
  fla_dim_t     n_iter_max = 30;
  fla_dim_t     k_accum    = 32;
  fla_dim_t     b_alg      = 512;

  // Check parameters.
  if ( FLA_Check_error_level() >= FLA_MIN_ERROR_CHECKING )
    FLA_Hevd_check( jobz, uplo, A, l );

  // Invoke FLA_Hevd_external() for now.
  if ( jobz == FLA_EVD_WITH_VECTORS )
  {
    if ( uplo == FLA_LOWER_TRIANGULAR )
    {
      r_val = FLA_Hevd_lv_unb_var1( n_iter_max, A, l, k_accum, b_alg );
    }
    else // if ( uplo == FLA_UPPER_TRIANGULAR )
    {
      FLA_Check_error_code( FLA_NOT_YET_IMPLEMENTED );
    }
  }
  else // if ( jobz == FLA_EVD_WITHOUT_VECTORS )
  {
    if ( uplo == FLA_LOWER_TRIANGULAR )
    {
      FLA_Check_error_code( FLA_NOT_YET_IMPLEMENTED );
    }
    else // if ( uplo == FLA_UPPER_TRIANGULAR )
    {
      FLA_Check_error_code( FLA_NOT_YET_IMPLEMENTED );
    }
  }

  return r_val;
}

