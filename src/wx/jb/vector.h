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


#endif // JB_VECTOR_H
