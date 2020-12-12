#include "Polynomial.hpp"
#include "doctest.hpp"

using Polynomials::make_poly;
using Polynomials::PowersList;
using Polynomials::Powers;

TEST_CASE("Squaring a single-variable polynomial")
{
    constexpr auto powers = PowersList<Powers<2>, Powers<1>, Powers<0>>{};
    constexpr auto coeffs = std::tuple(1, -1, 2);
    constexpr auto poly = make_poly(coeffs, powers);
    constexpr auto poly2 = poly * poly;

    static_assert(std::is_same_v<
        std::decay_t<decltype(poly2)>,
        Polynomials::Polynomial<int, Powers<0>, Powers<1>, Powers<2>, Powers<3>, Powers<4>>>);

    REQUIRE(poly2.coeffs()[0] == 4);
    REQUIRE(poly2.coeffs()[1] == -4);
    REQUIRE(poly2.coeffs()[2] == 5);
    REQUIRE(poly2.coeffs()[3] == -2);
    REQUIRE(poly2.coeffs()[4] == 1);
}

TEST_CASE("A more complicated, multi-variable test case")
{
    constexpr auto powers = PowersList<Powers<0, 0>, Powers<0, 1>, Powers<1, 0>, Powers<1, 1>>{};
    constexpr auto coeffs1 = std::array<int, 4>{ 2, 1, 2, 3 };
    constexpr auto coeffs2 = std::array<int, 4>{ 4, 3, 2, 1 };
    constexpr auto poly1 = make_poly(coeffs1, powers);
    constexpr auto poly2 = make_poly(coeffs2, powers);
    constexpr auto result = poly1 * poly2;

    static_assert(result.num_terms == 9);
    REQUIRE(result.coeffs()[0] == 8);
    REQUIRE(result.coeffs()[1] == 10);
    REQUIRE(result.coeffs()[2] == 3);
    REQUIRE(result.coeffs()[3] == 12);
    REQUIRE(result.coeffs()[4] == 22);
    REQUIRE(result.coeffs()[5] == 10);
    REQUIRE(result.coeffs()[6] == 4);
    REQUIRE(result.coeffs()[7] == 8);
    REQUIRE(result.coeffs()[8] == 3);
}