// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once
#if not defined( __i386__ )
#error This header only works on sys-v x86 architectures
#endif

#include <cstddef>

namespace daw::thunk_imp {
	template<std::size_t /*PassedParams*/>
	struct __attribute__( ( packed ) ) thunk {
		unsigned char push = 0x68;
		void *state = nullptr;
		unsigned char call = 0xE8;
		long call_offset = 0;
		unsigned char add_esp[3] = { 0x83, 0xC4, 0x04 };
		unsigned char ret = 0xC3;
	};

	template<typename Thunk>
	constexpr void set_thunk_params( Thunk &th, void *state,
	                                 void *function_pointer ) {
		th->state = state;
		th->call_offset = reinterpret_cast<void *>(
		  reinterpret_cast<char *>( function_pointer ) - &th->add_esp[0] );
	}
} // namespace daw::thunk_imp
