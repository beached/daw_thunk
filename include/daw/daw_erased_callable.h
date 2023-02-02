// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#include "daw_function_traits.h"

#include <daw/daw_traits.h>

#include <cassert>
#include <memory>
#include <type_traits>

namespace daw {

	template<typename>
	struct erased_callable;

	template<typename Result, typename... Args>
	struct erased_callable<Result( Args... )> {
		using function_t = daw::traits::make_fp<Result( void *, Args... )>;
		void *data = nullptr;
		function_t fp = nullptr;

		template<typename Func>
		static Result invoke_callable( void *data, Args... args ) {
			assert( data );
			auto &callable =
			  *reinterpret_cast<std::remove_reference_t<Func> *>( data );
			return callable( static_cast<Args>( args )... );
		}

	public:
		template<typename Func, std::enable_if_t<std::is_lvalue_reference_v<Func>,
		                                         std::nullptr_t> = nullptr>
		erased_callable( Func &&func ) noexcept
		  : data( static_cast<void *>(
		      const_cast<daw::remove_cvref_t<Func> *>( std::addressof( func ) ) ) )
		  , fp( invoke_callable<Func> ) {}

		template<typename Func,
		         std::enable_if_t<not std::is_lvalue_reference_v<Func>,
		                          std::nullptr_t> = nullptr>
		erased_callable( Func && ) = delete;
	};

	namespace erased_callable_impl {
		template<typename FT, std::size_t... Is>
		auto make_erased_callable_fn( std::index_sequence<Is...> )
		  -> erased_callable<
		    typename FT::result_t( typename FT::template argument<Is>::type... )>;

		template<typename FT>
		using make_erased_callable_t = decltype( make_erased_callable_fn<FT>(
		  std::make_index_sequence<FT::arity>{ } ) );
	} // namespace erased_callable_impl

	template<typename Func>
	constexpr auto make_erased_callable( Func &f )
	  -> erased_callable_impl::make_erased_callable_t<
	    daw::func::function_traits<Func>> {
		return { f };
	}

} // namespace daw
