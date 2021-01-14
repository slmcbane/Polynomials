#include "Polynomial.hpp"
#include "doctest.hpp"

using namespace Polynomials;

TEST_CASE("Partial derivatives of a single-variable function")
{
    constexpr auto powers = PowersList<Powers<0>, Powers<1>, Powers<2>, Powers<3>>{};
    constexpr auto coefficients = std::array{1.0, 2.0, 3.0, 4.0};
    constexpr auto poly = make_poly(coefficients, powers);
    constexpr auto polyprime = partial<0>(poly);

    static_assert(
        std::is_same_v<
            std::remove_cv_t<decltype(polyprime)>, Polynomial<double, Powers<0>, Powers<1>, Powers<2>>>);

    REQUIRE(polyprime.coeffs()[0] == 2.0);
    REQUIRE(polyprime.coeffs()[1] == 6.0);
    REQUIRE(polyprime.coeffs()[2] == 12.0);
}

TEST_CASE("Partial derivatives for a function of two variables")
{
    constexpr auto powers = PowersList<
        Powers<0, 0>, Powers<0, 1>, Powers<0, 2>, Powers<1, 0>, Powers<1, 1>, Powers<1, 2>, Powers<2, 2>>{};
    constexpr int coefficients[] = {1, 2, 1, 2, 1, 2, 1};
    constexpr auto poly = make_poly(coefficients, powers);
    constexpr auto d0poly = partial<0>(poly);

    using T = decltype(std::declval<int>() * std::declval<unsigned>());
    static_assert(std::is_same_v<
        std::remove_cv_t<decltype(d0poly)>,
        Polynomial<T, Powers<0, 0>, Powers<0, 1>, Powers<0, 2>, Powers<1, 2>>>);

    REQUIRE(d0poly.coeffs()[0] == 2);
    REQUIRE(d0poly.coeffs()[1] == 1);
    REQUIRE(d0poly.coeffs()[2] == 2);
    REQUIRE(d0poly.coeffs()[3] == 2);

    constexpr auto d1poly = partial<1>(poly);
    static_assert(std::is_same_v<
        std::remove_cv_t<decltype(d1poly)>,
        Polynomial<T, Powers<0, 0>, Powers<0, 1>, Powers<1, 0>, Powers<1, 1>, Powers<2, 1>>>);
    REQUIRE(d1poly.coeffs()[0] == 2);
    REQUIRE(d1poly.coeffs()[1] == 2);
    REQUIRE(d1poly.coeffs()[2] == 1);
    REQUIRE(d1poly.coeffs()[3] == 4);
    REQUIRE(d1poly.coeffs()[4] == 2);
}