// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#include <daw/daw_thunk.h>

#include <exception>
#include <iostream>

int func( int ( *callback )( int ) ) {
	return callback( 5 ) * 2;
}

int cb( int x, int y ) {
	return x + y;
}

int main( ) {
	int x = 42;
	auto f = [x]( int y ) {
		return cb( x, y );
	};
	auto th = daw::make_thunk( f );
	auto result = func( th );
	if( result != 94 ) {
		std::cerr << "Unexpected result\n";
		std::terminate( );
	}
}
