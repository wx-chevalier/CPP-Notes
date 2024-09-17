export module Vector;
export namespace Modules
{
    export class Vector
    {
    public:
        Vector(int size);
        double& operator[](int index) const;
        int size() const;
    private:
        double* elem;
        int sz;
    };

    // Members implicitly export with class
    Vector::Vector(int s) : elem {new double[s]}, sz {s} {}

    double& Vector::operator[](int i) const { return elem[i]; }

    int Vector::size() const { return sz; }

    export int size(const Vector& v) { return v.size(); }
}
