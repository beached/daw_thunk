// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#include "daw/daw_erased_callable.h"
#include "daw/daw_thunk.h"

#include <iostream>
#include <vector>

int main( ) {
	double x = 3.14159;
	auto lambda = [&x]( int i ) -> void {
		x += i;
	};
	auto fp = daw::erased_callable<void( int )>{ lambda };
	//fp.fp( fp.data, 1 );
	auto th = daw::Thunk<void( int )>( fp.data, fp.fp );
	assert( th.thunk );
	auto th_fp = th.get( );
	std::cout << x << '\n';
	th_fp( 1 );
	std::cout << x << '\n';
	th_fp( 2 );
	std::cout << x << '\n';
}
