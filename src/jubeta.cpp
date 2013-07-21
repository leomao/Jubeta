///////////////////////////////////////////////////////////
// Filename:   jubeta.cpp
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#include "jubeta.h"
#include "utility.h"
#include "convert.h"
#include "version.h"

#include <jb/jb.h>
#include <jb/string.h>
#include <jb/image.h>
#include <jb/panel.h>

jb::status = jb::S_WELCOME;

Jubeta::Jubeta()
    : jb::Panel()
{
    // Initial Objects
    is_music_   = false;
    is_paused_  = false;
    is_started_ = false;
    width_  = 500;
    height_ = 700;
    key_ = {52, 53, 54, 55, 82, 84, 89, 85,
            70, 71, 72, 74, 86, 66, 78, 77
           };
    songs_ = new Song*[1000];
    item_offset_ = 0;
    current_item_  = -1;
    current_place_ = 0;
    sync_timer_ = new Timer(&Jubeta::sync, this);
    music_ = new Music(this);

    config_ = new jb::Config("config");

    if (!config->read("width", &width_, width_))
        config->write("width", width_);
    if (!config->read("height", &height_, height_))
        config->write("height", height_);

    jb::Vector barsize;
    jb::Vector barpos;
    jb::Vector size;
    jb::Vector pos;
    jb::Vector tmp;

    if (width < height) {
        board = width / 21;
        outboard = (width - board * 19) / 2;
        side = 4 * board;
        size = wxSize(side, side);
        pos = wxPoint(0, height - width);
        int x = width / 120;
        x -= 1;
        x *= 120;
        int y = x / 10;
        barsize = wxSize(x, y);
        x = (width - x) / 2;
        y = height - width - y - outboard;
        barpos = wxPoint(x, y);
    }
    else {
        int buttonWidth = 10 * height / 11;
        board = buttonWidth / 21;
        outboard = (buttonWidth - board * 19) / 2;
        side = 4 * board;
        int x = buttonWidth / 120;
        x -= 1;
        x *= 120;
        int y = x / 10;
        barsize = wxSize(x, y);
        y = 0;
        x = width - buttonWidth + (buttonWidth - x) / 2;
        barpos = wxPoint(x, y);
        size = wxSize(side, side);
        pos = wxPoint(width - buttonWidth, height - buttonWidth);
    }

    inf = new Info(this, jb::Vector(0, 0), jb::Vector(0, 0));
    background_ = new BackGround(this, pos, wxSize(width, width));
    musicbar = new MusicBar(this, barpos, barsize);

    for (int i = 0; i < 16; i++) {
        tmp = jb::Vector(outboard + (i % 4) * (side + board),
                         outboard + (i / 4) * (side + board));
        buttons[i] = new Button(background_, i, tmp, size, marker, jackets,
                                background_Image_, musicbar, &now_);
    }


    config->update(); // update config file
    load_songs();

    welcome();

    return;
}

Jubeta::~Jubeta()
{
    // Destructor
    delete music_, sync_timer_, info_;
    delete background_, musicbar_, now_, config_;
    delete [] songs_;
}

void Jubeta::layout()
{
    if (width_ < height_) {
        
    }
    else {

    }
}

void Jubeta::load_marker()
{
    jb::String markername;
    // 設定Marker
    if (!config->read("marker", &markername, "default"))
        config->write("marker", "default");
    set_marker(markername);
}

void Jubeta::load_key()
{
    for (int i = 0; i < 16; i++) {
        string tmp = "b";
        tmp += convert_to_string(i);

        if (!config->read(tmp, &key_[i], key_[i]))
            config->write(tmp, key_[i]);
    }
}

void Jubeta::load_theme()
{
    jb::String themename;
    // 設定Theme
    if (!config->read("theme", &themename, "default"))
        config->write("theme", "default");
    set_theme(themename);
}


