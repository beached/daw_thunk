// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#include "daw/daw_thunk.h"

#include <iostream>
#include <csignal>

int main( ) {
	int x = 0; // 3.14159;
	auto lambda = [&x]( int signum ) -> void {
		++x;
	};
	auto th = daw::make_thunk( lambda );
	std::signal( SIGINT, th );
	int count = 0;
	while( x < 2 and count < 10 ) {
		std::cout << "Waiting for CTRL-C\n";
		sleep( 1 );
		if( x > 0 ) {
			std::cout << "CTRL-C pushed " << x << " times\n";
		}
		++count;
	}
}
