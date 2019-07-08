/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

#ifdef FLA_ENABLE_THREAD_SAFE_INTERFACES
void FLASH_Copyt_cntl_init_ts(FLA_Cntl_init_flash_s *FLA_cntl_flash_init_i)
{
	// Set blocksize for hierarchical storage.
	FLA_cntl_flash_init_i->flash_copyt_bsize     = FLA_Blocksize_create( 1, 1, 1, 1 );

	// Create a control tree that assumes A and B are small.
	FLA_cntl_flash_init_i->flash_copyt_cntl_blas = FLA_Cntl_copyt_obj_create( FLA_HIER,
	                                                   FLA_SUBPROBLEM,
	                                                   NULL,
	                                                   NULL );

	// Create a control tree that marches through A and B vertically.
	FLA_cntl_flash_init_i->flash_copyt_cntl_tb   = FLA_Cntl_copyt_obj_create( FLA_HIER,
	                                                   FLA_BLOCKED_VARIANT1,
	                                                   FLA_cntl_flash_init_i->flash_copyt_bsize,
	                                                   FLA_cntl_flash_init_i->flash_copyt_cntl_blas );

	// Create a control tree that marches through A and B horizontally.
	FLA_cntl_flash_init_i->flash_copyt_cntl_lr   = FLA_Cntl_copyt_obj_create( FLA_HIER,
	                                                   FLA_BLOCKED_VARIANT3,
	                                                   FLA_cntl_flash_init_i->flash_copyt_bsize,
	                                                   FLA_cntl_flash_init_i->flash_copyt_cntl_blas );

	// Create a control tree that marches through A and B horizontally, then
	// vertically.
	FLA_cntl_flash_init_i->flash_copyt_cntl      = FLA_Cntl_copyt_obj_create( FLA_HIER,
	                                                   FLA_BLOCKED_VARIANT3,
	                                                   FLA_cntl_flash_init_i->flash_copyt_bsize,
	                                                   FLA_cntl_flash_init_i->flash_copyt_cntl_tb );
}

void FLASH_Copyt_cntl_finalize_ts(FLA_Cntl_init_flash_s *FLA_cntl_flash_init_i)
{
	FLA_Cntl_obj_free( FLA_cntl_flash_init_i->flash_copyt_cntl_blas );

	FLA_Cntl_obj_free( FLA_cntl_flash_init_i->flash_copyt_cntl_tb );
	FLA_Cntl_obj_free( FLA_cntl_flash_init_i->flash_copyt_cntl_lr );
	FLA_Cntl_obj_free( FLA_cntl_flash_init_i->flash_copyt_cntl );

	FLA_Blocksize_free( FLA_cntl_flash_init_i->flash_copyt_bsize );
}

#endif

fla_copyt_t*       flash_copyt_cntl_blas = NULL;
fla_copyt_t*       flash_copyt_cntl_tb;
fla_copyt_t*       flash_copyt_cntl_lr;
fla_copyt_t*       flash_copyt_cntl;
fla_blocksize_t*   flash_copyt_bsize;

void FLASH_Copyt_cntl_init()
{
	// Set blocksize for hierarchical storage.
	flash_copyt_bsize     = FLA_Blocksize_create( 1, 1, 1, 1 );

	// Create a control tree that assumes A and B are small.
	flash_copyt_cntl_blas = FLA_Cntl_copyt_obj_create( FLA_HIER,
	                                                   FLA_SUBPROBLEM,
	                                                   NULL,
	                                                   NULL );

	// Create a control tree that marches through A and B vertically.
	flash_copyt_cntl_tb   = FLA_Cntl_copyt_obj_create( FLA_HIER,
	                                                   FLA_BLOCKED_VARIANT1,
	                                                   flash_copyt_bsize,
	                                                   flash_copyt_cntl_blas );

	// Create a control tree that marches through A and B horizontally.
	flash_copyt_cntl_lr   = FLA_Cntl_copyt_obj_create( FLA_HIER,
	                                                   FLA_BLOCKED_VARIANT3,
	                                                   flash_copyt_bsize,
	                                                   flash_copyt_cntl_blas );

	// Create a control tree that marches through A and B horizontally, then
	// vertically.
	flash_copyt_cntl      = FLA_Cntl_copyt_obj_create( FLA_HIER,
	                                                   FLA_BLOCKED_VARIANT3,
	                                                   flash_copyt_bsize,
	                                                   flash_copyt_cntl_tb );
}

void FLASH_Copyt_cntl_finalize()
{
	FLA_Cntl_obj_free( flash_copyt_cntl_blas );

	FLA_Cntl_obj_free( flash_copyt_cntl_tb );
	FLA_Cntl_obj_free( flash_copyt_cntl_lr );
	FLA_Cntl_obj_free( flash_copyt_cntl );

	FLA_Blocksize_free( flash_copyt_bsize );
}

