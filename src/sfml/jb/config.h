#ifndef JB_CONFIG_H
#define JB_CONFIG_H

#include <jb/jb.h>
#include <jb/string.h>
#include <jb/textfile.h>

#include <wx/wx.h>
#include <wx/fileconf.h>

class jb::Config
{
public:
    Config();
    Config();
    ~Config();

    bool readKey(const String& key, int* value);
    bool readKey(const String& key, String* value);
    bool readKey(const String& key, int* value, int def);
    bool readKey(const String& key, String* value, const String def);
    bool writeKey(const String& key, int value);
    bool writeKey(const String& key, const String& value);
    bool writeKey(const String& key, const char*);
    bool deleteKey(const String& key);
    bool flush();
    
private:
    wxFileConfig config_;

};

#endif // JB_CONFING_H
