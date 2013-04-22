///////////////////////////////////////////////////////////
// Filename:   wx/jb/config.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_CONFIG_H
#define JB_CONFIG_H

#include <jb/jb.h>
#include <jb/string.h>
#include <jb/textfile.h>

#include <wx/wx.h>
#include <wx/fileconf.h>

// definition of class jb::Config
class jb::Config
{
public:
    Config();
    ~Config();

    bool read(const String& key, int* value);
    bool read(const String& key, String* value);
    bool read(const String& key, int* value, int def);
    bool read(const String& key, String* value, const String def);
    bool read(const std::string& key, int* value);
    bool read(const std::string& key, String* value);
    bool read(const std::string& key, int* value, int def);
    bool read(const std::string& key, String* value, const String def);
    bool read(const char* key, int* value);
    bool read(const char* key, String* value);
    bool read(const char* key, int* value, int def);
    bool read(const char* key, String* value, const String def);
    bool write(const String& key, int value);
    bool write(const String& key, const String& value);
    bool write(const String& key, const char*);
    bool write(const std::string& key, int value);
    bool write(const std::string& key, int value);
    bool write(const std::string& key, const String& value);
    bool write(const char* key, const char*);
    bool write(const char* key, const String& value);
    bool write(const char* key, const char*);
    bool clear(const String& key);
    bool clear(const std::string& key);
    bool clear(const char* key);
    bool update();
    
private:
    wxFileConfig config_;

};

#endif // JB_CONFING_H
