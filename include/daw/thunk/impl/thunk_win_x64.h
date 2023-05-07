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
	inline namespace win_x64 {
		template<std::size_t /*PassedParams*/, typename = void>
		struct thunk;

		/***
		 * mov rcx, [data pointer]
		 * mov rax, [function pointer]
		 * rex_jmp rax
		 */
		template<>
		struct thunk<0> {
#pragma pack( push, 1 )
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
#pragma pack( pop )
		};

		/***
		 * mov rax, [function pointer]
		 * mov rdx, rcx
		 * mov rcx, [data pointer]
		 * rex_jmp rax
		 */
		template<>
		struct thunk<1> {
#pragma pack( push, 1 )
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
#pragma pack( pop )
		};

		/***
		 * mov rax, [function pointer]
		 * mov r8, rdx
		 * mov rdx, rcx
		 * mov rcx, [data pointer]
		 * rex_jmp rax
		 */
		template<>
		struct thunk<2> {
#pragma pack( push, 1 )
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char mov_r8_rdx[3] = { 0x49, 0x89, 0xD0 };
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
#pragma pack( pop )
		};

		/***
		 * mov rax, [function pointer]
		 * mov r9, r8
		 * mov r8, rdx
		 * mov rdx, rcx
		 * mov rcx, [data pointer]
		 * jmp rax
		 */
		template<>
		struct thunk<3> {
#pragma pack( push, 1 )
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };
			unsigned char mov_r8_rdx[3] = { 0x49, 0x89, 0xD0 };
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
#pragma pack( pop )
		};

		/***
		 * sub rsp, 56
		 * movabs rax, -4294858070
		 * mov QWORD PTR 32[rsp], r9
		 * mov r9, r8
		 * mov r8, rdx
		 * mov rdx, rcx
		 * movabs rcx, 1311768467463790320
		 * call rax
		 * add rsp, 56
		 * ret
		 */
		template<>
		struct thunk<4> {
#pragma pack( push, 1 )
			unsigned char sub_rsp_0x38[4] = { 0x48, 0x83, 0xEC, 0x38 };
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char mov_rsp_p_0x20_r9[5] = { 0x4C, 0x89, 0x4C, 0x24, 0x20 };
			unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };
			unsigned char mov_r8_ddx[3] = { 0x49, 0x89, 0xD0 };
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char call_rax[2] = { 0xFF, 0xD0 };
			unsigned char add_rsp_0x38[4] = { 0x48, 0x83, 0xC4, 0x38 };
			unsigned char ret[1] = { 0xC3 };
#pragma pack( pop )
		};

		/***
		 * sub rsp, 56
		 * mov rax, QWORD PTR 96[rsp]
		 * mov QWORD PTR 32[rsp], r9
		 * mov QWORD PTR 40[rsp], rax
		 * mov r9, r8
		 * movabs rax, [function pointer]
		 * mov r8, rdx
		 * mov rdx, rcx
		 * movabs rcx, [user pointer]
		 * call rax
		 * add  rsp, 56
		 * ret
		 */
		template<>
		struct thunk<5> {
#pragma pack( push, 1 )
			unsigned char sub_rsp_0x38[4] = { 0x48, 0x83, 0xEC, 0x38 };
			unsigned char mov_rax_rsp_p_0x60[5] = { 0x48, 0x8B, 0x44, 0x24, 0x60 };
			unsigned char mov_rsp_0x20_r9[5] = { 0x4C, 0x89, 0x4C, 0x24, 0x20 };
			unsigned char mov_rsp_p_0x28_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x28 };
			unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char mov_r8_rdx[3] = { 0x49, 0x89, 0xD0 };
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char call_rax[2] = { 0xFF, 0xD0 };
			unsigned char add_rsp_0x38[4] = { 0x48, 0x83, 0xC4, 0x38 };
			unsigned char ret_0[1] = { 0xC3 };
