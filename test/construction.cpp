#include "Polynomial.hpp"
#include "doctest.hpp"

using Polynomials::make_poly;
using Polynomials::Powers;
using Polynomials::PowersList;

TEST_CASE("Construct a polynomial with coefficient array")
{
    SUBCASE("Only a single coefficient")
    {
        constexpr auto powers = PowersList<Powers<0>>{};
        constexpr auto coeffs = std::array<double, 1>{1.0};
        constexpr auto poly = make_poly(coeffs, powers);
        REQUIRE(poly.coeffs()[0] == 1.0);
    }

    SUBCASE("Single-variable polynomial, powers are sorted, no duplicates")
    {
        constexpr auto powers = PowersList<Powers<0>, Powers<1>, Powers<2>, Powers<3>>{};
        constexpr std::array<double, 4> coeffs{1.0, 2.0, 3.0, 4.0};
        constexpr auto poly = make_poly(coeffs, powers);

        REQUIRE(poly.coeffs()[0] == 1.0);
        REQUIRE(poly.coeffs()[1] == 2.0);
        REQUIRE(poly.coeffs()[2] == 3.0);
        REQUIRE(poly.coeffs()[3] == 4.0);
    }

    SUBCASE("Single-variable polynomial, powers are not sorted, no duplicates")
    {
        constexpr auto powers = PowersList<Powers<0>, Powers<3>, Powers<2>, Powers<1>>{};
        constexpr std::array<double, 4> coeffs{1.0, 2.0, 3.0, 4.0};
        constexpr auto poly = make_poly(coeffs, powers);

        REQUIRE(poly.coeffs()[0] == 1.0);
        REQUIRE(poly.coeffs()[1] == 4.0);
        REQUIRE(poly.coeffs()[2] == 3.0);
        REQUIRE(poly.coeffs()[3] == 2.0);
    }

    SUBCASE("Single-variable polynomial, unsorted and duplicate powers")
    {
        constexpr auto powers = PowersList<
            Powers<3>, Powers<4>, Powers<4>, Powers<2>, Powers<1>, Powers<3>, Powers<2>, Powers<0>,
            Powers<1>>{};
        constexpr std::array<int, powers.size> coeffs{3, 4, 5, 4, 3, 5, 3, 5, 3};
        constexpr auto poly = make_poly(coeffs, powers);

        REQUIRE(poly.num_terms == 5);

        REQUIRE(poly.coeffs()[0] == 5);
        REQUIRE(poly.coeffs()[1] == 6);
        REQUIRE(poly.coeffs()[2] == 7);
        REQUIRE(poly.coeffs()[3] == 8);
        REQUIRE(poly.coeffs()[4] == 9);
    }

    SUBCASE("Multiple variables, unsorted and duplicate powers")
    {
        constexpr auto powers = PowersList<
            Powers<1, 0>, Powers<0, 0>, Powers<1, 0>, Powers<0, 1>, Powers<1, 1>, Powers<0, 1>>{};
        constexpr std::array<int, powers.size> coeffs{2, 3, 4, 5, 6, 7};
        constexpr auto poly = make_poly(coeffs, powers);

        REQUIRE(poly.num_terms == 4);

        REQUIRE(poly.coeffs()[0] == 3);
        REQUIRE(poly.coeffs()[1] == 12);
        REQUIRE(poly.coeffs()[2] == 6);
        REQUIRE(poly.coeffs()[3] == 6);
    }
}

TEST_CASE("Construct a polynomial with a coefficient tuple")
{
    SUBCASE("Single-variable polynomial, powers are sorted, no duplicates")
    {
        constexpr auto powers = PowersList<Powers<0>, Powers<1>, Powers<2>, Powers<3>>{};
        constexpr auto coeffs = std::tuple(1, 2, 3, '\4');
        constexpr auto poly = make_poly(coeffs, powers);

        REQUIRE(std::is_same_v<
                std::remove_cv_t<decltype(poly)>,
                Polynomials::Polynomial<int, Powers<0>, Powers<1>, Powers<2>, Powers<3>>>);

        REQUIRE(poly.coeffs()[0] == 1);
        REQUIRE(poly.coeffs()[1] == 2);
        REQUIRE(poly.coeffs()[2] == 3);
        REQUIRE(poly.coeffs()[3] == 4);
    }

    SUBCASE("Single-variable polynomial, powers are not sorted, no duplicates")
    {
        constexpr auto powers = PowersList<Powers<0>, Powers<3>, Powers<2>, Powers<1>>{};
        constexpr auto coeffs = std::tuple(1.0, 2.0, 3.0, 4.0);
        constexpr auto poly = make_poly(coeffs, powers);

        REQUIRE(poly.coeffs()[0] == 1.0);
        REQUIRE(poly.coeffs()[1] == 4.0);
        REQUIRE(poly.coeffs()[2] == 3.0);
        REQUIRE(poly.coeffs()[3] == 2.0);
    }

    SUBCASE("Single-variable polynomial, unsorted and duplicate powers")
    {
        constexpr auto powers = PowersList<
            Powers<3>, Powers<4>, Powers<4>, Powers<2>, Powers<1>, Powers<3>, Powers<2>, Powers<0>,
            Powers<1>>{};
        constexpr auto coeffs = std::tuple(3, 4, 5, 4, 3, 5, 3, 5, 3);
        constexpr auto poly = make_poly(coeffs, powers);

        REQUIRE(poly.num_terms == 5);

        REQUIRE(poly.coeffs()[0] == 5);
        REQUIRE(poly.coeffs()[1] == 6);
        REQUIRE(poly.coeffs()[2] == 7);
        REQUIRE(poly.coeffs()[3] == 8);
        REQUIRE(poly.coeffs()[4] == 9);
    }

    SUBCASE("Multiple variables, unsorted and duplicate powers")
    {
        constexpr auto powers = PowersList<
            Powers<1, 0>, Powers<0, 0>, Powers<1, 0>, Powers<0, 1>, Powers<1, 1>, Powers<0, 1>>{};
        constexpr auto coeffs = std::tuple(2, 3, 4, 5, 6, 7);
        constexpr auto poly = make_poly(coeffs, powers);

        REQUIRE(poly.num_terms == 4);

        REQUIRE(poly.coeffs()[0] == 3);
        REQUIRE(poly.coeffs()[1] == 12);
        REQUIRE(poly.coeffs()[2] == 6);
        REQUIRE(poly.coeffs()[3] == 6);
    }
}