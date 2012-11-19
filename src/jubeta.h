#ifndef JUBETA_H
#define JUBETA_H

#include "status.h"
#include "music.h"
#include "button.h"
#include "musicbar.h"
#include "information.h"
#include "background.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/string.h>
#include <wx/mediactrl.h>
#include <wx/stopwatch.h>
#include <wx/fileconf.h>

class Jubeta : public wxFrame
{
    public:
        Jubeta();
        virtual ~Jubeta();
        //Jubeta (int width, int height, wxString, wxString, wxString);
        //
        void onKey(wxKeyEvent& evt);
        void releaseKey(wxKeyEvent& evt);

    private:
        BG* bg;
        INF* inf;
        wxBitmap bgImage_[2];

        //wxMenuBar* menubar;
        //wxMenu* fileMenu;
        //wxMenu* optionMenu;
        //wxMenuItem* openfileItem;
        //wxMenuItem* convertItem;
        //wxMenuItem* quitItem;
        //wxMenuItem* optionItem;

        wxFileConfig* config;
        int height;
        int width;

        wxTimer* syncTimer;
        int position;
        int pointer;
        bool ispaused;
        bool isstart;

        Music* music;
        bool ismusic;
        MusicBar* musicbar;

        int key[20];
        wxBitmap marker[80];
        Button** buttons;
        int side;
        int board;
        int outboard;

        Song* now;
        int length;

        int songCount;
        int gridCount;
        Song** songs;

        int item[16];
        int itemPosition;
        int currentItem;
        int currentPlace;

        wxBitmap jackets[16];
        wxBitmap noJacket;

        wxString beatfile_;

        void setMarker(wxString);
        void setKey(int, int);
        void setTheme(wxString);
        void loadSongs();

        void welcome();
        void setUser();

        void chooseSong();
        void setItem();
        void shiftLeft();
        void shiftRight();
        void select(int);
        void setMusicBar(Song*);
        void start();

        void play();
        void sync(wxTimerEvent& evt);
        void toggle();
        void stop();
        void finish();

        void setOption();
        void convert();

        void onIdle(wxIdleEvent& evt);
        //void OnLoad (wxMediaEvent& evt);
        void onAbout(wxCommandEvent& event);
        void onQuit(wxCommandEvent& event);
};

#endif
