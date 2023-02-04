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
		unsigned char mov_rax_r8[3] = { 0x4C, 0x89, 0xC0 };
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
	/***
	 * push rax
	 * mov  rax,r9
	 * mov  r9,r8
	 * mov  r8,rcx
	 * mov  rcx,rdx
	 * mov  rdx,rsi
	 * mov  rsi,rdi
	 * mov  QWORD PTR [rsp],r9
	 * movabs  rdi, [user_data_pointer]
	 * movabs  r10, [function_ptr]
	 * push QWORD PTR [rsp+0x10]
	 * push rax
	 * call r10
	 * pop rcx
	 * ret
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<7> {
		unsigned char push_rax = 0x50;
		unsigned char mov_rax_r9[3] = { 0x4C, 0x89, 0xC8 };
		unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };
		unsigned char mov_r8_rcx[3] = { 0x49, 0x89, 0xC8 };
		unsigned char mov_rcx_rdx[3] = { 0x48, 0x89, 0xD1 };
		unsigned char mov_rdx_rsi[3] = { 0x48, 0x89, 0xF2 };
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_r10[2] = { 0x49, 0xBA };
		void *function_pointer = 0;
		unsigned char push_qword_ptr_rsp_plus_0x10[4] = { 0xFF, 0x74, 0x24, 0x10 };
		unsigned char push_rax_2 = 0x50;
		unsigned char call_r10[3] = { 0x41, 0xFF, 0xD2 };
		unsigned char add_rsp_0x10[4] = { 0x48, 0x83, 0xC4, 0x10 };
		unsigned char pop_rcx = 0x59;
		unsigned char ret = 0xc3;
	};
	/***
	 * sub  rsp,0x18
	 * mov  rax,r8
	 * mov  r8,rcx
	 * mov  rcx,rdx
	 * mov  rdx,rsi
	 * mov  rsi,rdi
	 * movaps xmm0, XMMWORD PTR [rsp+0x20]
	 * movups XMMWORD PTR [rsp+0x8], xmm0
	 * mov QWORD PTR [rsp],r9
	 * movabs  rdi, [user_data_pointer]
	 * movabs  r10, [function_ptr]
	 * mov r9, rax
	 * call r10
	 * add rsp,0x18
	 * ret
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<8> {
		unsigned char sub_rsp_0x18[4] = { 0x48, 0x83, 0xEC, 0x18 };

		unsigned char mov_rax_r8[3] = { 0x4C, 0x89, 0xC0 };
		unsigned char mov_r8_rcx[3] = { 0x49, 0x89, 0xC8 };
		unsigned char mov_rcx_rdx[3] = { 0x48, 0x89, 0xD1 };
		unsigned char mov_rdx_rsi[3] = { 0x48, 0x89, 0xF2 };
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };

		unsigned char movaps_xmm0_rsp_plus_0x20[5] = { 0x0F, 0x28, 0x44, 0x24,
		                                               0x20 };
		unsigned char movaps_rsp_plus_0x8_xmm0[5] = { 0x0F, 0x11, 0x44, 0x24,
		                                              0x08 };

		unsigned char mov_qword_ptr_rsp_r9[4] = { 0x4C, 0x89, 0x0C, 0x24 };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_r10[2] = { 0x49, 0xBA };
		void *function_pointer = 0;
		unsigned char mov_r9_rax[3] = { 0x49, 0x89, 0xC1 };
		unsigned char call_r10[3] = { 0x41, 0xFF, 0xD2 };

		unsigned char add_rsp_0x18[4] = { 0x48, 0x83, 0xc4, 0x18 };

		unsigned char ret = 0xc3;
	};

	/***
	 * sub  rsp,0x28
	 * mov  rax,r8
	 * mov  r8,rcx
	 * mov  rcx,rdx
	 * mov  rdx,rsi
	 * mov  rsi,rdi
	 * movaps xmm0, XMMWORD PTR [rsp+0x30]
	 * mov rdi, QWORD PTR [rsp+0x40]
	 * mov QWORD PTR [rsp+0x18],rdi
	 * movups XMMWORD PTR [rsp+0x8], xmm0
	 * mov QWORD PTR [rsp],r9
	 * movabs  rdi, [user_data_pointer]
	 * movabs  r10, [function_ptr]
	 * mov r9, rax
	 * call r10
	 * add rsp,0x28
	 * ret
	 */
	template<>
	struct __attribute__( ( packed ) ) thunk<9> {
		unsigned char sub_rsp_0x28[4] = { 0x48, 0x83, 0xEC, 0x28 };

		unsigned char mov_rax_r8[3] = { 0x4C, 0x89, 0xC0 };
		unsigned char mov_r8_rcx[3] = { 0x49, 0x89, 0xC8 };
		unsigned char mov_rcx_rdx[3] = { 0x48, 0x89, 0xD1 };
		unsigned char mov_rdx_rsi[3] = { 0x48, 0x89, 0xF2 };
		unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };

		unsigned char movaps_xmm0_rsp_plus_0x30[5] = { 0x0F, 0x28, 0x44, 0x24,
		                                               0x30 };
		unsigned char mov_rdi_rsp_plus_0x40[5] = { 0x48, 0x8b, 0x7c, 0x24, 0x40 };
		unsigned char mov_rdi_rsp_plus_18_rdi[5] = { 0x48, 0x89, 0x7c, 0x24, 0x18 };
		unsigned char movups_rsp_plus_0x8_xmm0[5] = { 0x0F, 0x11, 0x44, 0x24,
		                                              0x08 };
		unsigned char mov_rsp_r9[4] = { 0x4C, 0x89, 0x0C, 0x24 };
		unsigned char movabs_rdi[2] = { 0x48, 0xBF };
		void *user_data_pointer = 0;
		unsigned char movabs_r10[2] = { 0x49, 0xBA };
		void *function_pointer = 0;
		unsigned char mov_r9_rax[3] = { 0x49, 0x89, 0xC1 };
		unsigned char call_r10[3] = { 0x41, 0xFF, 0xD2 };

		unsigned char add_rsp_0x28[4] = { 0x48, 0x83, 0xc4, 0x28 };

		unsigned char ret = 0xc3;
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
	  ( calc_param_size_v<Ts> + ... );

} // namespace daw::thunk_impl
