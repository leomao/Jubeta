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

        void Reset();
        int GetPosition(int);
        bool GetNotes(int, int);
        int Judge(int, int, int);
        int Calculate();
        int GetLength();
        int GetNoteNumber();
        int* GetMusicBar();
        wxString GetMusic();
        wxString getTitle();
        wxString GetJacket();
        bool IsOk();

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
        int noteNumber_;
        int* musicbar_;

        int* judge_;

};

bool levelCompare(Song*, Song*);

#endif
