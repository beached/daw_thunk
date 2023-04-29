// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_thunk
//

#pragma once

#include "thunk/impl/daw_function_traits.h"

#include <daw/daw_attributes.h>
#include <daw/daw_traits.h>

#include <cassert>
#include <memory>
#include <type_traits>

namespace daw {

	template<typename>
	struct erased_callable;

	/// \brief Construct a type erased callable suitable to pass to api's with a
	/// function pointer/void * state first param \tparam Result The result type
	/// of the function \tparam Params The parameter types of the function
	template<typename Result, typename... Params>
	struct erased_callable<Result( Params... )> {
		using function_t = daw::traits::make_fp<Result( void *, Params... )>;
		void *data = nullptr;
		function_t fp = nullptr;

		template<typename Func>
		DAW_ATTRIB_INLINE static Result invoke_callable( void *data,
		                                                 Params... args ) {
			assert( data );
			auto &callable =
			  *reinterpret_cast<std::remove_reference_t<Func> *>( data );
			return callable( static_cast<Params>( args )... );
		}

	public:
		template<typename Func, std::enable_if_t<std::is_lvalue_reference_v<Func>,
		                                         std::nullptr_t> = nullptr>
		explicit erased_callable( Func &&func ) noexcept
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

	/// \brief A function to help make erased_callable types by deducing the
	/// result type and parameter types from the func provided \tparam Func class
	/// type of the function \param f function object \return A erased_callable
	/// with the appropriate Result and Params... types
	template<typename Func>
	constexpr auto make_erased_callable( Func &f ) {
		return erased_callable_impl::make_erased_callable_t<
		  daw::func::function_traits<std::remove_reference_t<Func>>>{ f };
	}
} // namespace daw
