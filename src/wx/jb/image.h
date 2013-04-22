///////////////////////////////////////////////////////////
// Filename:   wx/jb/image.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_IMAGE_H
#define IB_IMAGE_H

#include <string>

#include <jb/jb.h>
#include <jb/string.h>

// definition of class jb::Image
class jb::Image
{
public:
    Image();
    Image(const String& filename);
    Image(const char* filename);
    Image(const std::string filename);
    ~Image();
    bool load(const String& filename);
    bool load(const char* filename);
    bool load(const std::string filename);
    bool isOk();
    Image scale(int width, int height) const;

private:

};

#endif // JB_IMAGE_H