void Jubeta::load_songs()
{
    jb::Dir songdir("songs");

    if (!songdir.is_opened()) {
        wxMessageBox("Fail to load Songs!!");
        this->close();
    }
    else {
        song_count_ = 0; // 計算有幾首曲目
        jb::String songname; //曲目名稱
        //一個一個 folder 檢查
        bool cont = songdir.get_first(&songname, "" , jb::Dir::DIRS);

        while (cont) {
            songs[song_count_] = new Song(songname);  // 加入曲目

            if (songs[song_count_]->is_ok())
                song_count_++; // 如果這個曲目是可用的，將計數器加一

            cont = songdir.get_next(&songname);
        }
    }
    
    if (song_count_ > 12)
        grid_count_ = 3 * ((song_count_ - 1) / 3) + 3;
    else
        grid_count_ = 12;

    return;
}



void Jubeta::welcome()
{
    Refresh();
    convert();
    chooseSong();
}



void Jubeta::set_user()
{
    // 未加入此功能
}



void Jubeta::show_menu()
{
    status = S_MENU;
    set_item(); // 設定選曲畫面的buttons
    select(current_place_);  // 預設選擇上次的曲目
    return;
}



void Jubeta::set_item()
{
    if (item_offset_ < 0)
        item_offset_ = grid_count_ + item_offset_;

    item_offset_ %= grid_count_;
    item[0] = item_offset_;
    item[4] = item[0] + 1;
    item[8] = item[4] + 1;

    for (int i = 0; i <= 8; i += 4) {
        for (int j = 1; j <= 3; j++) {
            item[i + j] = item[i] + 3 * j;
            item[i + j] %= grid_count_;
        }
    }

    for (int i = 0; i < 12; i++) {
        if (item[i] < song_count_) {
            if (songs[item[i]]->get_jacket() != "none") {
                jackets[i].load(songs[item[i]]->get_jacket());
            }
            else {
                jackets[i] = noJacket;
            }

            buttons[i]->set_choosable(true, songs[item[i]]);
        }
        else {
            Song* noSong;
            buttons[i]->set_choosable(false, noSong);
        }
    }

    Refresh();
    return;
}



void Jubeta::shift_left()
{
    item_offset_ -= 3;
    set_item();

    if (current_place_ == -1) {
        current_place_ = 4 * (current_item_ % 3);

        if (item[current_place_] != current_item_)
            current_place_ = -1;
        else
            buttons[current_place_]->choose(true);
    }
    else {
        buttons[current_place_]->choose(false);

        if (current_place_ != 3 && current_place_ != 7 &&
                current_place_ != 11) {
            current_place_++;
            buttons[current_place_]->choose(true);
        }
        else {
            current_place_ = 4 * (current_place_ / 4);

            if (item[current_place_] != current_item_)
                current_place_ = -1;
            else
                buttons[current_place_]->choose(true);
        }
    }
}



void Jubeta::shift_right()
{
    item_offset_ += 3;
    set_item();

    if (current_place_ == -1) {
        current_place_ = 3 + 4 * (current_item_ % 3);

        if (item[current_place_] != current_item_)
            current_place_ = -1;
        else
            buttons[current_place_]->choose(true);
    }
    else {
        buttons[current_place_]->choose(false);

        if (current_place_ != 0 && current_place_ != 4 &&
                current_place_ != 8) {
            current_place_--;
            buttons[current_place_]->choose(true);
        }
        else {
            current_place_ = 3 + 4 * (current_place_ / 4);

            if (item[current_place_] != current_item_)
                current_place_ = -1;
            else
                buttons[current_place_]->choose(true);
        }
    }
}



void Jubeta::select(int button_i)
{
    if (button_i >= 0 && button_i < 12) {
        if (buttons[button_i]->get_choosable() &&
                item[button_i] != current_item_) {
            buttons[current_place_]->choose(false);
            buttons[button_i]->choose(true);
            current_item_ = item[button_i];
            current_place_ = button_i;
            now_ = songs[item[button_i]];

            max_index_ = now_->get_max_index();
            set_musicbar(now_);
            music->load(now_);
        }
    }

    return;
}



void Jubeta::set_musicbar(Song* in)
{
    musicbar->set(in);
    musicbar->clean();
    return;
}



void Jubeta::start()
{
    status = S_PLAY;
    play();
    return;
}



void Jubeta::play()
{
    musicbar->clean();
    is_started_ = true;
    now_->reset();
    pointer_ = 0;
    position_ = 0;

    for (int i = 0; i < 16; i++) {
        buttons[i]->stop();
    }

    is_paused_ = false;

    music_->stop();
    music_->start();
    sync_timer_->start();

    return;
}



