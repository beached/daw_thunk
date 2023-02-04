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
	struct thunk;

	/***
	 * sub    esp,0x18
	 * mov    eax, [function pointer]
 	 * push   [data pointer]
   * call   eax
   * add    esp,0x1c
   * ret
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<0> {
		unsigned char sub_esp_0x18[3] = { 0x83, 0xEC, 0x18 };
		unsigned char mov_eax = 0xB8;
		void *function_pointer = nullptr;
		unsigned char push = 0x68;
		void *user_data_pointer = nullptr;
		unsigned char call_eax[2] = { 0xE8, 0xD0 };
		unsigned char add_esp_0x1c[3] = { 0x83, 0xC4, 0x1C };
		unsigned char ret = 0xc3;
	};

	template<typename Thunk>
	constexpr void set_thunk_params( Thunk &th, void *user_data_pointer,
	                                 void *function_pointer ) {
		th->function_pointer = function_pointer;
		th->user_data_pointer = user_data_pointer;
	}
} // namespace daw::thunk_imp
