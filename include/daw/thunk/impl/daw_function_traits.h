// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/json_rpc
//

#pragma once

#include <daw/daw_fwd_pack_apply.h>
#include <daw/daw_traits.h>

namespace daw::func {
	namespace impl {
		template<typename A>
		using type_t = typename A::type;

		template<template<std::size_t> class Arguments, typename>
		struct make_arg_pack;

		template<template<std::size_t> class Arguments, std::size_t... Is>
		struct make_arg_pack<Arguments, std::index_sequence<Is...>> {
			using type = fwd_pack<type_t<Arguments<Is>>...>;
		};
	} // namespace impl
	template<typename, typename = void>
	struct function_traits;

	// function pointer
	template<typename R, typename... Args>
	struct function_traits<R ( * )( Args... )> : function_traits<R( Args... )> {};

	template<typename R, typename... Args>
	struct function_traits<R( Args... )> {
		using result_t = R;
		using params_t = fwd_pack<Args...>;

		static constexpr std::size_t arity = sizeof...( Args );

		template<std::size_t N>
		struct argument {
			static_assert( N < arity, "error: invalid parameter index." );
			using type = typename pack_element<N, fwd_pack<Args...>>::type;
		};
	};

	// member function pointer
	template<typename C, typename R, typename... Args>
	struct function_traits<R ( C::* )( Args... )>
	  : function_traits<R( C &, Args... )> {};

	// const member function pointer
	template<typename C, typename R, typename... Args>
	struct function_traits<R ( C::* )( Args... ) const>
	  : function_traits<R( C &, Args... )> {};

	// member object pointer
	template<typename C, typename R>
	struct function_traits<R( C::* )> : function_traits<R( C & )> {};

	// functor
	template<typename F>
	struct function_traits<F, std::enable_if_t<std::is_class_v<F>>> {
	private:
		using call_type = function_traits<decltype( &F::operator( ) )>;

	public:
		using result_t = typename call_type::result_t;

		static constexpr std::size_t arity = call_type::arity - 1;
		template<std::size_t N>
		struct argument {
			static_assert( N < arity, "error: invalid parameter index." );
			using type = typename call_type::template argument<N + 1>::type;
		};

		using params_t =
		  typename impl::make_arg_pack<argument,
		                               std::make_index_sequence<arity>>::type;
	};
} // namespace daw::func
