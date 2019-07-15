/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

extern __thread fla_scalr_t* flash_scalr_cntl;
extern __thread fla_gemm_t*  flash_gemm_cntl_pb_bb;

__thread fla_syr2k_t*        flash_syr2k_cntl_blas = NULL;
__thread fla_syr2k_t*        flash_syr2k_cntl_ip = NULL;
__thread fla_syr2k_t*        flash_syr2k_cntl_op = NULL;
__thread fla_syr2k_t*        flash_syr2k_cntl_mm = NULL;
__thread fla_blocksize_t*    flash_syr2k_bsize = NULL;

void FLASH_Syr2k_cntl_init()
{
	// Set syr2k blocksize for hierarchical storage.
	flash_syr2k_bsize      = FLA_Blocksize_create( 1, 1, 1, 1 );

	// Create a control tree that assumes A and B are b x b blocks.
	flash_syr2k_cntl_blas  = FLA_Cntl_syr2k_obj_create( FLA_HIER,
	                                                    FLA_SUBPROBLEM,
	                                                    NULL,
	                                                    NULL,
	                                                    NULL,
	                                                    NULL,
	                                                    NULL );

	// Create a control tree that assumes A and B form an inner panel product.
	flash_syr2k_cntl_ip    = FLA_Cntl_syr2k_obj_create( FLA_HIER,
	                                                    FLA_BLOCKED_VARIANT9,
	                                                    flash_syr2k_bsize,
	                                                    flash_scalr_cntl,
	                                                    flash_syr2k_cntl_blas,
	                                                    NULL,
	                                                    NULL );

	// Create a control tree that assumes A and B form an outer panel product.
	flash_syr2k_cntl_op    = FLA_Cntl_syr2k_obj_create( FLA_HIER,
	                                                    FLA_BLOCKED_VARIANT4,
	                                                    flash_syr2k_bsize,
	                                                    flash_scalr_cntl,
	                                                    flash_syr2k_cntl_blas,
	                                                    flash_gemm_cntl_pb_bb,
	                                                    flash_gemm_cntl_pb_bb );

	// Create a control tree that assumes A and B are both large.
	flash_syr2k_cntl_mm    = FLA_Cntl_syr2k_obj_create( FLA_HIER,
	                                                    FLA_BLOCKED_VARIANT9,
	                                                    flash_syr2k_bsize,
	                                                    flash_scalr_cntl,
	                                                    flash_syr2k_cntl_op,
	                                                    NULL,
	                                                    NULL );
}

void FLASH_Syr2k_cntl_finalize()
{
	FLA_Cntl_obj_free( flash_syr2k_cntl_blas );

	FLA_Cntl_obj_free( flash_syr2k_cntl_ip );
	FLA_Cntl_obj_free( flash_syr2k_cntl_op );
	FLA_Cntl_obj_free( flash_syr2k_cntl_mm );

	FLA_Blocksize_free( flash_syr2k_bsize );
}

