///////////////////////////////////////////////////////////
// Filename:   wx/jb/vector.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_VECTOR_H
#define JB_VECTOR_H

#include <jb/jb.h>

// definition of class jb::Vector
class jb::Vector
{
public:
    Vector();
    Vector(int x, int y);
    ~Vector();
    int get_x();
    int get_y();

private:
    int x_;
    int y_;

};

jb::Vector()
{
    x_ = 0;
    y_ = 0;
}

jb::Vector(int x, int y)
{
    x_ = x;
    y_ = y;
}

int jb::Vector::get_x()
{
    return x_;
}

int jb::Vector::get_y()
{
    return y_;
}

#endif // JB_VECTOR_H
