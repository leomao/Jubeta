#ifndef JB_TEXTFILE_H
#define JB_TEXTFILE_H

#include <jb/jb.h>
#include <jb/string.h>

#include <wx/textfile.h>

class jb::TextFile
{
public:
    TextFile();
    TextFile(const String& filename);
    ~TextFile();
    bool open();
    bool open(const String& filename);
    bool create(const String& filename);
    bool isOpened();
    bool first();
    bool next();
    bool eof();
    void seek(size_t line);
    String& get(size_t line);
    const String& get(size_t line) const;
    bool add(const String& newline);
    void close();
    

private:
    wxTextFile textfile_;
};

#endif // JB_H
