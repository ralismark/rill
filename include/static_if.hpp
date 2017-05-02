#pragma once

#include <utility>

namespace rill {

/**
 * \namespace cflow
 * \brief static control flow
 *
 * Despite the introuction of constexpr if into c++17, it is not well supported
 * (yet) and requires another way to support it.
 *
 * Use like this:
 *
 *      cflow::if_<true>([&] (auto&& id) {
 *              id(val).dothis();
 *      }).else_([&] (auto&& id) {
 *              id(val).dothat();
 *      });
 *
 * A function parameter is required to create a dependency on the argument.
 * Otherwise, both branches will be instantiated, resulting in errors.
 */

namespace cflow {

// required to create instantiation dependency
struct id {
	template <typename T>
	T operator()(T&& x) const {
		return std::forward<T>(x);
	}
};

struct nostatement
{
	template <typename Fn>
	void then_(const Fn&)
	{
	}

	template <typename Fn>
	void else_(const Fn&)
	{
	}

	template <bool, typename Fn>
	auto else_if_(const Fn&)
	{
		return nostatement{};
	}
};

template <bool Cond>
struct statement
{
	template <typename Fn>
	void then_(const Fn& fn)
	{
		fn(id{});
	}

	template <bool, typename Fn>
	auto else_if_(const Fn&)
	{
		return nostatement{};
	}

	template <typename Fn>
	void else_(const Fn&)
	{
	}
};

template <>
struct statement<false>
{
	template <typename Fn>
	void then_(const Fn&)
	{
	}

	template <bool Cond, typename Fn>
	auto else_if_(const Fn& fn)
	{
		statement<Cond> selection;
		selection.then_(fn);
		return selection;
	}

	template <typename Fn>
	void else_(const Fn& fn)
	{
		fn(id{});
	}
};

template <bool Cond, typename Fn>
auto if_(const Fn& fn)
{
	statement<Cond> selection;
	selection.then_(fn);
	return selection;
}

} // namespace cflow

} // namespace rill
