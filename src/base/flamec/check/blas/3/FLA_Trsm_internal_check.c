/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_THREAD_SAFE_INTERFACES
FLA_Error FLA_Trsm_internal_check_ts( FLA_cntl_init_s *FLA_cntl_init_i, FLA_Side side, FLA_Uplo uplo, FLA_Trans transa, FLA_Diag diag, FLA_Obj alpha, FLA_Obj A, FLA_Obj B, fla_trsm_t* cntl )
{
	FLA_Error e_val;

	// Abort if the control structure is NULL.
	e_val = FLA_Check_null_pointer( ( void* ) cntl );
	FLA_Check_error_code( e_val );

	// Verify that the object element types are identical.
	e_val = FLA_Check_identical_object_elemtype_ts( FLA_cntl_init_i, A, B );
	FLA_Check_error_code( e_val );

	// Verify conformality between all the objects. This check works regardless
	// of whether the element type is FLA_MATRIX or FLA_SCALAR because the
	// element length and width are used instead of scalar length and width.
	if ( side == FLA_LEFT )
	{
		//e_val = FLA_Check_matrix_matrix_dims( FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, A, B, B );
		//FLA_Check_error_code( e_val );
		e_val = FLA_Check_object_length_equals( A, FLA_Obj_length( B ) );
		FLA_Check_error_code( e_val );
	}
	else
	{
		e_val = FLA_Check_matrix_matrix_dims( FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, B, A, B );
		FLA_Check_error_code( e_val );
	}

	return FLA_SUCCESS;
}
#endif

FLA_Error FLA_Trsm_internal_check( FLA_Side side, FLA_Uplo uplo, FLA_Trans transa, FLA_Diag diag, FLA_Obj alpha, FLA_Obj A, FLA_Obj B, fla_trsm_t* cntl )
{
	FLA_Error e_val;

	// Abort if the control structure is NULL.
	e_val = FLA_Check_null_pointer( ( void* ) cntl );
	FLA_Check_error_code( e_val );

	// Verify that the object element types are identical.
	e_val = FLA_Check_identical_object_elemtype( A, B );
	FLA_Check_error_code( e_val );

	// Verify conformality between all the objects. This check works regardless
	// of whether the element type is FLA_MATRIX or FLA_SCALAR because the
	// element length and width are used instead of scalar length and width.
	if ( side == FLA_LEFT )
	{
		//e_val = FLA_Check_matrix_matrix_dims( FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, A, B, B );
		//FLA_Check_error_code( e_val );
		e_val = FLA_Check_object_length_equals( A, FLA_Obj_length( B ) );
		FLA_Check_error_code( e_val );
	}
	else
	{
		e_val = FLA_Check_matrix_matrix_dims( FLA_NO_TRANSPOSE, FLA_NO_TRANSPOSE, B, A, B );
		FLA_Check_error_code( e_val );
	}

	return FLA_SUCCESS;
}

