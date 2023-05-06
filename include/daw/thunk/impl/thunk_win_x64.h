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

#if DAW_THUNK_ARCH != DAW_THUNK_ARCH_C_win_x64
#error This header only works on sys-v x64 architectures
#endif

namespace daw::thunk_impl {
	template<std::size_t /*PassedParams*/, typename = void>
	struct thunk;

	/***
	 *  push [user data pointer]
	 *  movabs eax, [function pointer]
	 *  call eax
	 *  add esp, 4
	 *  ret
	 */
	template<>
	struct thunk<0> {
#pragma pack( push, 1 )
		unsigned char sub_rsp_0x28[4] = { 0x48, 0x83, 0xEC, 0x28 };
		unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
		void *user_data_pointer = nullptr;
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = nullptr;
		unsigned char call_rax[2] = { 0xFF, 0xD0 };
		unsigned char add_rsp_0x28[4] = { 0x48, 0x83, 0xC4, 0x28 };
		unsigned char ret = 0xC2;
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
