#include "doctest.hpp"
#include "Polynomial.hpp"

using Polynomials::make_poly;
using Polynomials::Powers;
using Polynomials::PowersList;

TEST_CASE("Add two single-variable polynomials with same powers")
{
    constexpr auto coeffs1 = std::tuple(1, 2, 3, 4);
    constexpr auto coeffs2 = std::tuple(5, 6, 7, 8);
    constexpr auto powers = PowersList<Powers<0>, Powers<1>, Powers<2>, Powers<3>>{};
    constexpr auto poly1 = make_poly(coeffs1, powers);
    constexpr auto poly2 = make_poly(coeffs2, powers);
    constexpr auto result = poly1 + poly2;

    REQUIRE(result.num_terms == 4);
    REQUIRE(result.coeffs()[0] == 6);
    REQUIRE(result.coeffs()[1] == 8);
    REQUIRE(result.coeffs()[2] == 10);
    REQUIRE(result.coeffs()[3] == 12);
}

TEST_CASE("Add two single-variable polynomials with same powers, in place")
{
    constexpr auto coeffs1 = std::tuple(1, 2, 3, 4);
    constexpr auto coeffs2 = std::tuple(5, 6, 7, 8);
    constexpr auto powers = PowersList<Powers<0>, Powers<1>, Powers<2>, Powers<3>>{};
    auto poly1 = make_poly(coeffs1, powers);
    constexpr auto poly2 = make_poly(coeffs2, powers);
    poly1 += poly2;

    REQUIRE(poly1.coeffs()[0] == 6);
    REQUIRE(poly1.coeffs()[1] == 8);
    REQUIRE(poly1.coeffs()[2] == 10);
    REQUIRE(poly1.coeffs()[3] == 12);
}

TEST_CASE("Add two single-variable polynomials with different powers")
{
    constexpr auto coeffs1 = std::tuple(1, 2, 3, 4);
    constexpr auto coeffs2 = std::tuple(5, 6, 7, 8);
    constexpr auto powers1 = PowersList<Powers<0>, Powers<1>, Powers<2>, Powers<3>>{};
    constexpr auto powers2 = PowersList<Powers<2>, Powers<3>, Powers<4>, Powers<5>>{};
    constexpr auto poly1 = make_poly(coeffs1, powers1);
    constexpr auto poly2 = make_poly(coeffs2, powers2);
    constexpr auto result = poly1 + poly2;

    REQUIRE(result.num_terms == 6);
    REQUIRE(result.coeffs()[0] == 1);
    REQUIRE(result.coeffs()[1] == 2);
    REQUIRE(result.coeffs()[2] == 8);
    REQUIRE(result.coeffs()[3] == 10);
    REQUIRE(result.coeffs()[4] == 7);
    REQUIRE(result.coeffs()[5] == 8);
}