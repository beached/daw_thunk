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

#if DAW_THUNK_ARCH != DAW_THUNK_ARCH_C_win_x86
#error This header only works on sys-v x86 architectures
#endif

namespace daw::thunk_impl {
	template<std::size_t /*PassedParams*/, typename = void>
	struct thunk;

	/***
	 *  sub rsp, 0x28
	 *  movabs rcx, [user data pointer]
	 *  movabs rax, [function pointer]
	 *  call rax
	 *  add rsp, 0x28
	 *  ret
	 */
	template<>
	struct thunk<0> {
#pragma pack( push, 1 )
		unsigned char push = 0x68;
		void *user_data_pointer = nullptr;
		unsigned char movabs_eax = 0xB8;
		void *function_pointer = nullptr;
		unsigned char call_eax[2] = { 0xFF, 0xD0 };
		unsigned char add_esp_0x4[3] = { 0x83, 0xC4, 0x04 };
		unsigned char ret = 0xC3;
#pragma pack( pop )
	};

	template<typename Thunk>
	constexpr void set_thunk_params( Thunk &th, void *user_data_pointer,
	                                 void *function_pointer ) {
		th->user_data_pointer = user_data_pointer;
		th->function_pointer = function_pointer;
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
