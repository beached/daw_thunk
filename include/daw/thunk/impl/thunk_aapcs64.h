// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#include "thunk_arch.h"

#include <daw/daw_cxmath.h>

#include <cstddef>
#include <type_traits>

#if DAW_THUNK_ARCH != DAW_THUNK_ARCH_C_aapcs64
#error This header only works on aapcs64 arm64 architectures
#endif

namespace daw::thunk_impl {
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
	// clang-format on

	using arm64_op = std::uint32_t;
	template<std::uint32_t reg, unsigned char off>
	static constexpr void set_imm_mov( std::uint64_t value, arm64_op &op_codes ) {
		static_assert( reg < 17 );
		if constexpr( off == 0 ) {
			auto const fp0 = static_cast<std::uint32_t>( value & 0xFFFFU );
			std::uint32_t result = { 0xD280'0000U };
			result |= fp0 << 5U;
			result |= reg;
			op_codes = result;
		} else if constexpr( off == 16 ) {
			auto const fp16 = static_cast<std::uint32_t>( ( value >> 16 ) & 0xFFFFU );
			std::uint32_t result = { 0xF2A0'0000U };
			result |= fp16 << 5U;
			result |= reg;
			op_codes = result;
		} else if constexpr( off == 32 ) {
			auto const fp32 = static_cast<std::uint32_t>( ( value >> 32 ) & 0xFFFFU );
			std::uint32_t result = { 0xF2C0'0000U };
			result |= fp32 << 5U;
			result |= reg;
			op_codes = result;
		} else {
			static_assert( off == 48 );
			auto const fp48 = static_cast<std::uint32_t>( ( value >> 48 ) & 0xFFFFU );
			std::uint32_t result = { 0xF2E0'0000U };
			result |= fp48 << 5U;
			result |= reg;
			op_codes = result;
		}
	}

	template<std::uint32_t reg>
	constexpr void set_imm_mov( std::uint64_t value, arm64_op *op_codes ) {
		set_imm_mov<reg, 0>( value, op_codes[0] );
		set_imm_mov<reg, 16>( value, op_codes[1] );
		set_imm_mov<reg, 32>( value, op_codes[2] );
		set_imm_mov<reg, 48>( value, op_codes[3] );
	}

	template<std::uint32_t reg_d, std::uint32_t reg_s>
	constexpr std::uint32_t mov_reg( ) {
		static_assert( reg_d < 16 );
		static_assert( reg_s < 16 );
		std::uint32_t result = 0b10101010000'00000'000000'11111'00000U;
		result |= ( reg_s & 0b11111U ) << 16U;
		result |= ( reg_d & 0b11111U );
		return result;
	}

	template<std::size_t ParamCount, typename = void>
	struct alignas( 16 ) thunk {
		static_assert( ParamCount <= 8, "Only 8 or less parameters are supported" );
		std::uint32_t op_codes[ParamCount + 9];

		template<std::size_t N>
		static constexpr std::uint32_t op_mov( ) noexcept {
			constexpr auto dst = ( ParamCount - N );
			constexpr auto src = dst - 1;
			return mov_reg<dst, src>( );
		}

		template<std::size_t... Is>
		constexpr thunk( std::index_sequence<Is...> ) noexcept
		  : op_codes{ op_mov<Is>( )...,      0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
		              /*br x16*/ 0xD61F'0200 } {}

		constexpr thunk( ) noexcept
		  : thunk( std::make_index_sequence<ParamCount>{ } ) {}

		constexpr void set_data_pointer( std::uintptr_t addr ) {
			set_imm_mov<0>( addr, op_codes +
			                        static_cast<std::ptrdiff_t>( ParamCount ) + 4 );
		}

		constexpr void set_function_pointer( std::uintptr_t addr ) {
			set_imm_mov<16>( addr,
			                 op_codes + static_cast<std::ptrdiff_t>( ParamCount ) );
		}
	};

	template<typename Thunk>
	constexpr void set_thunk_params( Thunk &th, void *user_data_pointer,
	                                 void *function_pointer ) {
		th->set_function_pointer(
		  reinterpret_cast<std::uintptr_t>( function_pointer ) );
		th->set_data_pointer(
		  reinterpret_cast<std::uintptr_t>( user_data_pointer ) );
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
