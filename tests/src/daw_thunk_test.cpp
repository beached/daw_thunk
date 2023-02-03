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
	using T = unsigned long long;
	T x = 0;
	auto const lambda = [&x]( T num1, T num2, T num3, T num4, T num5,
	                          T num6 ) -> void {
		x += num1 + num2 + num3 + num4 + num5 + num6;
	};
	auto ec = daw::make_erased_callable( lambda );
	auto const th = daw::Thunk( ec );
	assert( th.thunk );
	std::cout << x << '\n';
	th( 1, 2, 3, 4, 5, 10 );
	std::cout << x << '\n';
	th( 1, 2, 3, 4, 5, 10 );
	std::cout << x << '\n';
}