void Jubeta::jump(int pixelPosition)
{
    toggle(-1);
    position_ = musicbar->jump(pixelPosition);
    music->jump(position_);
    pointer_ =  now_->search_pointer(position_);
}



void Jubeta::sync(int position)
{
    position_ = position;

    if (position_ - musicbar->get_now() > 20 ||
            musicbar->get_now() - position_ > 20) {
        is_started_ = musicbar->now_refresh(position_);
    }

    for (int i = 0; i < 16; i++) {
        buttons[i]->sync(position_);
    }

    // wxString tmp;
    // tmp.Printf("Jubeta - Score : %d Notes : %d",
               // now_->calculate(), now_->getNoteNumber());
    // SetTitle(tmp);

    if (!is_paused_) {
        while (pointer_ < max_index_ &&
                now_->get_position(pointer_) <= position_) {
            //printf("%d : ", pointer);
            for (int i = 0; i < 16; i++) {
                if (now_->get_notes(i, pointer_))
                    buttons[i]->start(pointer_,
                                      now_->get_position(pointer_));
            }

            pointer_++;
            //printf("\n");
        }
    }

    return;
}



void Jubeta::toggle(int s = 0)
{
    if (s < 0 && !is_paused_) {
        is_paused_ = true;

        music->pause();

        //syncTimer->Stop();
        for (int i = 0; i < 16; i++) {
            buttons[i]->toggle();
        }
    }
    else if (s > 0 && is_paused_) {
        is_paused_ = false;

        music->start();

        for (int i = 0; i < 16; i++) {
            buttons[i]->toggle();
        }

        //syncTimer->Start();
    }
    else {
        if (is_paused_) {
            toggle(1);
        }
        else {
            toggle(-1);
        }
    }

    return;
}



void Jubeta::stop()
{
    is_started_ = false;
    music->stop();
    syncTimer->stop();
    musicbar->clean();
    status = S_MENU;
    chooseSong();
    return;
}



void Jubeta::finish()
{
    // wxString out;
    // out.Printf("Score : %d\n\
                // Perfect : %d\n\
                // Great : %d\n\
                // Good : %d\n\
                // Poor : %d\n\
                // Miss : %d\n",
               // now_->calculate(),
               // now_->perfect,
               // now_->great,
               // now_->good,
               // now_->poor,
               // now_->miss);
    // wxMessageBox(out, "Score");
    return;
}



void Jubeta::show_options()
{
    //loadSongs();
    //item_offset_ = 0;
    //current_place_ = 0;
    //chooseSong();
    status = S_MENU;
    return;
}


void Jubeta::show_marker_options()
{

}

void Jubeta::show_theme_options()
{

}

void Jubeta::show_key_options()
{

}


void Jubeta::set_marker(jb::String markername)
{
    jb::Dir markerdir("marker/" + markername);

    if (!markerdir.is_opened()) {
        wxMessageBox("Fail to load Marker!!");
        // this->close();
    }
    else {
        wxString tmp;

        for (int i = 0; i < 15; i++) {
            if (markerdir.get_first(&tmp,
                                   "*normal_" + convert_to_string(i)
                                   + ".png")) {
                marker[i].load(markerdir.get_name() + "/" + tmp);
            }
        }

        for (int i = 0; i < 7; i++) {
            if (markerdir.get_first(&tmp,
                                   "*normalpassed_" + convert_to_string(i)
                                   + ".png")) {
                marker[15 + i].load(markerdir.get_name() + "/" + tmp);
            }
        }

        for (int i = 0; i < 15; i++) {
            if (markerdir.get_first(&tmp,
                                   "*perfect_" + convert_to_string(i)
                                   + ".png")) {
                marker[22 + i].load(markerdir.get_name() + "/" + tmp);
            }
        }

        for (int i = 0; i < 15; i++) {
            if (markerdir.get_first(&tmp,
                                   "*great_" + convert_to_string(i)
                                   + ".png")) {
                marker[37 + i].load(markerdir.get_name() + "/" + tmp);
            }
        }

        for (int i = 0; i < 14; i++) {
            if (markerdir.get_first(&tmp,
                                   "*good_" + convert_to_string(i)
                                   + ".png")) {
                marker[52 + i].load(markerdir.get_name() + "/" + tmp);
            }
        }

        for (int i = 0; i < 13; i++) {
            if (markerdir.get_first(&tmp,
                                   "*bad_" + convert_to_string(i)
                                   + ".png")) {
                marker[66 + i].load(markerdir.get_name() + "/" + tmp);
            }
        }

        markerdir.get_first(&tmp, "*prev.png");
        marker[79].load(markerdir.get_name() + "/" + tmp, format);

        for (int i = 0; i < 79; i++) {
            marker[i] = marker[i].scale(side, side);
        }
    }

    return;
}



