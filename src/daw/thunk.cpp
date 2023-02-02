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

int main( ) {
	int x = 1; // 3.14159;
	using T = long long;
	auto lambda = [&x]( T d1, T d2, T d3, T d4, T d5 ) -> void {
		x += d1 + d2 + d3 + d4 + d5;
	};
	auto th = daw::make_thunk( lambda );
	assert( th.thunk );
	std::cout << x << '\n';
	th( 1, 2, 3, 4, 5 );
	std::cout << x << '\n';
	th( 1, 2, 3, 4, 5 );
	std::cout << x << '\n';
}
