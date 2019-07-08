/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

double             fla_chol_var3_in_to_ou_bsize_ratio = 0.25;

#ifdef FLA_ENABLE_THREAD_SAFE_INTERFACES
void FLA_Chol_cntl_init_ts(FLA_Cntl_init_flamec_s *FLA_cntl_flamec_init_i)
{
	// Set blocksize with default values for conventional storage.
	FLA_cntl_flamec_init_i->fla_chol_var3_bsize  = FLA_Query_blocksizes( FLA_DIMENSION_MIN );
	FLA_cntl_flamec_init_i->fla_chol_var3_bsize_in = FLA_Blocksize_create_copy_ts( FLA_cntl_flamec_init_i, FLA_cntl_flamec_init_i->fla_chol_var3_bsize );
	FLA_Blocksize_scale_ts( FLA_cntl_flamec_init_i, FLA_cntl_flamec_init_i->fla_chol_var3_bsize_in, fla_chol_var3_in_to_ou_bsize_ratio );

	// Create a control tree to invoke LAPACK.
	FLA_cntl_flamec_init_i->fla_chol_cntl_leaf    = FLA_Cntl_chol_obj_create( FLA_FLAT,
#ifdef FLA_ENABLE_EXTERNAL_LAPACK_FOR_SUBPROBLEMS
	                                                  FLA_BLOCKED_EXTERN,
#else
                                                      FLA_UNB_OPT_VARIANT2,
#endif
                                                      NULL,
                                                      NULL,
                                                      NULL,
                                                      NULL,
                                                      NULL );

	// Create a control tree for small subproblems.
	FLA_cntl_flamec_init_i->fla_chol_cntl_in     = FLA_Cntl_chol_obj_create( FLA_FLAT,
	                                                 FLA_BLOCKED_VARIANT3, 
	                                                 FLA_cntl_flamec_init_i->fla_chol_var3_bsize_in,
	                                                 FLA_cntl_flamec_init_i->fla_chol_cntl_leaf,
	                                                 FLA_cntl_flamec_init_i->fla_herk_cntl_blas,
	                                                 FLA_cntl_flamec_init_i->fla_trsm_cntl_blas,
	                                                 NULL );

	// Create a control tree for larger problems with one level of recursion.
	FLA_cntl_flamec_init_i->fla_chol_cntl2       = FLA_Cntl_chol_obj_create( FLA_FLAT,
	                                                 FLA_BLOCKED_VARIANT3, 
	                                                 FLA_cntl_flamec_init_i->fla_chol_var3_bsize,
	                                                 FLA_cntl_flamec_init_i->fla_chol_cntl_in,
	                                                 FLA_cntl_flamec_init_i->fla_herk_cntl_blas,
	                                                 FLA_cntl_flamec_init_i->fla_trsm_cntl_blas,
	                                                 NULL );

	// Create a control tree for large problems with no extra recursion.
	FLA_cntl_flamec_init_i->fla_chol_cntl        = FLA_Cntl_chol_obj_create( FLA_FLAT,
	                                                 FLA_BLOCKED_VARIANT3, 
	                                                 FLA_cntl_flamec_init_i->fla_chol_var3_bsize,
	                                                 FLA_cntl_flamec_init_i->fla_chol_cntl_leaf,
	                                                 FLA_cntl_flamec_init_i->fla_herk_cntl_blas,
	                                                 FLA_cntl_flamec_init_i->fla_trsm_cntl_blas,
	                                                 NULL );
}

void FLA_Chol_cntl_finalize_ts(FLA_Cntl_init_flamec_s *FLA_cntl_flamec_init_i)
{
	FLA_Cntl_obj_free( FLA_cntl_flamec_init_i->fla_chol_cntl );
	FLA_Cntl_obj_free( FLA_cntl_flamec_init_i->fla_chol_cntl2 );
	FLA_Cntl_obj_free( FLA_cntl_flamec_init_i->fla_chol_cntl_leaf );
	FLA_Cntl_obj_free( FLA_cntl_flamec_init_i->fla_chol_cntl_in );

	FLA_Blocksize_free( FLA_cntl_flamec_init_i->fla_chol_var3_bsize );
	FLA_Blocksize_free( FLA_cntl_flamec_init_i->fla_chol_var3_bsize_in );
}
#endif

extern fla_herk_t* fla_herk_cntl_blas;
extern fla_trsm_t* fla_trsm_cntl_blas;

fla_chol_t*        fla_chol_cntl = NULL;
fla_chol_t*        fla_chol_cntl2 = NULL;

fla_chol_t*        fla_chol_cntl_in = NULL;
fla_chol_t*        fla_chol_cntl_leaf = NULL;
fla_blocksize_t*   fla_chol_var3_bsize = NULL;
fla_blocksize_t*   fla_chol_var3_bsize_in = NULL;

void FLA_Chol_cntl_init()
{
	// Set blocksize with default values for conventional storage.
	fla_chol_var3_bsize  = FLA_Query_blocksizes( FLA_DIMENSION_MIN );
	fla_chol_var3_bsize_in = FLA_Blocksize_create_copy( fla_chol_var3_bsize );
	FLA_Blocksize_scale( fla_chol_var3_bsize_in, fla_chol_var3_in_to_ou_bsize_ratio );

	// Create a control tree to invoke LAPACK.
	fla_chol_cntl_leaf    = FLA_Cntl_chol_obj_create( FLA_FLAT,
#ifdef FLA_ENABLE_EXTERNAL_LAPACK_FOR_SUBPROBLEMS
	                                                  FLA_BLOCKED_EXTERN,
#else
                                                      FLA_UNB_OPT_VARIANT2,
#endif
                                                      NULL,
                                                      NULL,
                                                      NULL,
                                                      NULL,
                                                      NULL );

	// Create a control tree for small subproblems.
	fla_chol_cntl_in     = FLA_Cntl_chol_obj_create( FLA_FLAT,
	                                                 FLA_BLOCKED_VARIANT3, 
	                                                 fla_chol_var3_bsize_in,
	                                                 fla_chol_cntl_leaf,
	                                                 fla_herk_cntl_blas,
	                                                 fla_trsm_cntl_blas,
	                                                 NULL );

	// Create a control tree for larger problems with one level of recursion.
	fla_chol_cntl2       = FLA_Cntl_chol_obj_create( FLA_FLAT,
	                                                 FLA_BLOCKED_VARIANT3, 
	                                                 fla_chol_var3_bsize,
	                                                 fla_chol_cntl_in,
	                                                 fla_herk_cntl_blas,
	                                                 fla_trsm_cntl_blas,
	                                                 NULL );

	// Create a control tree for large problems with no extra recursion.
	fla_chol_cntl        = FLA_Cntl_chol_obj_create( FLA_FLAT,
	                                                 FLA_BLOCKED_VARIANT3, 
	                                                 fla_chol_var3_bsize,
	                                                 fla_chol_cntl_leaf,
	                                                 fla_herk_cntl_blas,
	                                                 fla_trsm_cntl_blas,
	                                                 NULL );
}

void FLA_Chol_cntl_finalize()
{
	FLA_Cntl_obj_free( fla_chol_cntl );
	FLA_Cntl_obj_free( fla_chol_cntl2 );
	FLA_Cntl_obj_free( fla_chol_cntl_leaf );
	FLA_Cntl_obj_free( fla_chol_cntl_in );

	FLA_Blocksize_free( fla_chol_var3_bsize );
	FLA_Blocksize_free( fla_chol_var3_bsize_in );
}