#pragma pack( pop )
		};

		/***
		 *  sub rsp, 72
		 *  mov rax, QWORD PTR 120[rsp]
		 *  mov DWORD PTR 32[rsp], r9
		 *  mov DWORD PTR 48[rsp], rax
		 *  mov rax, QWORD PTR 112[rsp]
		 *  mov r9, r8
		 *  mov QWORD PTR 40[rsp], rax
		 *  mov r8, rdx
		 *  movabs rax, [function pointer]
		 *  mov rdx, rcx
		 *  movabs rcx, [data pointer]
		 *  call rax
		 *  add rsp, 72
		 *  ret
		 */
		template<>
		struct thunk<6> {
#pragma pack( push, 1 )
			unsigned char sub_rsp_0x48[4] = { 0x48, 0x83, 0xEC, 0x48 };
			unsigned char mov_rax_rsp_p_0x78[5] = { 0x48, 0x8B, 0x44, 0x24, 0x78 };
			unsigned char mov_rsp_p_0x20_r9[5] = { 0x4C, 0x89, 0x4C, 0x24, 0x20 };
			unsigned char mov_rsp_p_0x30_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x30 };
			unsigned char mov_rax_rsp_p_0x70[5] = { 0x48, 0x8B, 0x44, 0x24, 0x70 };
			unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };
			unsigned char mov_rsp_p_0x28_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x28 };
			unsigned char mov_r8_rdx[3] = { 0x49, 0x89, 0xD0 };
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char call_rax[2] = { 0xFF, 0xD0 };
			unsigned char add_rsp_0x48[4] = { 0x48, 0x83, 0xC4, 0x48 };
			unsigned char ret[1] = { 0xC3 };
#pragma pack( pop )
		};

		/***
		 * sub rsp, 72
		 * mov rax, QWORD PTR 128[rsp]
		 * mov QWORD PTR 32[rsp], r9
		 * mov QWORD PTR 56[rsp], rax
		 * mov rax, QWORD PTR 120[rsp]
		 * mov r9, r8
		 * mov QWORD PTR 48[rsp], rax
		 * mov rax, QWORD PTR 112[rsp]
		 * mov r8, rdx
		 * mov QWORD PTR 40[rsp], rax
		 * mov rdx, rcx
		 * movabs rax, [function pointer]
		 * movabs rcx, [data pointer]
		 * call rax
		 * add rsp, 72
		 * ret
		 */
		template<>
		struct thunk<7> {
#pragma pack( push, 1 )
			unsigned char sub_rsp_0x48[4] = { 0x48, 0x83, 0xEC, 0x48 };
			unsigned char mov_rax_rsp_p_0x80[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x80, 0x0,  0x0,  0x0 };
			unsigned char mov_rsp_p_0x20_r9[5] = { 0x4C, 0x89, 0x4C, 0x24, 0x20 };
			unsigned char mov_rsp_p_0x38_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x38 };
			unsigned char mov_rax_rsp_p_0x78[5] = { 0x48, 0x8B, 0x44, 0x24, 0x78 };
			unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };
			unsigned char mov_rsp_p_0x30_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x30 };
			unsigned char mov_rax_rsp_p_0x70[5] = { 0x48, 0x8B, 0x44, 0x24, 0x70 };
			unsigned char mov_r8_rdx[3] = { 0x49, 0x89, 0xD0 };
			unsigned char mov_rsp_p_0x28_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x28 };
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char call_rax[2] = { 0xFF, 0xD0 };
			unsigned char add_rsp_0x48[4] = { 0x48, 0x83, 0xC4, 0x48 };
			unsigned char ret[1] = { 0xC3 };
