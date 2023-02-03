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
#include <cstdint>

namespace daw::thunk_imp {
	template<std::size_t /*PassedParams*/>
	struct __attribute__( ( packed ) ) thunk {
		unsigned char push = 0x68;
		std::uintptr_t user_data_pointer = 0;
		unsigned char call = 0xE8;
		long call_offset = 0;
		unsigned char add_esp[3] = { 0x83, 0xC4, 0x04 };
		unsigned char ret = 0xC3;
	};

	template<typename Thunk>
	constexpr void set_thunk_params( Thunk &th, std::uintptr_t user_data_pointer,
	                                 std::uintptr_t function_pointer ) {
		th->user_data_pointer = user_data_pointer;
		th->call_offset = function_pointer - &th->add_esp[0];
	}
} // namespace daw::thunk_imp
