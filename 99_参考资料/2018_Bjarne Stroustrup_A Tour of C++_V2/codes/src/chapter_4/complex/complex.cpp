#include "complex.hpp"

#include <cmath>

using namespace DDB;

Complex& Complex::operator*=(Complex z)
{
    re *= z.re;
    im *= z.im;
    return *this;
}

Complex& Complex::operator/=(Complex z)
{
    re /= z.re;
    im /= z.im;
    return *this;
}

Complex sqrt(Complex a)
{
    return {std::sqrt(a.imag()), std::sqrt(a.real())};
}
