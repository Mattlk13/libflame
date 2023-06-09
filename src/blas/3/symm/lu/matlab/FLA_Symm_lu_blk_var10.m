%
%
%   Copyright (C) 2014, The University of Texas at Austin
%
%   This file is part of libflame and is available under the 3-Clause
%   BSD license, which can be found in the LICENSE file at the top-level
%   directory, or at http://opensource.org/licenses/BSD-3-Clause
%
%

function [ C_out ] = FLA_Symm_lu_blk_var10( A, B, C, nb_alg )

  [ BL, BR ] = FLA_Part_1x2( B, ...
                               0, 'FLA_RIGHT' );

  [ CL, CR ] = FLA_Part_1x2( C, ...
                               0, 'FLA_RIGHT' );

  while ( size( BR, 2 ) < size( B, 2 ) )

    b = fla_min( size( BL, 2 ), nb_alg );

    [ B0, B1, B2 ]= FLA_Repart_1x2_to_1x3( BL, BR, ...
                                         b, 'FLA_LEFT' );

    [ C0, C1, C2 ]= FLA_Repart_1x2_to_1x3( CL, CR, ...
                                         b, 'FLA_LEFT' );

    %------------------------------------------------------------%

    C1 = C1 + A * B1;

    %------------------------------------------------------------%

    [ BL, BR ] = FLA_Cont_with_1x3_to_1x2( B0, B1, B2, ...
                                           'FLA_RIGHT' );

    [ CL, CR ] = FLA_Cont_with_1x3_to_1x2( C0, C1, C2, ...
                                           'FLA_RIGHT' );

  end

  C_out = [ CL, CR ];

return

