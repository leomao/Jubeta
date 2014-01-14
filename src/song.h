#ifndef SONG_H
#define SONG_H

#include <wx/wx.h>

class Song
{
public:
    Song();
    Song(wxString name);
    ~Song();

    int perfect;
    int great;
    int good;
    int poor;
    int miss;
    int combo;
    int door;

    void reset();
    int searchPointer(int);
    int getPosition(int);
    bool getNotes(int, int);
    int judge(int, int, int);
    int calculate();
    int calBonus();
    int getLength();
    int getMaxIndex();
    int getNoteNumber();
    int* getMusicBar();
    wxString getMusic();
    wxString getTitle();
    wxString getJacket();
    bool isOk();

    static bool compareByLev(Song*, Song*);

private:
    bool isOk_;
    bool* notes_;
    int* position_;
    int* positionInBar_;
    int lev_;
    int dif_;
    int highScore_;

    wxString title_;
    wxString artist_;
    wxString music_;
    wxString jacket_;

    int length_;
    int maxIndex_;
    int noteNumber_;
    int* musicbar_;

    int* judge_;

};


#endif
