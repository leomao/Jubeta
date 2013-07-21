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
#include "timer.h"

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
    void press_key(int keycode);
    void release_key(int keycode);
    void on_left_down(int x, int y);
    void on_left_up(int x, int y);
    void sync();

private:
    jb::Config* config_;
    int height_;
    int width_;

    Timer* sync_timer_;
    BackGround* background_;
    jb::Image bg_image_[2];

    Info* info_;
    MusicBar* musicbar_;

    Button* buttons_[16];
    int key_[16];
    jb::Image marker_[80];
    int side_;
    int board_;
    int outboard_;
    int item_[16];
    int item_offset_;
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
    bool is_started_;
    bool is_music_;
    int max_index_;

    void layout();
    void load_config();
    void load_marker();
    void load_key();
    void load_theme();
    void load_songs();

    void welcome();
    void set_user();

    void show_menu();
    void set_item();
    void shift_left();
    void shift_right();
    void select(int);
    void set_musicbar(Song*);
    void start();

    void play();
    void jump(int position);
    void toggle();
    void stop();
    void finish();

    void show_options();
    void show_marker_options();
    void show_theme_options();
    void show_key_options();
    void set_marker(const jb::String&);
    void set_theme(const jb::String&);
    void set_key(int place, int keycode);
    void reload_songs();
    void convert();

};

#endif
