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

#define ensure_equal( Lhs, Rhs )                                              \
	do {                                                                        \
		if( Lhs != Rhs ) {                                                        \
			std::cerr << "Expected equal: " << ( Lhs ) << " = " << ( Rhs ) << '\n'; \
			std::terminate( );                                                      \
		}                                                                         \
	} while( false )

int main( ) {
	using T = long long;
	T x = 0;

	{
		std::cout << "\n0 params\n********\n";
		auto const lambda = [&]( ) {
			++x;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( );
		std::cout << x << '\n';
		th( );
		std::cout << x << '\n';
		ensure_equal( x, 2 );
	}
	{
		std::cout << "\n1 param\n*******\n";
		x = 0;
		auto const lambda = [&]( int y0 ) {
			x += y0;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2 );
		std::cout << x << '\n';
		th( 2 );
		std::cout << x << '\n';
		ensure_equal( x, 4 );
	}

	{
		std::cout << "\n2 param\n*******\n";
		x = 0;
		auto const lambda = [&]( int y0, int y1 ) {
			x += y0 + y1;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2, 1 );
		std::cout << x << '\n';
		th( 2, 1 );
		std::cout << x << '\n';
		ensure_equal( x, 6 );
	}
	{
		std::cout << "\n3 param\n*******\n";
		x = 0;
		auto const lambda = [&]( int y0, int y1, int y2 ) {
			x += y0 + y1 + y2;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2, 1, 1 );
		std::cout << x << '\n';
		th( 2, 1, 1 );
		std::cout << x << '\n';
		ensure_equal( x, 8 );
	}
	{
		std::cout << "\n4 param\n*******\n";
		x = 0;
		auto const lambda = [&]( int y0, int y1, int y2, int y3 ) {
			x += y0 + y1 + y2 + y3;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2, 1, 1, 1 );
		std::cout << x << '\n';
		th( 2, 1, 1, 1 );
		std::cout << x << '\n';
		ensure_equal( x, 10 );
	}
	{
		std::cout << "\n5 param\n*******\n";
		x = 0;
		auto const lambda = [&]( int y0, int y1, int y2, int y3, int y4 ) {
			x += y0 + y1 + y2 + y3 + y4;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1 );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1 );
		std::cout << x << '\n';
		ensure_equal( x, 12 );
	}
	{
		std::cout << "\n6 param\n*******\n";
		x = 0;
		auto const lambda = [&]( T y0, T y1, T y2, T y3, T y4, T y5 ) {
			x += y0 + y1 + y2 + y3 + y4 + y5;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1, 1 );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1, 1 );
		std::cout << x << '\n';
		ensure_equal( x, 14 );
	}
	{
		std::cout << "\n7 param\n*******\n";
		x = 0;
		auto const lambda = [&]( T y0, T y1, T y2, T y3, T y4, T y5, T y6 ) {
			x += y0 + y1 + y2 + y3 + y4 + y5 + y6;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1, 1, 1 );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1, 1, 1 );
		std::cout << x << '\n';
		ensure_equal( x, 16 );
	}
	{
		std::cout << "\n8 param\n*******\n";
		x = 0;
		auto const lambda = [&]( T y0, T y1, T y2, T y3, T y4, T y5, T y6, T y7 ) {
			x += y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1, 1, 1, 1 );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1, 1, 1, 1 );
		std::cout << x << '\n';
		ensure_equal( x, 18 );
	}
	{
		std::cout << "\n9 param\n*******\n";
		x = 0;
		auto const lambda = [&]( T y0, T y1, T y2, T y3, T y4, T y5, T y6, T y7,
		                         T y8 ) {
			x += y0 + y1 + y2 + y3 + y4 + y5 + y6 + y7 + y8;
		};
		auto const th = daw::make_thunk( lambda );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1, 1, 1, 1, 1 );
		std::cout << x << '\n';
		th( 2, 1, 1, 1, 1, 1, 1, 1, 1 );
		std::cout << x << '\n';
		ensure_equal( x, 20 );
	}
	std::cout << "\n\n" << std::flush;
}
