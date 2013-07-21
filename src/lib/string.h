///////////////////////////////////////////////////////////
// Filename:   wx/jb/string.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_STRING_H
#define JB_STRING_H

#include <jb/jb.h>

// definition of class jb::String
class jb::String : public std::string
{
public:
    String();
    String(const char* str);
    String(const std::string& str);
    String(const String& str);
    String operator= (const char* str);
    String operator= (const std::string& str);
    ~String();
    string to_std_string();
    size_t length();

private:

};

#endif // JB_STRING_H
