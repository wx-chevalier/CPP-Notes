#include "smiley.hpp"

DDB::Smiley::~Smiley()
{
    delete mouth;
    for (auto p : eyes)
        delete p;
}

void DDB::Smiley::move(Point to)
{
    Circle::move(to);
    draw();
}

void DDB::Smiley::draw() const
{
    Circle::draw();
    for (auto p : eyes)
        p->draw();
    mouth->draw();
}

void DDB::Smiley::setMouth(Shape* s)
{
    mouth = s;
}

void DDB::Smiley::wink(int i)
{
    auto p = eyes.at(i);
    p->draw();
}
