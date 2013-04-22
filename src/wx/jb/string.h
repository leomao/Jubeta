///////////////////////////////////////////////////////////
// Filename:   wx/jb/string.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_STRING_H
#define JB_STRING_H

#include <jb/jb.h>

#include <wx/wx.h>
#include <wx/string.h>

using namespace std;

// definition of class jb::String
class jb::String : public wxString
{
public:
    String();
    String(const char* str);
    String(const string& str);
    String(const String& str);
    String operator= (const char* str);
    String operator= (const string& str);
    ~String();

private:

};

#endif // JB_STRING_H
