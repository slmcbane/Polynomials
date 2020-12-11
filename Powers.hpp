#ifndef POLYNOMIAL_POWERS_HPP
#define POLYNOMIAL_POWERS_HPP

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

namespace Polynomials
{

template <unsigned... Ps>
struct Powers
{
    constexpr Powers() noexcept {}
    constexpr static auto sum = (Ps + ...);

    static constexpr std::array<unsigned, sizeof...(Ps)> terms{ Ps... };

    template <int which>
    constexpr static unsigned term = terms[which];

    static constexpr std::size_t nvars = sizeof...(Ps);

    template <unsigned... Qs>
    constexpr auto operator*(Powers<Qs...>) const noexcept
    {
        static_assert(sizeof...(Ps) == sizeof...(Qs));
        return Powers<(Ps+Qs)...>{};
    }
};

template <unsigned... Ps, unsigned... Qs>
constexpr bool operator==(Powers<Ps...>, Powers<Qs...>) noexcept
{
    return std::is_same_v<Powers<Ps...>, Powers<Qs...>>;
}

template <unsigned... P1s, unsigned... P2s>
constexpr bool operator<(Powers<P1s...>, Powers<P2s...>) noexcept
{
    static_assert(sizeof...(P1s) == sizeof...(P2s), "Comparison of Powers types only meaningful if they are the same size");
    using Ps1 = Powers<P1s...>;
    using Ps2 = Powers<P2s...>;
    if constexpr (Ps1::sum < Ps2::sum)
    {
        return true;
    }
    else if constexpr (Ps1::sum > Ps2::sum)
    {
        return false;
    }
    else
    {
        constexpr unsigned terms1[sizeof...(P1s)] = { P1s... };
        constexpr unsigned terms2[sizeof...(P1s)] = { P2s... };
        for (unsigned i = 0; i < sizeof...(P1s); ++i)
        {
            if (terms1[i] < terms2[i])
            {
                return true;
            }
            else if (terms1[i] > terms2[i])
            {
                return false;
            }
        }
        return false;
    }
}

namespace detail
{

template <class T>
struct is_powers
{
    constexpr static bool value = false;
};

template <unsigned... Ps>
struct is_powers<Powers<Ps...>>
{
    constexpr static bool value = true;
};

template <class... Ps>
struct all_are_powers;

template <>
struct all_are_powers<>
{
    static constexpr bool value = true;
};

template <class P, class... Ps>
struct all_are_powers<P, Ps...>
{
    static constexpr bool value = is_powers<P>::value && all_are_powers<Ps...>::value;
};

template <class... Ps>
struct all_same_size;

template <>
struct all_same_size<> : public std::true_type {};

template <class P>
struct all_same_size<P> : public std::true_type {};

template <class P, class Q, class... Ps>
struct all_same_size<P, Q, Ps...> : public std::bool_constant<P::nvars == Q::nvars && all_same_size<Ps...>::value> {};

template <class... Ps>
struct sorted;

template <>
struct sorted<> : public std::true_type {};

template <class P>
struct sorted<P> : public std::true_type {};

template <class P1, class P2, class... Ps>
struct sorted<P1, P2, Ps...> : public std::bool_constant<!(P2{} < P1{}) && sorted<P2, Ps...>::value> {};

template <std::size_t... Is, class... Ps>
constexpr auto tuple_tail(std::index_sequence<Is...>, std::tuple<Ps...>)
{
    return std::tuple{ std::get<Is+2>(std::tuple{Ps{}...})... };
}

template <class... Ps>
constexpr auto tuple_tail(std::tuple<Ps...>)
{
    constexpr auto Is = std::make_index_sequence<sizeof...(Ps)-2>();
    return tuple_tail(Is, std::tuple{Ps{}...});
}

template <class... Ps>
constexpr auto do_sort(std::tuple<Ps...>) noexcept
{
    if constexpr (sorted<Ps...>::value)
    {
        return std::tuple{Ps{}...};
    }
    else
    {
        constexpr std::tuple<Ps...> terms{ Ps{}... };
        if constexpr (std::get<1>(terms) < std::get<0>(terms))
        {
            return do_sort(std::tuple_cat(std::tuple{std::get<1>(terms)}, 
                do_sort(std::tuple_cat(std::tuple{std::get<0>(terms)}, do_sort(tuple_tail(std::tuple{Ps{}...}))))));
        }
        else
        {
            return do_sort(std::tuple_cat(std::tuple{std::get<0>(terms)},
                do_sort(std::tuple_cat(std::tuple{std::get<1>(terms)}, do_sort(tuple_tail(std::tuple{Ps{}...}))))));
        }
    }
}

template <class... Ps>
struct powers_sorter
{
    static constexpr auto value = do_sort(std::tuple{Ps{}...});
};

} // namespace detail


template <class... Ps>
struct PowersList
{
    static_assert(detail::all_are_powers<Ps...>::value, "PowersList can only contain Powers objects");
    static_assert(detail::all_same_size<Ps...>::value, "All Powers in PowersList should have same order (number of variables)");
    constexpr static auto size = sizeof...(Ps);
    static constexpr std::tuple<Ps...> terms { Ps{}... };
    static constexpr std::size_t nvars = std::get<0>(terms).nvars;

