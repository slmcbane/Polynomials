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
        std::is_same_v<std::remove_cv_t<decltype(polyprime)>,
            Polynomial<double, Powers<0>, Powers<1>, Powers<2>>>
    );

    REQUIRE(polyprime.coeffs()[0] == 2.0);
    REQUIRE(polyprime.coeffs()[1] == 6.0);
    REQUIRE(polyprime.coeffs()[2] == 12.0);
}