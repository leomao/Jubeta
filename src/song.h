#ifndef SONG_H
#define SONG_H

#include <wx/wx.h>

class Song
{
public:
    Song();
    Song(jb::String name);
    ~Song();

    int perfect;
    int great;
    int good;
    int poor;
    int miss;

    void reset();
    int search_pointer(int);
    int get_position(int);
    bool get_notes(int, int);
    int judge(int, int, int);
    int calculate();
    int get_length();
    int get_max_index();
    int get_note_number();
    int* get_musicbar();
    jb::String get_music();
    jb::String get_title();
    jb::String get_jacket();
    bool is_ok();

private:
    bool is_ok_;
    bool* notes_;
    int* position_;
    int* position_in_bar_;
    int lev_;
    int dif_;
    int highscore_;

    wxString title_;
    wxString artist_;
    wxString music_;
    wxString jacket_;

    int length_;
    int max_index_;
    int note_number_;
    int* musicbar_;

    int* judge_;

};

bool level_compare(Song*, Song*);

#endif