void Jubeta::set_key(int button, int keycode)
{
    key_[button] = keycode;
}



void Jubeta::set_theme(jb::String theme)
{
    jb::Dir themedir("theme/" + theme);
    jb::Dir imagedir(themedir.get_name() + "/images");
    jb::Dir sounddir(themedir.get_name() + "/sounds");

    if (!imagedir.is_opened() || !sounddir.is_opened()) {
        wxMessageBox("Fail to Load Theme!!");
        this->close();
    }
    else {
        wxImage ojackets[16];
        wxString tmp;

        if (imagedir.get_first(&tmp, "*LArrow.*"))
            jackets[12].load(imagedir.get_name() + "/" + tmp);

        if (imagedir.get_first(&tmp, "*RArrow.*"))
            jackets[13].load(imagedir.get_name() + "/" + tmp);

        if (imagedir.get_first(&tmp, "*Option.*"))
            jackets[14].load(imagedir.get_name() + "/" + tmp);

        if (imagedir.get_first(&tmp, "*Start.*"))
            jackets[15].load(imagedir.get_name() + "/" + tmp);

        if (imagedir.get_first(&tmp, "*NoJacket.*"))
            no_jacket_.load(imagedir.get_name() + "/" + tmp);

        for (int i = 0; i < 12; i++) {
            jackets[i] = no_jacket_;
        }

        if (imagedir.get_first(&tmp, "*playbackground_.*")) {
            bg_image_[1].load(imagedir.get_name() + "/" + tmp);
        }

        if (imagedir.get_first(&tmp, "*choosebackground_.*")) {
            bg_image_[0].load(imagedir.get_name() + "/" + tmp);
        }

        background_->set_theme(bg_image_);

        if (sounddir.get_first(&tmp, "beat.wav")) {
            beatfile_ = sounddir.get_name() + "/" + tmp;
        }
    }

    return;
}

void Jubeta::reload_songs()
{
    
}



void Jubeta::convert()
{
    convert_sheet();
    return;
}

void Jubeta::press_key(int keycode)
{
    switch (status) {
    case S_WELCOME:

        break;

    case S_MENU:

        if (keycode == jb::K_ESCAPE) {
            //status = S_WELCOME;
        }
        else if (keycode == key[15]) {
            start();
        }
        else if (keycode == key[14]) {
            status = S_OPTION;
            set_option();
        }
        else if (keycode == key[13]) {
            shift_right();
        }
        else if (keycode == key[12]) {
            shift_left();
        }
        else {
            for (int i = 0; i < 12; i++) {
                if (keycode == key[i]) {
                    select(i);
                    break;
                }
            }
        }

        break;

    case S_PLAY :

        if (is_started_ && keycode == jb::K_ESCAPE) {
            toggle();
        }
        else if (keycode == jb::K_ESCAPE) {
            if (is_started_)
                stop();
        }
        else if (keycode == 'e' || keycode == 'E') {
            //Finish();
        }
        else {
            for (int i = 0; i < 16; i++) {
                if (keycode == key[i]) {
                    if (status == S_PLAY)
                        buttons[i]->press(music->time());

                    break;
                }
            }
        }

        break;

    case S_OPTION :

        break;
    }

    return;
}

void Jubeta::release_key(int keycode)
{
    for (int i = 0; i < 16; i++) {
        if (keycode == key[i]) {
            buttons[i]->release();
            break;
        }
    }

    return;
}

void Jubeta::on_left_down(int x, int y)
{
    if (status == S_PLAY && is_started_) {
        jump(x);
    }
}

void Jubeta::on_left_up(int x, int y)
{
    if (status == S_PLAY) {
    }
}

