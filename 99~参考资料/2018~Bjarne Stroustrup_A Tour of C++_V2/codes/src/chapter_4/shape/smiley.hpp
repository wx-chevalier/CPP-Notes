#ifndef SMILEY_HPP_INCLUDED
#define SMILEY_HPP_INCLUDED
#include "circle.hpp"

#include <vector>

namespace DDB
{
    class Smiley : public Circle
    {
    public:
        Smiley(Point p, int rad) : Circle {p,rad}, mouth {nullptr} {}
        ~Smiley() override;
        void move(Point to) override;
        void draw() const override;
        void addEye(Shape* s) { eyes.push_back(s); }
        void setMouth(Shape* s);
        virtual void wink(int i);
    private:
        std::vector<Shape*> eyes;
        Shape* mouth;
    };
}
#endif
