///////////////////////////////////////////////////////////
// Filename:   jubeta.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JUBETA_H
#define JUBETA_H

#include "status.h"
#include "music.h"
#include "button.h"
#include "musicbar.h"
#include "information.h"
#include "background.h"

#include <jb/jb.h>
#include <jb/string.h>
#include <jb/image.h>
#include <jb/panel.h>

// definition of class Jubeta
class Jubeta : public jb::Panel
{
public:
    Jubeta();
    ~Jubeta();

    // event function
    void pressKey(int keycode);
    void releaseKey(int keycode);
    void onLeftDown(int x, int y);
    void onLeftUp(int x, int y);

private:
    jb::Config* config_;
    int height_;
    int width_;

    Timer* sync_timer_;
    BG* bg_;
    jb::Image bg_image_[2];

    INF* inf_;
    MusicBar* musicbar_;

    Button* buttons_[16];
    int key_[20];
    jb::Image marker_[80];
    int side_;
    int board_;
    int outboard_;
    int item_[16];
    int item_position_;
    int current_item_;
    int current_place_;
    jb::Image jackets_[16];
    jb::Image no_jacket_;
    jb::String beatfile_;

    Song** songs_;
    Song* now_;
    Music* music_;
    int song_count_;
    int grid_count_;
    int position_;
    int pointer_;
    bool is_paused_;
    bool is_start_;
    bool is_music_;
    int max_index_;

    void loadConfig();
    void loadMarker();
    void loadKey();
    void loadTheme();
    void loadSongs();

    void welcome();
    void setUser();

    void showMenu();
    void setItem();
    void shiftLeft();
    void shiftRight();
    void select(int);
    void setMusicBar(Song*);
    void start();

    void play();
    void sync();
    void jump(int position);
    void toggle();
    void stop();
    void finish();

    void showOptions();
    void showMarkerOptions();
    void showThemeOptions();
    void showKeyOptions();
    void setMarker(const jb::String&);
    void setTheme(const jb::String&);
    void setKey(int place, int keycode);
    void reloadSongs();
    void convert();

};

#endif
