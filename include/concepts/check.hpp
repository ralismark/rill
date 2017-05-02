#pragma once

#include <type_traits>
#include <utility>

/**
 * \file
 * \brief concept check framework
 *
 * This implements a basic concept checking framework based on the concept
 * checking in Eric Niebler's ranges-v3. Concepts are defines with a similar
 * syntax:
 *
 * \code {.cpp}
 * // see http://en.cppreference.com/w/cpp/concept/CopyConstructible
 * struct CopyConstructible
 *      : concept::refines<MoveConstructible>
 * {
 *      template <typename T>
 *      auto requires(T&&) -> decltype(concept::valid_expr(
 *                      T(std::declval<T&>()),
 *                      T(std::declval<const T&>()),
 *                      T(std::declval<T&&>())
 *              ));
 * };
 * \endcode
 */

namespace rill {

namespace concept {

/**
 * \struct True
 * \brief Concept matching anything
 *
 * Every complete type models this, as it imposes no requirements. As a result,
 * it can be used when a true 'type' is needed e.g. as the base of a refinement
 * tree (see \ref refines).
 */
struct True
{
	template <typename... Ts>
	auto requires(Ts&&...) -> void;
};

/**
 * \namespace check
 * \brief concept check details
 *
 * This contains all the implementation details for concepts. Normally, you do
 * not need to know what is in here.
 */
namespace check {

/**
 * \typedef void_t
 * \brief make anything into void
 *
 * This is used to detect ill-formed expressions in SFINAE contexts. See
 * http://en.cppreference.com/w/cpp/types/void_t.
 */
template <typename...>
using void_t = void;

// forward declaration
template <typename Concept, typename... Ts>
struct models_t;

/**
 * \struct require_base
 * \brief get type being refined
 *
 * This determines what base concepts exist for a given concepts, allowing for
 * refinement of existing concepts.
 */
template <typename T, typename = void>
struct require_base
{
	using is_root = std::true_type;
	using type = True;
};

template <typename T>
struct require_base<T, void_t<typename T::_require_base_>>
{
	using is_root = std::false_type;
	using type = typename T::_require_base_;
};

/**
 * \fn models_
 * \brief concept checker
 *
 * This uses multiple function templates, as well as SFINAE, to determine if a
 * concept is modelled. This is done by determining if the return type of
 * 'requires' is well-formed.
 *
 * Do not use this explicitly; instead use \ref models.
 */

template <typename... Ts>
auto models_(...) -> std::false_type;

// matches above if fails
// matches below if most derived succeeds
template <typename... Ts, typename Concept,
	typename = decltype(
		std::declval<Concept&>()
			.template requires<Ts...>(std::declval<Ts>()...)
		)>
auto models_(Concept*) ->
	typename std::conditional<
		require_base<Concept>::is_root::value,
		std::true_type, // is base type
		typename models_t<typename require_base<Concept>::type, Ts...>::value_t // refines something
	>::type;

/**
 * \struct models_t
 * \brief helper for concept checking
 *
 * This uses \ref models_ to determine if the given types model a concept.
 *
 * Do not use explicitly; instead use \ref models.
 */
template <typename Concept, typename... Ts>
struct models_t
{
	using value_t = decltype(models_<Ts...>(static_cast<Concept*>(nullptr)));
};

/**
 * \struct refines_t
 * \brief helper for refining concepts
 *
 * This creates a typedef which allows the base concept to be determines.
 */
template <typename T>
struct refines_t
{
	using _require_base_ = T;
};

} // namespace check

/**
 * \typedef models
 * \brief check if types model a concept
 *
 * This checks the given types to see if they model a concept. This typedef
 * will resolve to either std::true_type or std::false_type.
 */
template <typename Concept, typename... Ts>
using models = typename check::models_t<Concept, Ts...>::value_t;

/**
 * \typedef refines
 * \brief refine another concept
 *
 * Concepts may require other concepts as prerequisites. Inherit from this to
 * do that. Currently, multiple bases are not supported, but will probably be
 * added in the near future.
 */
template <typename T>
using refines = check::refines_t<T>;

/**
 * \fn valid_expr
 * \brief check if the expressions passed are valid
 *
 * This can be used to check if all the given expressions are valid.
 */
template <typename... Ts>
void valid_expr(Ts&&...);

/**
 * \fn is_same
 * \brief check if the passed types are the same
 *
 * Use this with valid_expr to determine if the expressions have the same type.
 */
template <typename T1, typename T2>
auto valid_expr(T1&&, T2&&) -> std::enable_if_t<std::is_same<T1, T2>::value, int>;

/**
 * \fn model_of
 * \brief check if the passed types model a concept
 */
template <typename Concept, typename... Ts>
auto model_of(Ts&&...) -> std::enable_if_t<concept::models<Concept, Ts...>::value, int>;

/**
 * \typedef exists
 * \brief check if types are valid
 *
 * This is similar to void_t, and does pretty much the same thing
 */
template <typename... T>
using exists = int;

} // namespace concept

} // namespace rill
