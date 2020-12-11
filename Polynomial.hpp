#ifndef POLYNOMIAL_POLYNOMIAL_HPP
#define POLYNOMIAL_POLYNOMIAL_HPP

#include "Powers.hpp"

#include <array>
#include <utility>

namespace Polynomials
{

namespace detail
{

template <class T, class Tuple, std::size_t... Is>
constexpr std::array<T, sizeof...(Is)> to_array_impl(std::index_sequence<Is...>, const Tuple &tup) noexcept
{
    return std::array<T, sizeof...(Is)>{std::get<Is>(tup)...};
}

template <class T, class... Ts>
constexpr std::array<T, sizeof...(Ts)> to_array(const std::tuple<Ts...> &tup) noexcept
{
    return to_array_impl<T>(std::make_index_sequence<sizeof...(Ts)>(), tup);
}

struct PolyMaker;

} // namespace detail

template <class T, class... Ps>
class Polynomial
{
    std::array<T, sizeof...(Ps)> m_coeffs;

    constexpr Polynomial(const std::array<T, sizeof...(Ps)> &cs) noexcept : m_coeffs(cs) {}

    constexpr Polynomial() noexcept : m_coeffs{0} {}

    template <class... Ts, class = std::enable_if_t<sizeof...(Ts) == sizeof...(Ps)>>
    constexpr Polynomial(const std::tuple<Ts...> &cs) noexcept : m_coeffs{detail::to_array<T>(cs)}
    {
    }

    friend struct detail::PolyMaker;

  public:
    constexpr const auto &coeffs() const noexcept { return m_coeffs; }
    static constexpr auto num_terms = sizeof...(Ps);

    constexpr Polynomial<T, Ps...> operator+(const Polynomial<T, Ps...> &other) const noexcept
    {
        Polynomial<T, Ps...> result;
        for (unsigned i = 0; i < sizeof...(Ps); ++i)
        {
            result.m_coeffs[i] = m_coeffs[i] + other.m_coeffs[i];
        }
        return result;
    }

    constexpr Polynomial<T, Ps...> &operator+=(const Polynomial<T, Ps...> &other) noexcept
    {
        for (unsigned i = 0; i < sizeof...(Ps); ++i)
        {
            m_coeffs[i] += other.m_coeffs[i];
        }
        return *this;
    }
};

namespace detail
{

struct PolyMaker
{
    template <class T, class... Ps>
    constexpr static auto create(const std::array<T, sizeof...(Ps)> &coeffs, PowersList<Ps...>) noexcept
    {
        return Polynomial<T, Ps...>(coeffs);
    }

    template <class... Ts, class... Ps>
    constexpr static auto create(const std::tuple<Ts...> &coeffs, PowersList<Ps...>) noexcept
    {
        static_assert(
            sizeof...(Ts) == sizeof...(Ps), "Length of coeffs should match number of Powers terms");
        using T = std::common_type_t<std::decay_t<Ts>...>;

        return Polynomial<T, Ps...>(coeffs);
    }
};

template <class P, class Q, class... Qs>
constexpr std::size_t map_index(P, std::tuple<Q, Qs...>) noexcept
{
    if constexpr (std::is_same_v<P, Q>)
    {
        return 0;
    }
    else
    {
        return 1 + map_index(P{}, std::tuple<Qs...>{});
    }
}

template <std::size_t... Is, class... Ps, class... Qs>
constexpr std::array<std::size_t, sizeof...(Ps)>
    map_indices(std::index_sequence<Is...>, PowersList<Ps...>, PowersList<Qs...>) noexcept
{
    constexpr auto ps = std::tuple(Ps{}...);
    constexpr auto qs = std::tuple(Qs{}...);
    return std::array<std::size_t, sizeof...(Ps)>{map_index(std::get<Is>(ps), qs)...};
}

template <class... Ps, class... Qs>
constexpr std::array<std::size_t, sizeof...(Ps)> map_indices(PowersList<Ps...>, PowersList<Qs...>) noexcept
{
    constexpr auto seq = std::make_index_sequence<sizeof...(Ps)>();
    return map_indices(seq, PowersList<Ps...>{}, PowersList<Qs...>{});
}

template <class T, std::size_t Size, std::size_t FinalSize>
constexpr auto collect_coeffs(
    const std::array<T, Size> &coeffs, const std::array<std::size_t, Size> &mapped_indices,
    std::integral_constant<std::size_t, FinalSize>) noexcept
{
    std::array<T, FinalSize> final_coeffs{0};
    for (unsigned i = 0; i < Size; ++i)
    {
        final_coeffs[mapped_indices[i]] += coeffs[i];
    }

    return final_coeffs;
}

template <class... Ts, std::size_t... Is>
constexpr auto to_array_impl(const std::tuple<Ts...> &t, std::index_sequence<Is...>) noexcept
{
    static_assert(sizeof...(Ts) == sizeof...(Is));
    using T = std::common_type_t<Ts...>;
    return std::array<T, sizeof...(Ts)>{std::get<Is>(t)...};
}

template <class... Ts, std::size_t FinalSize>
constexpr auto collect_coeffs(
    const std::tuple<Ts...> &coeffs, const std::array<std::size_t, sizeof...(Ts)> &mapped_indices,
    std::integral_constant<std::size_t, FinalSize>) noexcept
{
    return collect_coeffs(
        to_array_impl(coeffs, std::make_index_sequence<sizeof...(Ts)>()), mapped_indices,
        std::integral_constant<std::size_t, FinalSize>{});
}

} // namespace detail

template <class C, class... Ps>
constexpr auto make_poly(const C &coeffs, PowersList<Ps...>) noexcept
{
    static_assert(std::tuple_size_v<C> == sizeof...(Ps));
    constexpr auto final_powers = remove_dupes(sort(PowersList<Ps...>{}));
    constexpr auto mapped_indices = detail::map_indices(PowersList<Ps...>{}, final_powers);

    return detail::PolyMaker::create(
        detail::collect_coeffs(
            coeffs, mapped_indices, std::integral_constant<std::size_t, final_powers.size>{}),
        final_powers);
}

template <class T, class... Ps, class U, class... Qs>
constexpr auto operator+(const Polynomial<T, Ps...> &p, const Polynomial<U, Qs...> &q) noexcept
{
    using V = std::common_type_t<T, U>;
    std::array<V, sizeof...(Ps) + sizeof...(Qs)> coeffs{ 0 };
    for (unsigned i = 0; i < sizeof...(Ps); ++i)
    {
        coeffs[i] = p.coeffs()[i];
    }
    for (unsigned i = 0; i < sizeof...(Qs); ++i)
    {
        coeffs[i + p.num_terms] = q.coeffs()[i];
    }
    return make_poly(coeffs, PowersList<Ps..., Qs...>{});
}

} // namespace Polynomials

#endif // POLYNOMIAL_POLYNOMIAL_HPP
