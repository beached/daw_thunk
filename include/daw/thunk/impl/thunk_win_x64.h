// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#if not defined( _WIN64 )
#error This header only works on Windows with x64 architectures
#endif

#include <cstddef>

namespace daw::thunk_impl {
	template<std::size_t /*PassedParams*/>
	struct thunk;

	/***
	 * mov rax, [function pointer]
	 * mov rcx, [data pointer]
	 * rex_jmp rax
	 */
	template<>
	struct thunk<0> {
#pragma pack( push, 1 )
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = nullptr;
		unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
		void *user_data_pointer = nullptr;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
#pragma pack( pop )
	};

	/***
	 * mov edx, ecx
	 * mov rax, [function pointer]
	 * mov rcx, [data pointer]
	 * rex_jmp rax
	 */
	template<>
	struct thunk<1> {
#pragma pack( push, 1 )
		unsigned char mov_edx_ecx[2] = { 0x89, 0xCA };
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = nullptr;
		unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
		void *user_data_pointer = nullptr;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
#pragma pack( pop )
	};

	/***
	 * mov r8d, edx
	 * mov rax, [function pointer]
	 * mov edx, ecx
	 * mov rcx, [data pointer]
	 * rex_jmp rax
	 */
	template<>
	struct thunk<2> {
#pragma pack( push, 1 )
		unsigned char mov_r8d_edx[3] = { 0x41, 0x89, 0xD0 };
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = nullptr;
		unsigned char mov_edx_ecx[2] = { 0x89, 0xCA };
		unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
		void *user_data_pointer = nullptr;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
#pragma pack( pop )
	};

	/***
	 * mov r9d, r8d
	 * mov rax, [function pointer]
	 * mov r8d, edx
	 * mov edx, ecx
	 * mov rcx, [data pointer]
	 * jmp rax
	 */
	template<>
	struct thunk<3> {
#pragma pack( push, 1 )
		unsigned char mov_r8d_edx[3] = { 0x41, 0x89, 0xD0 };
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = nullptr;
		unsigned char mov_edx_ecx[2] = { 0x89, 0xCA };
		unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
		void *user_data_pointer = nullptr;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
#pragma pack( pop )
	};

	template<>
	struct thunk<4> {
#pragma pack( push, 1 )
		unsigned char sub_rsp_0x38[4] = { 0x48, 0x83, 0xEC, 0x38 };
		unsigned char mov_rsp_0x020_r9d[5] = { 0x44, 0x89, 0x4C, 0x24, 0x20 };

		unsigned char mov_r8d_edx[3] = { 0x41, 0x89, 0xD0 };
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = nullptr;
		unsigned char mov_edx_ecx[2] = { 0x89, 0xCA };
		unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
		void *user_data_pointer = nullptr;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
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
