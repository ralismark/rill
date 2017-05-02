#pragma once

#include <utility>

#include "check.hpp"

namespace rill {

namespace concept {

/*
 * concept/source
 */
struct source
{
	template <typename T>
	auto requires(T&& src) -> decltype(concept::valid_expr(
			concept::exists<typename T::char_type>(),
			src.read(std::declval<typename T::char_type*>(), std::declval<std::size_t>())
		));
};

template <typename T>
constexpr bool Source()
{
	return concept::models<concept::source, T>::value;
}

/*
 * concept/sink
 */
struct sink
{
	template <typename T>
	auto requires(T&& sink) -> decltype(concept::valid_expr(
			concept::exists<typename T::char_type>(),
			sink.write(std::declval<const typename T::char_type*>(), std::declval<std::size_t>())
		));
};

template <typename T>
constexpr bool Sink()
{
	return concept::models<concept::sink, T>::value;
}

} // namespace concept

} // namespace rill
