// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk_a_dunk
//

#include "daw/daw_erased_callable.h"
#include "daw/daw_thunk.h"

#include <iostream>
#include <vector>

int main( ) {
	auto x = std::vector<double>{ 3.14159 };

	auto lambda = [x]( ) mutable -> std::vector<double> const & {
		assert( not x.empty( ) );
		auto next_value = x.back( ) + 1.0;
		x.push_back( next_value );
		return x;
	};
	auto fp = daw::erased_callable<std::vector<double> const &( )>{ lambda };
	//fp.fp( fp.data );
	auto th = daw::Thunk<std::vector<double> const &( )>( fp.data, fp.fp );
	assert( th.thunk );
	auto th_fp = th.get( );
	std::cout << th_fp( ).back( ) << '\n';
	std::cout << th_fp( ).back( ) << '\n';
}
