// Ensure consistency with header file
#include "vector.hpp"

#include <algorithm>
#include <initializer_list>
#include <stdexcept>

using namespace DDB;

// V1::Vector
Vector::Vector(const int sz)
    : elem {new double[validateSize(sz)]}, sz {sz}
{}

double& Vector::operator[] (const int i) const
{
    // throw transfers control to a handler for exceptions of type out_of_range
    // in some function that directly or indirectly called this function. The
    // implementation will unwind the function call stack as need to get back
    // to the context of the caller.
    if (i < 0 || size() <= i)
        throw std::out_of_range {"Vector::operator[] out_of_range"};
    return elem[i];
}

int Vector::size() const { return sz; }

int Vector::validateSize(const int sz)
{
    if (sz < 0)
        throw std::length_error {"Vector::validateSize() negative size"};
    return sz;
}

// V2::Vector
// Use of static_cast to explicitly convert unsigned integer to int.
// Other casts include reinterpret_cast (treats an object as a sequence of
// bytes) and const_cast (casts away const).
V2::Vector::Vector(std::initializer_list<double> list)
    : elem {new double[list.size()]}, sz {static_cast<int>(list.size())}
{
    std::copy(list.begin(), list.end(), elem);
}

double& V2::Vector::operator[](int i) const
{
    return elem[i];
}

int V2::Vector::size() const
{
    return sz;
}

// V3::Vector

V3::Vector::Vector(const Vector& a)
    : elem {new double[a.sz]}, sz {a.sz}
{
    for (int i {}; i != sz; ++i)
        elem[i] = a.elem[i];
}

V3::Vector& V3::Vector::operator=(const Vector& a)
{
    if (this == &a)
        return *this;

    double* const p {new double[a.sz]};
    for (int i {}; i != a.sz; ++i)
        p[i] = a.elem[i];
    delete[] elem;
    elem = p;
    sz = a.sz;
    return *this;
}

V3::Vector::Vector(Vector&& a) noexcept
    : elem {a.elem}, sz {a.sz}
{
    a.elem = nullptr;
    a.sz = 0;
}

V3::Vector& V3::Vector::operator=(Vector&& a) noexcept
{
    elem = a.elem;
    sz = a.sz;
    a.elem = nullptr;
    a.sz = 0;
    return *this;
}
