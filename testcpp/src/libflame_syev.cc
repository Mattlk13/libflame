
/******************************************************************************
* Copyright (c) 2019 - present Advanced Micro Devices, Inc. All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*******************************************************************************/

/*! @file liblame_syev.cc
 *  libflame_syev.cc Test application to validate CPP template interface
 *  */
 
#include "libflame_test.hh"

template< typename T >
void syev_test()
{
  int n = 512;
  srand (time(NULL));

  FLA_Init( );
  FLA_Obj aCIOObj, wCOObj;
  T *aCPPIOBuff, *wCPPOBuff;
  T *aCIOBuff, *wCOBuff;
  char jobz = 'N';
  char uplo = 'U';
  int datatype = getDatatype<T>();

  //Allocate and initialize buffers for C and CPP functions with random values
  allocate_init_buffer(aCPPIOBuff, aCIOBuff, n*n);
  wCPPOBuff  = new T[n];
  wCOBuff  = new T[n];

  //Call CPP function
  libflame::syev( LAPACK_COL_MAJOR, &jobz, &uplo, &n, aCPPIOBuff, &n, wCPPOBuff );

  //Allocate Object for C function and copy already allocated and filled buffer
  FLA_Obj_create_without_buffer( datatype, n, n, &aCIOObj );
  FLA_Obj_create_without_buffer( datatype, n, 1, &wCOObj );
  FLA_Obj_attach_buffer( aCIOBuff, 1, n, &aCIOObj );
  FLA_Obj_attach_buffer( wCOBuff, 1, n, &wCOObj );

  //Call C function
  FLA_Hevd_external( FLA_EVD_WITHOUT_VECTORS, FLA_UPPER_TRIANGULAR, aCIOObj, wCOObj );

  double diff =  computeError<T>( n, n, aCIOBuff, aCPPIOBuff );
  diff +=  computeError<T>( n, 1, wCOBuff, wCPPOBuff );

  if(diff != 0.0)
  {
    printf( "syev(): %s TEST FAIL\n" , __PRETTY_FUNCTION__);
  }else{
    printf( "syev(): %s TEST PASS\n" , __PRETTY_FUNCTION__);
  }

  //Free up the buffers
  delete aCPPIOBuff ;
  delete wCPPOBuff ;
  FLA_Obj_free( &aCIOObj );
  FLA_Obj_free( &wCOObj );
}

void syev_testall_variants(){
  syev_test<float>();
  syev_test<double>();
}

int main(int argc, char *argv[])
{
  syev_testall_variants();
}