#pragma pack( pop )
		};

		/***
		 * sub rsp, 88
		 * mov rax, QWORD PTR 152[rsp]
		 * mov QWORD PTR 32[rsp], r9
		 * mov QWORD PTR 64[rsp], rax
		 * mov rax, QWORD PTR 144[rsp]
		 * mov r9, r8
		 * mov QWORD PTR 56[rsp], rax
		 * mov rax, QWORD PTR 136[rsp]
		 * mov r8, rdx
		 * mov QWORD PTR 48[rsp], rax
		 * mov rax, QWORD PTR 128[rsp]
		 * mov rdx, rcx
		 * mov QWORD PTR 40[rsp], rax
		 * movabs rcx, [data pointer]
		 * movabs rax, [function pointer]
		 * call rax
		 * add rsp, 88
		 * ret
		 */
		template<>
		struct thunk<8> {
#pragma pack( push, 1 )
			unsigned char sub_rsp_0x58[4] = { 0x48, 0x83, 0xEC, 0x58 };
			unsigned char mov_rax_rsp_p_0x98[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x98, 0x0,  0x0,  0x0 };
			unsigned char mov_rsp_p_0x20_r9[5] = { 0x4C, 0x89, 0x4C, 0x24, 0x20 };
			unsigned char mov_rsp_p_0x40_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x40 };
			unsigned char mov_rax_rsp_p_0x90[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x90, 0x0,  0x0,  0x0 };
			unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };
			unsigned char mov_rsp_p_0x38_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x38 };
			unsigned char mov_rax_rsp_p_0x88[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x88, 0x0,  0x0,  0x0 };
			unsigned char mov_r8_rdx[3] = { 0x49, 0x89, 0xD0 };
			unsigned char mov_rsp_p_0x30_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x30 };
			unsigned char mov_rax_rsp_p_0x80[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x80, 0x0,  0x0,  0x0 };
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char mov_rsp_p_0x28_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x28 };
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char call_rax[2] = { 0xFF, 0xD0 };
			unsigned char add_rsp_0x58[4] = { 0x48, 0x83, 0xC4, 0x58 };
			unsigned char ret[1] = { 0xC3 };
#pragma pack( pop )
		};

		/***
		 * sub rsp, 88
		 * mov rax, QWORD PTR 160[rsp]
		 * mov QWORD PTR 32[rsp], r9
		 * mov QWORD PTR 72[rsp], rax
		 * mov rax, QWORD PTR 152[rsp]
		 * mov r9, r8
		 * mov QWORD PTR 64[rsp], rax
		 * mov rax, QWORD PTR 144[rsp]
		 * mov r8, rdx
		 * mov QWORD PTR 56[rsp], rax
		 * mov rax, QWORD PTR 136[rsp]
		 * mov rdx, rcx
		 * mov QWORD PTR 48[rsp], rax
		 * mov rax, QWORD PTR 128[rsp]
		 * movabs rcx, [user pointer]
		 * mov QWORD PTR 40[rsp], rax
		 * movabs rax, [function pointer]
		 * call rax
		 * add rsp, 88
		 * ret
		 */
		template<>
		struct thunk<9> {
#pragma pack( push, 1 )
			unsigned char sub_rsp_0x58[4] = { 0x48, 0x83, 0xEC, 0x58 };
			unsigned char mov_rax_rsp_p_0xa0[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0xA0, 0x0,  0x0,  0x0 };
			unsigned char mov_rsp_p_0x20_r9[5] = { 0x4C, 0x89, 0x4C, 0x24, 0x20 };
			unsigned char mov_rsp_p_0x48_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x48 };
			unsigned char mov_rax_rsp_p_0x98[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x98, 0x0,  0x0,  0x0 };
			unsigned char mov_r9_r8[3] = { 0x4D, 0x89, 0xC1 };

			unsigned char mov_rsp_p_0x40_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x40 };
			unsigned char mov_rax_rsp_p_0x90[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x90, 0x0,  0x0,  0x0 };
			unsigned char mov_r8_rdx[3] = { 0x49, 0x89, 0xD0 };
			unsigned char mov_rsp_p_0x38_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x38 };
			unsigned char mov_rax_rsp_p_0x88[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x88, 0x0,  0x0,  0x0 };
			unsigned char mov_rdx_rcx[3] = { 0x48, 0x89, 0xCA };
			unsigned char mov_rsp_p_0x30_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x30 };
			unsigned char mov_rax_rsp_p_0x80[8] = { 0x48, 0x8B, 0x84, 0x24,
			                                        0x80, 0x0,  0x0,  0x0 };
			unsigned char movabs_rcx[2] = { 0x48, 0xB9 };
			void *user_data_pointer = nullptr;
			unsigned char mov_rsp_p_0x28_rax[5] = { 0x48, 0x89, 0x44, 0x24, 0x28 };
			unsigned char movabs_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char call_rax[2] = { 0xFF, 0xD0 };
			unsigned char add_rsp_0x58[4] = { 0x48, 0x83, 0xC4, 0x58 };
			unsigned char ret[1] = { 0xC3 };
#pragma pack( pop )
		};

		//**************
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

	} // namespace win_x64
} // namespace daw::thunk_impl
