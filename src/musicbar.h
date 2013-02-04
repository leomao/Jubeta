#ifndef MUSICBAR_H
#define MUSICBAR_H

#include "song.h"
#include "object.h"

#include <wx/wx.h>

class MusicBar : public wxPanel
{
    public:
        MusicBar(wxWindow*, wxPoint, wxSize);
        void Set(Song*);
        void Result(int, int);
        void Now(int);
        int GetNow();
        bool NowRefresh(int);
        int jump(int);
        void Clean();
        void show(wxDC&);

    private:
        wxColour none;
        wxColour gray;
        wxColour blue;
        wxColour yellow;
        wxColour border;
        wxColour position;

        wxSize size_;
        wxPoint pos_;
        int side_;
        int* bar_;
        int* judge_;
        int length_;
        int now_;

        void loadRecord();
        void OnPaint(wxPaintEvent& evt);
};

#endif
