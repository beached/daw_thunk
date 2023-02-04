// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#include "daw/daw_erased_callable.h"
#include "impl/daw_function_traits.h"
#include "impl/thunk_impl.h"

#include <daw/daw_attributes.h>

#include <cstring>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>

namespace daw {
	[[noreturn]] DAW_ATTRIB_INLINE void do_error( std::string_view /*sv*/ ) {
		std::terminate( );
		// throw std::runtime_error(static_cast<std::string>(sv));
	}
	namespace thunk_impl {
		template<std::size_t PassedParams>
		inline constexpr thunk<PassedParams> default_thunk{ };

		template<std::size_t Len>
		struct virtual_alloc_deleter {
			virtual_alloc_deleter( ) = default;

			inline void operator( )( void *ptr ) const {
				if( ::VirtualFree( ptr, 0, MEM_RELEASE ) == 0 ) {
					do_error( "Error unmapping region, check GetLastError" );
				}
			}
		};

	}; // namespace thunk_impl
	template<typename>
	struct Thunk;

	namespace thunk_impl {
		template<typename T>
		inline constexpr bool can_passthrough_thunk_v =
		  std::is_same_v<void, T> or std::is_trivially_copyable_v<T> or
		  std::is_reference_v<T>;
	}

	template<typename Result, typename... Params>
	struct Thunk<Result( Params... )> {
		static_assert(
		  thunk_impl::can_passthrough_thunk_v<Result>,
		  "Only trivially copyable types are currently supported. Passing by "
		  "reference or pointer may help" );
		static_assert(
		  ( thunk_impl::can_passthrough_thunk_v<Params> and ... ),
		  "Only trivially copyable types are currently supported. Passing by "
		  "reference or pointer may help" );
		static constexpr std::size_t param_count =
		  thunk_impl::calculate_size_v<Result, Params...>;
		using thunk_t = thunk_impl::thunk<param_count>;
		using uptr_thunk_t =
		  std::unique_ptr<thunk_t,
		                  thunk_impl::virtual_alloc_deleter<sizeof( thunk_t )>>;
		uptr_thunk_t thunk = nullptr;

		Thunk( ) = default;

		DAW_ATTRIB_NOINLINE explicit Thunk(
		  void *user_data_pointer,
		  Result ( *function_pointer )( void *, Params... ) ) {
			static_assert( sizeof( Result( * )( void *, Params... ) ) ==
			                 sizeof( void * ),
			               "Unexpected function pointer size" );
			void *tmp = ::VirtualAlloc( nullptr, sizeof( thunk_t ),
			                            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
			if( not tmp ) {
				do_error( "Error mapping region, check GetLastError" );
			}
			thunk = uptr_thunk_t( reinterpret_cast<thunk_t *>( memcpy(
			  tmp, &thunk_impl::default_thunk<param_count>, sizeof( thunk_t ) ) ) );

			thunk_impl::set_thunk_params(
			  thunk, user_data_pointer,
			  reinterpret_cast<void *>( function_pointer ) );
			auto old_protection = DWORD( );
			if( ::VirtualProtect( thunk.get( ), sizeof( thunk_t ), PAGE_EXECUTE_READ,
			                      &old_protection ) == 0 ) {
				do_error( "Error protecting region, check GetLastError" );
			}
		}

		explicit Thunk( erased_callable<Result( Params... )> const &ec )
		  : Thunk( ec.data, ec.fp ) {}

		template<typename Func,
		         std::enable_if_t<std::is_invocable_v<Func, Params...>,
		                          std::nullptr_t> = nullptr>
		explicit Thunk( Func &f )
		  : Thunk( erased_callable<Result( Params... )>( f ) ) {}

		using thunked_fp_t = daw::traits::make_fp<Result( Params... )>;
		thunked_fp_t get( ) const & {
			return reinterpret_cast<thunked_fp_t>( thunk.get( ) );
		}

		inline operator thunked_fp_t( ) const & {
			return get( );
		}
	};

	template<typename R, typename... Params>
	Thunk( void *, R ( * )( void *, Params... ) ) -> Thunk<R( Params... )>;

	template<typename R, typename... Params>
	Thunk( erased_callable<R( Params... )> ) -> Thunk<R( Params... )>;

	template<typename Func>
	auto make_thunk( Func &f ) {
		return Thunk( make_erased_callable( f ) );
	}
}; // namespace daw
