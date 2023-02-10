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

namespace daw::thunk_impl {
	template<std::size_t /*PassedParams*/>
	struct thunk;

	// clang-format off
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
	 * x0
	 * lsl 48
	 *                                           0123'4567'8901'2345'6789'0123'4567'8901
	 *                                           XXfX XXXX Xff0 0001 1112 2223 333X XXXX
	 * f555 -> 1111'0101'0101'0101: f2fe'aaa0 -> 1111'0010'1111'1110'1010'1010'1010'0000
	 * 5555 -> 0101'0101'0101'0101: f2ea'aaa0 -> 1111'0010'1110'1010'1010'1010'1010'0000
	 * 5455 -> 0101'0100'0101'0101: f2ea'8aa0 -> 1111'0010'1110'1010'1000'1010'1010'0000
	 * 5355 -> 0101'0011'0101'0101: f2ea'6aa0 -> 1111'0010'1110'1010'0110'1010'1010'0000
	 * 5545 -> 0101'0101'0100'0101: f2ea'a8a0 -> 1111'0010'1110'1010'1010'1000'1010'0000
	 * lsl 0
	 * 5545 -> 0101'0101'0100'0101: d28a'a8a0 -> 1101'0010'1000'1010'1010'1000'1010'0000
	 * lsl 16
	 * 5545 -> 0101'0101'0100'0101: f2aa'a8a0 -> 1111'0010'1010'1010'1010'1000'1010'0000
	 * lsl 32
	 * 5545 -> 0101'0101'0100'0101: f2ca'a8a0 -> 1111'0010'1100'1010'1010'1000'1010'0000
	 * lsl 48 x0
	 * 5545 -> 0101'0101'0100'0101: f2ea'a8a0 -> 1111'0010'1110'1010'1010'1000'1010'0000
	 * lsl 48 x1
	 * 5545 -> 0101'0101'0100'0101: f2ea'a8a1 -> 1111'0010'1110'1010'1010'1000'1010'0001
	 *
	 */
	using arm64_op = unsigned char[4];
	// clang-format on
	template<>
	struct thunk<0> {
		arm64_op mov_x0_fp_lsl0 = { };
		arm64_op mov_x1_dp_lsl0 = { };
		arm64_op mov_x1_dp_lsl16 = { };
		arm64_op mov_x0_fp_lsl16 = { };
		arm64_op mov_x1_dp_lsl32 = { };
		arm64_op mov_x0_fp_lsl32 = { };
		arm64_op mov_x1_dp_lsl48 = { };
		arm64_op mov_x0_fp_lsl48 = { };
		arm64_op mov_x16_x1 = {
		  0xAA,
		  0x01,
		  0x03,
		  0xF0,
		};
		arm64_op br_x16 = {
		  0xD6,
		  0x1f,
		  0x02,
		  0x0,
		};

		template<std::uint32_t reg, unsigned char off>
		void set_imm_mov( void *pointer, arm64_op &op_codes ) {
			static_assert( reg < 16 );
			auto const fp_full = reinterpret_cast<std::uintptr_t>( pointer );
			auto op = [&] {
				if constexpr( off == 0 ) {
					// 0123'4567'8901'2345'6789'0123'4567'8901
					// XXfX XXXX Xff0 0001 1112 2223 333X XXXX
					// 1101'0010'100
					auto const fp0 = static_cast<std::uint32_t>( fp_full & 0xFFFFUL );
					std::uint32_t result = 0b1101'0010'1000'0000'0000'0000'0000'0000UL;
					result |= fp0 << 5U;
					return result;
				} else if constexpr( off == 16 ) {
					auto const fp16 =
					  static_cast<std::uint32_t>( ( fp_full >> 16 ) & 0xFFFFUL );
					std::uint32_t result = 0b1111'0010'1010'0000'0000'0000'0000'0000UL;
					result |= fp16 << 5U;
					return result;
				} else if constexpr( off == 32 ) {
					auto const fp32 =
					  static_cast<std::uint32_t>( ( fp_full >> 32 ) & 0xFFFFUL );
					std::uint32_t result = 0b1111'0010'1100'0000'0000'0000'0000'0000UL;
					result |= fp32 << 5U;
					return result;
				} else {
					static_assert( off == 48 );
					auto const fp48 =
					  static_cast<std::uint32_t>( ( fp_full >> 48 ) & 0xFFFFUL );
					std::uint32_t result = 0b1111'0010'1110'0000'0000'0000'0000'0000UL;
					result |= fp48 << 5U;
					return result;
				}
			}( );
			op |= reg;
			op_codes[0] = ( op >> 0U ) & 0xFFU;
			op_codes[1] = ( op >> 8U ) & 0xFFU;
			op_codes[2] = ( op >> 16U ) & 0xFFU;
			op_codes[3] = ( op >> 24U ) & 0xFFU;
		}
		void set_data_pointer( void *data_pointer ) {
			set_imm_mov<1, 0>( data_pointer, mov_x1_dp_lsl0 );
			set_imm_mov<1, 16>( data_pointer, mov_x1_dp_lsl16 );
			set_imm_mov<1, 32>( data_pointer, mov_x1_dp_lsl32 );
			set_imm_mov<1, 48>( data_pointer, mov_x1_dp_lsl48 );
		}
		void set_function_pointer( void *data_pointer ) {
			set_imm_mov<0, 0>( data_pointer, mov_x0_fp_lsl0 );
			set_imm_mov<0, 16>( data_pointer, mov_x0_fp_lsl16 );
			set_imm_mov<0, 32>( data_pointer, mov_x0_fp_lsl32 );
			set_imm_mov<0, 48>( data_pointer, mov_x0_fp_lsl48 );
		}
	};

	template<typename Thunk>
	constexpr void set_thunk_params( Thunk &th, void *user_data_pointer,
	                                 void *function_pointer ) {
		th->set_function_pointer( function_pointer );
		th->set_data_pointer( user_data_pointer );
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
