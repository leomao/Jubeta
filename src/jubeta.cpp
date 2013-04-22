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
    // buttons_  = new Button*[16];
    songs_    = new Song*[1000];
    item_position_ = 0;
    current_item_  = -1;
    current_place_ = 0;
    //
    sync_timer_ = new Timer();
    //
    //music player
    music_ = new Music(this);
    //
    // Load Config
    config_ = new jb::Config("config");

    if (!config->read("width", &width_, width_))
        config->write("width", width_);
    if (!config->read("height", &height_, height_))
        config->write("height", height_);

    setSize();

    for (int i = 0; i < 16; i++) {
        string tmp = "b";
        tmp += convertToString(i);

        if (!config->read(tmp, &key_[i], ke_y[i]))
            config->write(tmp, ke_y[i]);
    }

    wxSize  barsize;
    wxPoint barpos;
    wxSize  size;
    wxPoint pos;
    wxPoint tmp;

    if (width < height) {
        board = width / 21;
        outboard = (width - board * 19) / 2;
        side = 4 * board;
        size = wxSize(side, side);
        pos = wxPoint(0, height - width - 20);
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
        pos = wxPoint(width - buttonWidth, height - buttonWidth - 20);
    }

    inf = new INF(this, wxPoint(0, 0), wxSize(0, 0));
    bg = new BG(this, pos, wxSize(width, width));
    musicbar = new MusicBar(this, barpos, barsize);

    for (int i = 0; i < 16; i++) {
        tmp = wxPoint(outboard + (i % 4) * (side + board),
                      outboard + (i / 4) * (side + board));
        buttons[i] = new Button(bg, i, tmp, size, marker, jackets,
                                bgImage_, musicbar, &now_);
    }


    setMarker(markername);
    jb::String themename;

    // 設定Theme
    if (!config->read("theme", &themename, "default"))
        config->write("theme", "default");

    setTheme(themename);
    config->update(); // update config file
    loadSongs();

    welcome();

    return;
}

Jubeta::~Jubeta()
{
    // Destructor
    delete music_, sync_timer_, inf_, bg_, musicbar_, now_, config_;
    delete [] songs_;
}

void Jubeta::loadConfig()
{

}

void Jubeta::loadMarker()
{
    jb::String markername;

    // 設定Marker
    if (!config->read("marker", &markername, "default"))
        config->write("marker", "default");
}

void Jubeta::loadKey()
{

}

void Jubeta::loadTheme()
{

}


void Jubeta::loadSongs()
{
    jb::Dir songdir("songs");

    if (!songdir.isOpened()) {
        wxMessageBox("Fail to load Songs!!");
        this->close();
    }
    else {
        songCount = 0; // 計算有幾首曲目
        jb::String songname; //曲目名稱
        //一個一個 folder 檢查
        bool cont = songdir.getFirst(&songname, "" , jb::Dir::DIRS);

        while (cont) {
            //wxPuts (songname); //debug用
            songs[songCount] = new Song(songname);  // 加入曲目

            if (songs[songCount]->isOk())
                songCount++; // 如果這個曲目是可用的，將計數器加一

            cont = songdir.getNext(&songname);
        }
    }

    if (songCount > 12)
        gridCount = 3 * ((songCount - 1) / 3) + 3;
    else
        gridCount = 12;

    return;
}



void Jubeta::welcome()
{
    Refresh();
    convert();
    chooseSong();
}



void Jubeta::setUser()
{
    // 未加入此功能
}



void Jubeta::chooseSong()
{
    status = S_MENU;
    setItem(); // 設定選曲畫面的buttons
    select(currentPlace);  // 預設選擇上次的曲目
    return;
}



void Jubeta::setItem()
{
    if (itemPosition < 0)
        itemPosition = gridCount + itemPosition;

    itemPosition %= gridCount;
    item[0] = itemPosition;
    item[4] = item[0] + 1;
    item[8] = item[4] + 1;

    for (int i = 0; i <= 8; i += 4) {
        for (int j = 1; j <= 3; j++) {
            item[i + j] = item[i] + 3 * j;
            item[i + j] %= gridCount;
        }
    }

    for (int i = 0; i < 12; i++) {
        if (item[i] < songCount) {
            if (songs[item[i]]->getJacket() != "none") {
                wxImage ojacket;
                ojacket.LoadFile(songs[item[i]]->getJacket());
                jackets[i] = wxBitmap(ojacket.Scale(side * 0.85,
                                                    side * 0.85));
            }
            else {
                jackets[i] = noJacket;
            }

            buttons[i]->setCanChose(true, songs[item[i]]);
        }
        else {
            Song* noSong;
            buttons[i]->setCanChose(false, noSong);
        }
    }

    Refresh();
    return;
}



