#ifndef BUTTON_H
#define BUTTON_H

#include "song.h"
#include "musicbar.h"

#include <wx/wx.h>
#include <wx/sound.h>
#include <wx/stopwatch.h>

class Button : public wxPanel
{
    public:
        Button(wxWindow*, int, wxPoint, wxSize,
               wxBitmap*, wxBitmap*, wxBitmap*, 
               MusicBar*, Song**);
        void setCanChose(bool, Song*);
        bool getCanChose();
        void choose(bool);
        void start(int, int);
        void toggle();
        void stop();
        void press(int);
        void release();
        int getFrame();
        void runMarker(int);

        static bool isBGhidden;
        //
    private:
        bool canChose_;
        bool isPaused_;
        bool isPressed_;
        bool isSelected_;
        //
        int pointer_;
        int startPosition_;
        int nowPosition_;
        //
        int place_;
        MusicBar* musicbar_;
        Song** now_;
        Song* item_;
        int frame_;
        int time_;
        wxBitmap* marker_;
        wxBitmap* jackets_;
        wxBitmap* bg_;
        wxSize size_;
        wxPoint jacketPosition_;
        wxPoint pos_;
        //
        void onPaint(wxPaintEvent& event);
};

#endif
