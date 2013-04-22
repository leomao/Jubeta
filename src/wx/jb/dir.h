///////////////////////////////////////////////////////////
// Filename:   wx/jb/dir.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_DIR_H
#define JB_DIR_H

#include <string>

#include <jb/jb.h>
#include <jb/string.h>

#include <wx/wx.h>
#include <wx/dir.h>

// definition of class jb::Dir
class jb::Dir
{
public:
    enum Type {FILES, DIRS}

    Dir();
    Dir(const String& dir);
    Dir(const char* dir);
    Dir(const std::string& dir);
    String getName();
    bool open(const String& dir);
    bool open(const char* dir);
    bool open(const std::string& dir);
    bool getFirst(String* filename, const String& spec, Type type);
    bool getFirst(String* filename, const char* spec, Type type);
    bool getFirst(String* filename, const std::string& spec, Type type);
    bool getNext(String* filename);
    void close();
    bool isOpened();

    static isDir(const String& dir);
    static isDir(const char* dir);
    static isDir(const std::string& dir);

private:
    wxDir dir_;
};

#endif // JB_DIR_H