void Jubeta::shiftLeft()
{
    itemPosition -= 3;
    setItem();

    if (currentPlace == -1) {
        currentPlace = 4 * (currentItem % 3);

        if (item[currentPlace] != currentItem)
            currentPlace = -1;
        else
            buttons[currentPlace]->choose(true);
    }
    else {
        buttons[currentPlace]->choose(false);

        if (currentPlace != 3 && currentPlace != 7 && currentPlace != 11) {
            currentPlace++;
            buttons[currentPlace]->choose(true);
        }
        else {
            currentPlace = 4 * (currentPlace / 4);

            if (item[currentPlace] != currentItem)
                currentPlace = -1;
            else
                buttons[currentPlace]->choose(true);
        }
    }
}



void Jubeta::shiftRight()
{
    itemPosition += 3;
    setItem();

    if (currentPlace == -1) {
        currentPlace = 3 + 4 * (currentItem % 3);

        if (item[currentPlace] != currentItem)
            currentPlace = -1;
        else
            buttons[currentPlace]->choose(true);
    }
    else {
        buttons[currentPlace]->choose(false);

        if (currentPlace != 0 && currentPlace != 4 && currentPlace != 8) {
            currentPlace--;
            buttons[currentPlace]->choose(true);
        }
        else {
            currentPlace = 3 + 4 * (currentPlace / 4);

            if (item[currentPlace] != currentItem)
                currentPlace = -1;
            else
                buttons[currentPlace]->choose(true);
        }
    }
}



void Jubeta::select(int button_i)
{
    if (button_i >= 0 && button_i < 12) {
        if (buttons[button_i]->getCanChose()
                && item[button_i] != currentItem) {
            buttons[currentPlace]->choose(false);
            buttons[button_i]->choose(true);
            currentItem = item[button_i];
            currentPlace = button_i;
            now_ = songs[item[button_i]];

            maxIndex_ = now_->getMaxIndex();
            setMusicBar(now_);
            music->Load(now_);
        }
    }

    return;
}



void Jubeta::setMusicBar(Song* in)
{
    musicbar->Set(in);
    musicbar->Clean();
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
    musicbar->Clean();
    isstart = true;
    now_->reset();
    pointer = 0;
    position_ = 0;

    for (int i = 0; i < 16; i++) {
        buttons[i]->stop();
    }

    ispaused = false;

    music->Stop();
    music->Start();
    syncTimer->Start(1);

    return;
}



void Jubeta::jump(int pixelPosition)
{
    toggle(-1);
    position_ = musicbar->jump(pixelPosition);
    music->jump(position_);
    pointer =  now_->searchPointer(position_);
}



void Jubeta::sync(int position)
{
    position_ = position;

    if (position_ - musicbar->GetNow() > 20 ||
            musicbar->GetNow() - position_ > 20) {
        isstart = musicbar->NowRefresh(position_);
    }

    for (int i = 0; i < 16; i++) {
        buttons[i]->runMarker(position_);
    }

    wxString tmp;
    tmp.Printf("Jubeta - Score : %d Notes : %d",
               now_->calculate(), now_->getNoteNumber());
    SetTitle(tmp);

    if (!ispaused) {
        while (pointer < maxIndex_ &&
                now_->getPosition(pointer) <= position_) {
            //printf("%d : ", pointer);
            for (int i = 0; i < 16; i++) {
                if (now_->getNotes(i, pointer))
                    buttons[i]->start(pointer,
                                      now_->getPosition(pointer));
            }

            pointer++;
            //printf("\n");
        }
    }

    return;
}



