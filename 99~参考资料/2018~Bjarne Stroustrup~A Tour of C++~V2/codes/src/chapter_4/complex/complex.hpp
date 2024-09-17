#ifndef COMPLEX_HPP_INCLUDED
#define COMPLEX_HPP_INCLUDED
namespace DDB
{
    // 4.2.1 "classical user-defined arithmetic type"
    //      The class definition itself only contains operations requiring
    //      access to the representation.
    class Complex
    {
        double re;
        double im;
    public:
        Complex(double r, double i)
            : re {r}, im {i} {}
        Complex(double r)
            : re {r}, im {} {}
        Complex()
            : re {}, im {} {}

        // const specifiers guarantee representation wont be modified
        //      This allows const objects to also invoke these functions
        //      generally only non-const objects would be able to.
        double real() const { return re; }
        void real(double d) { re = d; }
        double imag() const { return im; }
        void imag(double d) { im = d; }

        Complex& operator+=(Complex z)
        {
            re += z.re;
            im += z.im;
            return *this;
        }

        Complex& operator-=(Complex z)
        {
            re -= z.re;
            im -= z.im;
            return *this;
        }

        // 4.2.1 defined out-of-class
        Complex& operator*=(Complex);
        Complex& operator/=(Complex);
    };

    // Useful operations that don't require direct access to representation can
    // be defined as follows:
    inline Complex operator+(Complex a, Complex b) { return a += b; }
    inline Complex operator-(Complex a, Complex b) { return a -= b; }
    inline Complex operator-(Complex a) { return {-a.real(), -a.imag()}; } // unary minus
    inline Complex operator*(Complex a, Complex b) { return a *= b; }
    inline Complex operator/(Complex a, Complex b) { return a /= b; }

    inline bool operator==(Complex a, Complex b)
    {
        return a.real() == b.real() && a.imag() == b.imag();
    }
    inline bool operator!=(Complex a, Complex b)
    {
        return !(a == b);
    }
    Complex sqrt(Complex);
}
#endif
