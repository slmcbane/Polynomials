#include "doctest.hpp"
#include "Polynomial.hpp"

using Polynomials::Powers;
using Polynomials::PowersList;
using Polynomials::make_poly;

TEST_CASE("Evaluating a multivariable polynomial")
{
    constexpr auto powers = PowersList<Powers<3, 0>, Powers<0, 3>, Powers<2, 1>, Powers<1, 2>,
        Powers<2, 0>, Powers<0, 2>, Powers<1, 1>, Powers<1, 0>, Powers<0, 1>, Powers<0, 0>>{};

    constexpr std::array<int, powers.size> coeffs { 2, 1, -3, 4, 0, 5, 1, 1, 2, 1 };

    constexpr auto poly = make_poly(coeffs, powers);

    REQUIRE(poly(1, 2) == 48);
    REQUIRE(poly(2, 2) == 63);
    REQUIRE(poly(4.0, '\2') == 141);
}