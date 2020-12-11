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
