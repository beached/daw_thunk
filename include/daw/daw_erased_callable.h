// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#include <memory>
#include <type_traits>

#include <daw/daw_traits.h>

#include <cassert>

namespace daw {
	template<typename Func, typename Result, typename... Args>
	static Result invoke_callable( void *data, Args... args ) {
		assert( data );
		auto &callable = *reinterpret_cast<std::remove_reference_t<Func> *>( data );
		return callable( static_cast<Args>( args )... );
	}

	template<typename>
	struct erased_callable;

	template<typename Result, typename... Args>
	struct erased_callable<Result( Args... )> {
		using function_t = Result ( * )( void *, Args... );
		void *data = nullptr;
		function_t fp = nullptr;

	public:
		template<typename Func, std::enable_if_t<std::is_lvalue_reference_v<Func>,
		                                         std::nullptr_t> = nullptr>
		erased_callable( Func &&func ) noexcept
		  : data( static_cast<void *>(
		      const_cast<daw::remove_cvref_t<Func> *>( std::addressof( func ) ) ) )
		  , fp( invoke_callable<Func, Args...> ) {}

		template<typename Func,
		         std::enable_if_t<not std::is_lvalue_reference_v<Func>,
		                          std::nullptr_t> = nullptr>
		erased_callable( Func && ) = delete;
	};
} // namespace daw
