#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED
#include <initializer_list>
#include <stdexcept>

namespace DDB
{
    inline namespace V1
    {
        class Vector
        {
        public:
            Vector(int sz);
            double& operator[](int i) const;
            [[nodiscard]] int size() const;
            static int validateSize(int);
        private:
            double* elem {};
            int sz {};
        };
    }

    namespace V2
    {
        class Vector
        {
        public:
            Vector(int s) : elem {new double[s]}, sz {s}
            {
                for (int i {}; i != s; ++i)
                {
                    elem[i] = 0;
                }
            }
            Vector(std::initializer_list<double> list);

            // plain *delete* removes an individual object from memory
            // *delete[]* removes an entire array
            ~Vector() { delete[] elem; } // release resources

            double& operator[] (int i) const;
            int size() const;
        private:
            double* elem;
            int sz;
        };
    }

    namespace V3
    {
        class Vector
        {
        public:
            explicit Vector(const int s) : elem {new double[s]}, sz {s} {}
            ~Vector() { delete[] elem; }
            Vector(const Vector& a);
            Vector& operator=(const Vector& a);
            Vector(Vector&& a) noexcept;
            Vector& operator=(Vector&& a) noexcept;
            double& operator[](const int i) { return elem[i]; }
            const double& operator[](const int i) const { return elem[i]; }
            [[nodiscard]] int size() const { return sz; }
        private:
            double* elem {};
            int sz {};
        };
    }

    namespace V4
    {
        template <typename T> // type parameterized class
        class Vector
        {
        public:
            explicit Vector(int s);
            Vector(const Vector& v);
            Vector(Vector&& v) noexcept;
            ~Vector() { delete[] elem; }
            Vector& operator=(const Vector& rhs);   
            Vector& operator=(Vector&& rhs) noexcept;
            T& operator[](int i) const;
            T* begin() const { return sz ? &elem[0] : nullptr; }
            T* end() const { return sz ? &elem[0] + sz : nullptr; }
            [[nodiscard]] constexpr int size() const { return sz; }
        private:
            T* elem {};
            int sz {};
        };
        template <typename T>
        Vector<T>::Vector(const int s)
        {
            if (s < 0)
                throw std::length_error {"V4::Vector negative length"};
            elem = new T[s];
            sz = s;
        }

        template <typename T>
        Vector<T>::Vector(const Vector& v)
            : elem {new T[v.sz]}, sz {v.sz}
        {
            for (int i {}; i != v.sz; ++i)
                elem[i] = v[i];
        }

        template <typename T>
        Vector<T>::Vector(Vector&& v) noexcept
            : elem {v.elem}, sz {v.sz}
        {
            v.sz = 0;
            v.elem = nullptr;
        }

        template <typename T>
        T& Vector<T>::operator[](const int i) const
        {
            if (i < 0 || sz <= i)
                throw std::out_of_range {"V4::Vector out of range index"};
            return elem[i];
        }

        template <typename T>
        // NOLINTNEXTLINE(bugprone-unhandled-self-assignment): false positive
        Vector<T>& Vector<T>::operator=(const Vector& rhs)
        {
            if (this != &rhs)
            {
                delete[] elem;
                elem = new T[rhs.sz];
                sz = rhs.sz;
                for (int i {}; i != rhs.sz; ++i)
                    elem[i] = rhs.elem[i];
            }

            return *this;
        }

        template <typename T>
        Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
        {
            elem = rhs.elem;
            sz = rhs.sz;
            rhs.elem = nullptr;
            rhs.sz = 0;
            return *this;
        }
    }
}
#endif
