#ifndef CIRCLE_HPP_INCLUDED
#define CIRCLE_HPP_INCLUDED
#include "shape.hpp"
namespace DDB
{
    class Circle : public Shape
    {
    public:
        Circle(Point p, int rad);
        Point center() const override { return p; }
        void move(Point to) override { p = to; }
        void draw() const override;
        void rotate(int) override {}
    private:
        Point p;
        int r;
    };
}
#endif
