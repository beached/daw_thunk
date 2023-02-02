// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#include <daw/daw_attributes.h>

#include <cstring>
#include <memory>
#include <stdexcept>
#include <sys/mman.h>
#include <type_traits>

namespace daw {
	[[noreturn]] DAW_ATTRIB_INLINE void do_error( std::string_view /*sv*/ ) {
		std::terminate( );
		// throw std::runtime_error(static_cast<std::string>(sv));
	}
	namespace thunk_impl {

#if defined( __x86_64__ )
		template<std::size_t /*PassedParams*/>
		struct thunk;

		/***
		 * mov rdi, [state]
		 * mov rax, [function_ptr]
		 * jmp rax
		 */
		template<>
		struct __attribute__( ( packed ) ) thunk<0> {
			unsigned char mov_rdi[2] = { 0x48, 0xBF };
			void *state = nullptr;
			unsigned char mov_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
		};
		/***
		 * mov rsi, rdi
		 * mov rdi, [state]
		 * mov rax, [function_ptr]
		 * jmp rax
		 */
		template<>
		struct __attribute__( ( packed ) ) thunk<1> {
			unsigned char mov_rsi_rdi[3] = { 0x48, 0x89, 0xFE };
			unsigned char mov_rdi[2] = { 0x48, 0xBF };
			void *state = nullptr;
			unsigned char mov_rax[2] = { 0x48, 0xB8 };
			void *function_pointer = nullptr;
			unsigned char jmp_rax[2] = { 0xFF, 0xE0 };
		};
#elif defined( __i386__ )
		template<std::size_t /*PassedParams*/>
		struct __attribute__( ( packed ) ) thunk {
			unsigned char push = unsigned char{ 0x68 };
			void *state = nullptr;
			unsigned char call = unsigned char{ 0xE8 };
			long call_offset = 0;
			unsigned char add_esp[3] = { 0x83, 0xC4, 0x04 };
			unsigned char ret = unsigned char{ 0xC3 };
		};
#else
#error Architecture unsupported
#endif
		template<std::size_t PassedParams>
		inline constexpr thunk<PassedParams> default_thunk{ };

		template<std::size_t Len>
		struct mmap_deleter {
			mmap_deleter( ) = default;

			inline void operator( )( void *ptr ) const {
				if( munmap( ptr, Len ) == -1 ) {

					do_error( "Error unmapping region, check errno" );
				}
			}
		};

	}; // namespace thunk_impl
	template<typename>
	struct Thunk;

	template<typename T>
	inline constexpr bool can_passthrough_thunk_v =
	  std::is_same_v<void, T> or std::is_trivially_copyable_v<T> or
	  std::is_reference_v<T>;

	template<typename Result, typename... Params>
	struct Thunk<Result( Params... )> {
		static_assert(
		  can_passthrough_thunk_v<Result>,
		  "Only trivially copyable types are currently supported. Passing by "
		  "reference or pointer may help" );
		static_assert(
		  ( can_passthrough_thunk_v<Params> and ... ),
		  "Only trivially copyable types are currently supported. Passing by "
		  "reference or pointer may help" );
		using thunk_t = thunk_impl::thunk<sizeof...( Params )>;
		using uptr_thunk_t =
		  std::unique_ptr<thunk_t, thunk_impl::mmap_deleter<sizeof( thunk_t )>>;
		uptr_thunk_t thunk = nullptr;

		Thunk( ) = default;
		DAW_ATTRIB_NOINLINE Thunk( void *data,
		                           Result ( *code )( void *, Params... ) ) {
			void *tmp = ::mmap( 0, sizeof( thunk_t ), PROT_WRITE,
			                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
			if( tmp == MAP_FAILED ) {
				do_error( "Error mapping region" );
			}
			thunk = uptr_thunk_t( reinterpret_cast<thunk_t *>(
			  memcpy( tmp, &thunk_impl::default_thunk<sizeof...( Params )>,
			          sizeof( thunk_t ) ) ) );
#if defined( __x86_64__ )
			thunk->state = data;
			thunk->function_pointer = reinterpret_cast<void *>( code );
#else
			thunk->state = data;
			thunk->call_offset =
			  reinterpret_cast<void *>( code ) - (void *)&thunk->add_esp[0];
#endif
			if( mprotect( thunk.get( ), sizeof( thunk_t ), PROT_EXEC ) == -1 ) {
				do_error( "Error protecting region" );
			}
		}

		using thunked_fp_t = daw::traits::make_fp<Result( Params... )>;
		thunked_fp_t get( ) const {
			return reinterpret_cast<thunked_fp_t>( thunk.get( ) );
		}
	};

} // namespace daw
