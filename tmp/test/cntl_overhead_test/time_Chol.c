
#include "FLAME.h"



#define FLA_ALG_REFERENCE 0
#define FLA_ALG_FRONT     1
#define FLA_ALG_HANDTUNED 2


FLA_Error REF_Chol( FLA_Trans trans, FLA_Obj A );

void time_Chol(
                int param_combo, int type, int nrepeats, int m,
                FLA_Obj A, FLA_Obj A_ref,
                double *dtime, double *diff, double *gflops );


void time_Chol(
                int param_combo, int type, int nrepeats, int m,
                FLA_Obj A, FLA_Obj A_ref,
                double *dtime, double *diff, double *gflops )
{
  int
    irep;

  double
    dtime_old = 1.0e9;

  FLA_Obj
    A_old, A_flat;

  if( param_combo == 1 && type == FLA_ALG_HANDTUNED )
  {
    *gflops = 0.0;
    *diff   = 0.0;
    return;
  }

  FLASH_Obj_create_conf_to( FLA_NO_TRANSPOSE, A, &A_old );
  FLASH_Obj_create_flat_conf_to_hier( FLA_NO_TRANSPOSE, A, &A_flat );

  FLASH_Copy( A, A_old );

  for ( irep = 0 ; irep < nrepeats; irep++ )
  {
    FLASH_Copy( A_old, A );
    FLASH_Obj_flatten( A, A_flat );

    *dtime = FLA_Clock();

    switch( param_combo ){

    case 0:{
      switch( type ){
      case FLA_ALG_REFERENCE:
        REF_Chol( FLA_LOWER_TRIANGULAR, A_flat );
        break;
      case FLA_ALG_FRONT:
        FLASH_Chol( FLA_LOWER_TRIANGULAR, A );
        break;
      case FLA_ALG_HANDTUNED:
        FLA_Chol_l_blk_var3_ht( FLA_LOWER_TRIANGULAR, A, NULL );
        break;
      default:
        printf("trouble\n");
      }

      break;
    }

    case 1:{
      switch( type ){
      case FLA_ALG_REFERENCE:
        REF_Chol( FLA_UPPER_TRIANGULAR, A_flat );
        break;
      case FLA_ALG_FRONT:
        FLASH_Chol( FLA_UPPER_TRIANGULAR, A );
        break;
      case FLA_ALG_HANDTUNED:
        break;
      default:
        printf("trouble\n");
      }

      break;
    }

    }

    *dtime = FLA_Clock() - *dtime;
    dtime_old = fla_min( *dtime, dtime_old );
  }

  if ( type == FLA_ALG_REFERENCE ){
    FLASH_Obj_hierarchify( A_flat, A_ref );
    *diff = 0.0;
  }
  else{
    *diff = FLASH_Max_elemwise_diff( A, A_ref );
  }

  *gflops = 1.0 / 3.0 * m * m * m /
            dtime_old / 1e9;

  *dtime = dtime_old;

  FLASH_Copy( A_old, A );

  FLASH_Obj_free( &A_old );
  FLASH_Obj_free( &A_flat );
}

