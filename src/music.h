#ifndef MUSIC_H
#define MUSIC_H

#include <wx/wx.h>
#include <wx/mediactrl.h>
#include <wx/stopwatch.h>

#include "song.h"

class Music
{
    public:
        Music(wxWindow* p);
        void Load(Song* song);
        void Start();
        void Pause();
        void Stop();
        long Time();

    private:
        wxMediaCtrl* mediaCtrl_;
        wxStopWatch* stopWatch_;
        bool isMusic_;
        bool isStop_;
        long now_;

};

#endif
