///////////////////////////////////////////////////////////
// Filename:   wx/jb/config.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_CONFIG_H
#define JB_CONFIG_H

#include <jb/jb.h>
#include <jb/string.h>
#include <jb/textfile.h>

// definition of class jb::Config
class jb::Config
{
public:
    Config();
    ~Config();

    bool load(const String& filename);
    bool load(const std::string* filename);
    bool load(const const char* filename);
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
    bool write(const String& key, const char* value);
    bool write(const std::string& key, int value);
    bool write(const std::string& key, const String& value);
    bool write(const std::string& key, const char* value);
    bool write(const char* key, int value);
    bool write(const char* key, const String& value);
    bool write(const char* key, const char* value);
    bool clear(const String& key);
    bool clear(const std::string& key);
    bool clear(const char* key);
    bool update();
    
private:

};

#endif // JB_CONFING_H
