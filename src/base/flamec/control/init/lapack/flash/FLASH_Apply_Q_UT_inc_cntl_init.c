/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_THREAD_SAFE_INTERFACES
void FLASH_Apply_Q_UT_inc_cntl_init_ts(FLA_Cntl_init_flash_s *FLA_cntl_flash_init_i)
{
	// Set blocksizes for hierarchical storage.
	FLA_cntl_flash_init_i->flash_apqutinc_var1_bsize = FLA_Blocksize_create( 1, 1, 1, 1 );

	// Create a control tree to invoke variant 1.
	FLA_cntl_flash_init_i->flash_apqutinc_cntl = FLA_Cntl_apqutinc_obj_create( FLA_HIER,
	                                                    FLA_BLOCKED_VARIANT1, 
	                                                    FLA_cntl_flash_init_i->flash_apqutinc_var1_bsize,
	                                                    FLA_cntl_flash_init_i->flash_apqut_cntl,
	                                                    FLA_cntl_flash_init_i->flash_apq2ut_cntl );
}

void FLASH_Apply_Q_UT_inc_cntl_finalize_ts(FLA_Cntl_init_flash_s *FLA_cntl_flash_init_i)
{
	FLA_Cntl_obj_free( FLA_cntl_flash_init_i->flash_apqutinc_cntl );

	FLA_Blocksize_free( FLA_cntl_flash_init_i->flash_apqutinc_var1_bsize );
}

#endif

extern fla_apqut_t*  flash_apqut_cntl;
extern fla_apq2ut_t* flash_apq2ut_cntl;

fla_apqutinc_t*      flash_apqutinc_cntl = NULL;
fla_blocksize_t*     flash_apqutinc_var1_bsize = NULL;

void FLASH_Apply_Q_UT_inc_cntl_init()
{
	// Set blocksizes for hierarchical storage.
	flash_apqutinc_var1_bsize = FLA_Blocksize_create( 1, 1, 1, 1 );

	// Create a control tree to invoke variant 1.
	flash_apqutinc_cntl = FLA_Cntl_apqutinc_obj_create( FLA_HIER,
	                                                    FLA_BLOCKED_VARIANT1, 
	                                                    flash_apqutinc_var1_bsize,
	                                                    flash_apqut_cntl,
	                                                    flash_apq2ut_cntl );
}

void FLASH_Apply_Q_UT_inc_cntl_finalize()
{
	FLA_Cntl_obj_free( flash_apqutinc_cntl );

	FLA_Blocksize_free( flash_apqutinc_var1_bsize );
}

