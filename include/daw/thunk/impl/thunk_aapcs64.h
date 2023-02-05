// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#include "thunk_arch.h"

#include <cstddef>
#include <type_traits>

#if DAW_THUNK_ARCH != DAW_THUNK_ARCH_C_aapcs64
#error This header only works on aapcs64 arm64 architectures
#endif

#error Not implemented

namespace daw::thunk_impl {
	template<std::size_t /*PassedParams*/>
	struct thunk;

	/*
	 * mov x0, #0xa4a4
	 * mov x1, #0x3232
	 * movk x1, #0x1212, lsl #16
	 * movk x0, #0xb3b3, lsl #16
	 * movk x1, #0xf0f, lsl #32
	 * movk x0, #0xc2c2, lsl #32
	 * movk x1, #0xf0f0, lsl #48
	 * movk x0, #0xd1d1, lsl #48
	 * mov x16, x1
	 * br x16
	 * x1 is data pointer, x0 is function pointer
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<0> {
		void *function_pointer = nullptr;
		void *user_data_pointer = nullptr;
	};

	template<typename Thunk>
	constexpr void set_thunk_params( Thunk &th, void *user_data_pointer,
	                                 void *function_pointer ) {
		th->function_pointer = function_pointer;
		th->user_data_pointer = user_data_pointer;
	}

	template<typename Param>
	inline constexpr std::size_t calc_param_size_v =
	  std::is_floating_point_v<Param> ? 0
	  : sizeof( Param ) <= 8          ? 1
	  : sizeof( Param ) <= 16         ? 2
	                                  : 0;

	template<typename, typename... Ts>
	inline constexpr std::size_t calculate_size_v =
	  ( calc_param_size_v<Ts> + ... + 0 );

} // namespace daw::thunk_impl