    template <int which>
    static constexpr auto term = std::get<which>(terms);

    template <class... Qs>
    constexpr auto operator+(PowersList<Qs...>) const noexcept
    {
        return PowersList<Ps..., Qs...>{};
    }
};

namespace detail
{
    template <class... Ps>
    constexpr PowersList<Ps...> list_from_tuple(std::tuple<Ps...>) noexcept
    {
        return PowersList<Ps...>{};
    }

    template <class... Ps>
    constexpr auto remove_dupe_impl(std::tuple<Ps...> t) noexcept
    {
        if constexpr (sizeof...(Ps) < 2)
        {
            return t;
        }
        else if constexpr (std::get<0>(t) == std::get<1>(t))
        {
            return remove_dupe_impl(std::tuple_cat(std::tuple(std::get<0>(t)), tuple_tail(t)));
        }
        else
        {
            return std::tuple_cat(
                std::tuple(std::get<0>(t)),
                remove_dupe_impl(std::tuple_cat(std::tuple(std::get<1>(t)), tuple_tail(t)))
            );
        }
    }
} // namespace detail

template <class... Ps>
constexpr auto sort(PowersList<Ps...>) noexcept
{
    return detail::list_from_tuple(detail::do_sort(std::tuple{Ps{}...}));
}

template <class... Ps>
constexpr auto remove_dupes(PowersList<Ps...>) noexcept
{
    return detail::list_from_tuple(detail::remove_dupe_impl(std::tuple(Ps{}...)));
}

template <unsigned P, class T>
constexpr T raise(T x) noexcept
{
    if constexpr (P == 0)
    {
        return 1;
    }
    else if constexpr (P == 1)
    {
        return x;
    }
    else
    {
        constexpr unsigned first_power = P / 2;
        constexpr unsigned second_power = P - first_power;
        return raise<first_power>(x) * raise<second_power>(x);
    }
}

namespace detail
{

template <class Tup, unsigned... Ps, std::size_t... Is>
constexpr auto raise_tuple_impl(const Tup &xs, Powers<Ps...>, std::index_sequence<Is...>) noexcept
{
    return (raise<Ps>(std::get<Is>(xs)) * ...);
}

}

template <class T, unsigned... Ps>
constexpr auto raise(Powers<Ps...>, const std::array<T, sizeof...(Ps)> &xs) noexcept
{
    return detail::raise_tuple_impl(xs, Powers<Ps...>{}, std::make_index_sequence<sizeof...(Ps)>());
}

template <class... Ts, unsigned... Ps>
constexpr auto raise(Powers<Ps...>, const std::tuple<Ts...> &xs) noexcept
{
    return detail::raise_tuple_impl(xs, Powers<Ps...>{}, std::make_index_sequence<sizeof...(Ps)>());
}

template <class... Ts, unsigned... Ps>
constexpr auto raise(Powers<Ps...>, Ts... xs) noexcept
{
    return (raise<Ps>(xs) * ...);
}

template <unsigned... Ps, class... Qs>
constexpr auto operator*(Powers<Ps...>, PowersList<Qs...>) noexcept
{
    return PowersList<decltype(Powers<Ps...>{} * Qs{})...>{};
}

template <class... Ps, class... Qs>
constexpr auto operator*(PowersList<Ps...>, PowersList<Qs...>) noexcept
{
    return ((Ps{} * PowersList<Qs...>{}) + ...);
}

} // namespace Polynomials

#endif // POLYNOMIAL_POWERS_HPP
