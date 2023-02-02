// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#include <daw/daw_thunk.h>

#include <cstdio>
#include <cassert>

int main( int argc, char ** argv ) {
	int x = 0;
	auto lambda = [&x] {
		++x;
	};
	auto th = daw::make_thunk( lamba );
	th( );
	th( );
	assert( x == 2 );
}