void Jubeta::toggle(int s = 0)
{
    if (s < 0 && !ispaused) {
        ispaused = true;

        music->Pause();

        //syncTimer->Stop();
        for (int i = 0; i < 16; i++) {
            buttons[i]->toggle();
        }
    }
    else if (s > 0 && ispaused) {
        ispaused = false;

        music->Start();

        for (int i = 0; i < 16; i++) {
            buttons[i]->toggle();
        }

        //syncTimer->Start();
    }
    else {
        if (ispaused) {
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
    isstart = false;
    music->Stop();
    syncTimer->Stop();
    musicbar->Clean();
    status = S_MENU;
    chooseSong();
    return;
}



void Jubeta::finish()
{
    wxString out;
    out.Printf("Score : %d\n\
                Perfect : %d\n\
                Great : %d\n\
                Good : %d\n\
                Poor : %d\n\
                Miss : %d\n",
               now_->calculate(),
               now_->perfect,
               now_->great,
               now_->good,
               now_->poor,
               now_->miss);
    wxMessageBox(out, "Score");
    return;
}



void Jubeta::setOption()
{
    //loadSongs();
    //itemPosition = 0;
    //currentPlace = 0;
    //chooseSong();
    status = S_MENU;
    return;
}


void Jubeta::setMarker(jb::String markername)
{
    jb::Dir markerdir("marker/" + markername);

    if (!markerdir.isOpened()) {
        wxMessageBox("Fail to load Marker!!");
        // this->close();
    }
    else {
        wxString tmp;

        for (int i = 0; i < 15; i++) {
            if (markerdir.getFirst(&tmp,
                                   "*normal_" + convertToString(i)
                                   + ".png")) {
                marker[i].load(markerdir.getName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 7; i++) {
            if (markerdir.getFirst(&tmp,
                                   "*normalpassed_" + convertToString(i)
                                   + ".png")) {
                marker[15 + i].load(markerdir.getName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 15; i++) {
            if (markerdir.getFirst(&tmp,
                                   "*perfect_" + convertToString(i)
                                   + ".png")) {
                marker[22 + i].load(markerdir.getName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 15; i++) {
            if (markerdir.getFirst(&tmp,
                                   "*great_" + convertToString(i)
                                   + ".png")) {
                marker[37 + i].load(markerdir.getName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 14; i++) {
            if (markerdir.getFirst(&tmp,
                                   "*good_" + convertToString(i)
                                   + ".png")) {
                marker[52 + i].load(markerdir.getName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 13; i++) {
            if (markerdir.getFirst(&tmp,
                                   "*bad_" + convertToString(i)
                                   + ".png")) {
                marker[66 + i].load(markerdir.getName() + "/" + tmp);
            }
        }

        markerdir.getFirst(&tmp, "*prev.png");
        marker[79].load(markerdir.getName() + "/" + tmp, format);

        for (int i = 0; i < 79; i++) {
            marker[i] = marker[i].scale(side, side);
        }
    }

    return;
}



void Jubeta::setKey(int button, int keycode)
{
    return;
}



void Jubeta::setTheme(wxString theme)
{
    jb::Dir themedir("theme/" + theme);
    jb::Dir imagedir(themedir.getName() + "/images");
    jb::Dir sounddir(themedir.getName() + "/sounds");

    if (!imagedir.isOpened() || !sounddir.isOpened()) {
        wxMessageBox("Fail to Load Theme!!");
        this->close();
    }
    else {
        wxImage ojackets[16];
        wxImage onoJacket;
        wxString tmp;

        if (imagedir.getFirst(&tmp, "*LArrow.*"))
            ojackets[12].load(imagedir.getName() + "/" + tmp);

        if (imagedir.getFirst(&tmp, "*RArrow.*"))
            ojackets[13].load(imagedir.getName() + "/" + tmp);

        if (imagedir.getFirst(&tmp, "*Option.*"))
            ojackets[14].load(imagedir.getName() + "/" + tmp);

        if (imagedir.getFirst(&tmp, "*Start.*"))
            ojackets[15].load(imagedir.getName() + "/" + tmp);

        if (imagedir.getFirst(&tmp, "*NoJacket.*"))
            onoJacket.load(imagedir.getName() + "/" + tmp);

        noJacket = wxBitmap(onoJacket.scale(side * 0.85, side * 0.85));

        for (int i = 0; i < 12; i++) {
            jackets[i] = noJacket;
        }

        for (int i = 12; i < 16; i++) {
            jackets[i] = wxBitmap(ojackets[i].scale(side, side));
        }

        wxImage playBG;
        wxImage chooseBG;

        if (imagedir.getFirst(&tmp, "*playBG.*")) {
            playBG.load(imagedir.getName() + "/" + tmp);
            //playBG = playBG.Scale(width, width);
            bgImage_[1] = wxBitmap(playBG.scale(board * 21, board * 21));
        }

        if (imagedir.getFirst(&tmp, "*chooseBG.*")) {
            chooseBG.load(imagedir.getName() + "/" + tmp);
            //chooseBG = chooseBG.Scale(width, width);
            bgImage_[0] = wxBitmap(chooseBG.scale(board * 21, board * 21));
        }

        bg->setTheme(bgImage_);

        if (sounddir.getFirst(&tmp, "beat.wav")) {
            beatfile_ = sounddir.getName() + "/" + tmp;
            Button::beatFile = beatfile_;
            wxSound::Play(beatfile_);
        }
    }

    return;
}



void Jubeta::convert()
{
    Convert_sheet();
    return;
}

void Jubeta::pressKey(int keycode)
{
    switch (status) {
    case S_WELCOME:

        break;

    case S_MENU:

        if (keycode == WXK_ESCAPE) {
            //status = S_WELCOME;
        }
        else if (keycode == key[15]) {
            start();
        }
        else if (keycode == key[14]) {
            status = S_OPTION;
            setOption();
        }
        else if (keycode == key[13]) {
            shiftRight();
        }
        else if (keycode == key[12]) {
            shiftLeft();
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

        if (isstart && keycode == WXK_SPACE) {
            toggle();
        }
        else if (keycode == WXK_ESCAPE) {
            if (isstart)
                stop();
        }
        else if (keycode == 'e' || keycode == 'E') {
            //Finish();
        }
        else {
            for (int i = 0; i < 16; i++) {
                if (keycode == key[i]) {
                    if (status == S_PLAY)
                        buttons[i]->press(music->Time());

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

void Jubeta::releaseKey(int keycode)
{
    for (int i = 0; i < 16; i++) {
        if (keycode == key[i]) {
            buttons[i]->release();
            break;
        }
    }

    return;
}

void Jubeta::onLeftDown(int x, int y)
{
    if (status == S_PLAY && isstart) {
        jump(x);
    }
}

void Jubeta::onLeftUp(int x, int y)
{
    if (status == S_PLAY) {
    }
}

