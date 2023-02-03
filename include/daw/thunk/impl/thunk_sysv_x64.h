// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#if not defined( __x86_64__ )
#error This header only works on sys-v x64 architectures
#endif

#include <cstddef>

namespace daw::thunk_impl {
	template<std::size_t /*PassedParams*/>
	struct thunk;

	/***
	 * movabs rdi, [user_data_pointer]
	 * movabs rax, [function_ptr]
	 * jmp rax
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<0> {
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = 0;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };

		constexpr void set( void *user_data_pointer_ptr, void *func_ptr ) {}
	};
	/***
	 * mov rsi, rdi
	 * movabs rdi, [user_data_pointer]
	 * movabs rax, [function_ptr]
	 * jmp rax
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<1> {
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = 0;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
	};
	/***
	 * mov rdx, rsi
	 * mov rsi, rdi
	 * movabs rdi, [user_data_pointer]
	 * movabs rax, [function_ptr]
	 * jmp rax
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<2> {
		unsigned char mov_rdx_rsi[3] = { 0x48, 0x89, 0xF2 };
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = 0;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
	};
	/***
	 * mov rcx, rdx
	 * mov rdx, rsi
	 * mov rsi, rdi
	 * movabs rdi, [user_data_pointer]
	 * movabs rax, [function_ptr]
	 * jmp rax
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<3> {
		unsigned char mov_rcx_rdx[3] = { 0x48, 0x89, 0xD1 };
		unsigned char mov_rdx_rsi[3] = { 0x48, 0x89, 0xF2 };
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = 0;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
	};
	/***
	 * mov r8, rcx
	 * mov rcx, rdx
	 * mov rdx, rsi
	 * mov rsi, rdi
	 * movabs rdi, [user_data_pointer]
	 * movabs rax, [function_ptr]
	 * jmp rax
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<4> {
		unsigned char mov_r8_rcx[3] = { 0x48, 0x89, 0xC8 };
		unsigned char mov_rcx_rdx[3] = { 0x48, 0x89, 0xD1 };
		unsigned char mov_rdx_rsi[3] = { 0x48, 0x89, 0xF2 };
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = 0;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
	};
	/***
	 * mov r9, r8
	 * mov r8, rcx
	 * mov rcx, rdx
	 * mov rdx, rsi
	 * mov rsi, rdi
	 * movabs rdi, [user_data_pointer]
	 * movabs rax, [function_ptr]
	 * jmp rax
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<5> {
		unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };
		unsigned char mov_r8_rcx[3] = { 0x49, 0x89, 0xC8 };
		unsigned char mov_rcx_rdx[3] = { 0x48, 0x89, 0xD1 };
		unsigned char mov_rdx_rsi[3] = { 0x48, 0x89, 0xF2 };
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_rax[2] = { 0x48, 0xB8 };
		void *function_pointer = 0;
		unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
	};
	/***
	 * push rax
	 * mov  rax,r8
	 * mov  r8,rcx
	 * mov  rcx,rdx
	 * mov  rdx,rsi
	 * mov  rsi,rdi
	 * mov  QWORD PTR [rsp],r9
	 * movabs  rdi, [user_data_pointer]
	 * movabs  r10, [function_ptr]
	 * mov r9, rax
	 * call r10
	 * pop rcx
	 * ret
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<6> {
		unsigned char push_rax = 0x50;
		unsigned char movabs_rax_r8[3] = { 0x4C, 0x89, 0xC0 };
		unsigned char mov_r8_rcx[3] = { 0x49, 0x89, 0xC8 };
		unsigned char mov_rcx_rdx[3] = { 0x48, 0x89, 0xD1 };
		unsigned char mov_rdx_rsi[3] = { 0x48, 0x89, 0xF2 };
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };
		unsigned char mov_qword_ptr_rsp_r9[4] = { 0x4C, 0x89, 0x0C, 0x24 };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_r10[2] = { 0x49, 0xBA };
		void *function_pointer = 0;
		unsigned char mov_r9_rax[3] = { 0x49, 0x89, 0xC1 };
		unsigned char call_r10[3] = { 0x41, 0xFF, 0xD2 };
		unsigned char pop_rcx = 0x59;
		unsigned char ret = 0xc3;
	};

	template<typename Thunk>
	constexpr void set_thunk_params( Thunk &th, void *user_data_pointer,
	                                 void *function_pointer ) {
		th->user_data_pointer = user_data_pointer;
		th->function_pointer = function_pointer;
	}
} // namespace daw::thunk_impl
