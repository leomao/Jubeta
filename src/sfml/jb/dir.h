#ifndef JB_DIR_H
#define JB_DIR_H

#include <jb/jb.h>
#include <jb/string.h>

#include <wx/wx.h>
#include <wx/dir.h>

class jb::Dir
{
public:
    Dir();
    Dir(const String&);
    String getName();
    bool open(const String& dir);
    bool find(const String& filespec);
    bool first(String* filename);
    bool next(String* filename);
    void close();
    bool isOpened();

    static isDir(const String& dir);

private:
    wxDir dir_;
};

#endif // JB_DIR_H
