#ifndef SHAPE_HPP_INCLUDED
#define SHAPE_HPP_INCLUDED
namespace DDB
{
    class Shape
    {
    public:
        struct Point 
        {
            int x {};
            int y {};
        };
        virtual Point center() const = 0;
        virtual void move(Point to) = 0;
        virtual void draw() const = 0;
        virtual void rotate(int angle) = 0;
        virtual ~Shape() {}
    };
}
#endif
