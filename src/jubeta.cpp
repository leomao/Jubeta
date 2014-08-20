#include <cstdio>
#include <iostream>
#include <algorithm>

#include "jubeta.h"
#include "object.h"
#include "utility.h"
#include "convert.h"
#include "version_no.h"

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/graphics.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <wx/mediactrl.h>
#include <wx/stopwatch.h>

// 預設寬高
const long defheight = 700;
const long defwidth = 600;
// 預設鍵位
const long defkey[16] = {52, 53, 54, 55, 82, 84, 89, 85,
                         70, 71, 72, 74, 86, 66, 78, 77
                        };

//Game Status
Status status = S_WEL;
 
Jubeta::Jubeta()
    : wxFrame(NULL, ID_JUBETA, "Jubeta", wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX),
              "Jubeta")
{
    // Initial Objects
    ismusic  = false;    //是否有音樂檔
    ispaused = false;   //是否暫停
    isstart  = false;    //是否開始
    //key      = new int[20];  //鍵位
    //marker   = new wxBitmap[80];  //marker圖檔
    //jackets  = new wxBitmap[16]; //曲目的Jacket
    //bgImage_ = new wxBitmap[2]; //背景
    buttons  = new Button*[16];  //按鍵
    songs    = new Song*[1000];    //曲目
    //item     = new int[16];     // 選曲的選項
    itemPosition = 0;      //啟始選單位置
    currentItem  = -1;        //目前曲目編號
    currentPlace = 0;        //目前曲目位置
    //
    // 用 Timer 當微小時間的呼叫器
    syncTimer = new wxTimer(this, ID_SYNCTIMER);
    //
    //音樂播放器
    music = new Music(this);
    //
    // Load Config
    config = new wxFileConfig("Jubeta", wxEmptyString, "config", "config",
                              wxCONFIG_USE_LOCAL_FILE |
                              wxCONFIG_USE_RELATIVE_PATH,
                              wxConvAuto());
    long configData;

    if (!config->Read("width", &configData, defwidth))
        config->Write("width", defwidth);

    width = configData;

    if (!config->Read("height", &configData, defheight))
        config->Write("height", defheight);

    height = configData;
    SetClientSize(width, height);

    //設定鍵位
    //之後要加入 gamepad 的 Key
    for (int i = 0; i < 16; i++) {
        string tmp = "b";
        tmp += convertToString(i);

        if (!config->Read(tmp, &configData, defkey[i]))
            config->Write(tmp, defkey[i]);

        key[i] = configData;
    }

    //
    wxSize  barsize;
    wxPoint barpos;
    wxSize  size;
    wxPoint pos;
    wxPoint tmp;

    //
    // 設定畫面配置
    // 以後看是不是改用 Sizer

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

    wxString markername;

    //設定Marker
    if (!config->Read("marker", &markername, "default"))
        config->Write("marker", "default");

    setMarker(markername);
    wxString theme;

    //設定Theme
    if (!config->Read("theme", &theme, "default"))
        config->Write("theme", "default");

    setTheme(theme);
    config->Flush(); //更新config file
    loadSongs();
    Center();

    Bind(wxEVT_TIMER, &Jubeta::onTimer, this, ID_SYNCTIMER);
    // 微小時間的觸發器和 onTimer()連結
    // musicbar->Bind(wxEVT_LEFT_DOWN, &Jubeta::onLeftDown, this);
    // musicbar->Bind(wxEVT_LEFT_UP, &Jubeta::onLeftUp, this);
    // SetBackgroundStyle(wxBG_STYLE_PAINT);
    // Bind(wxEVT_PAINT, &Jubeta::onPaint, this);

    welcome(); //一開始先從 welcome()開始進入正式程序
    // syncTimer->Start(16);
    
    return;
}

Jubeta::~Jubeta()
{
    // Destructor
    delete music, syncTimer, inf, bg, musicbar, now_, config;
    delete []  buttons, songs;
}

void Jubeta::setMarker(wxString markername)
{
    wxBitmapType format = wxBITMAP_TYPE_PNG;
    wxDir markerdir("marker/" + markername);

    if (!markerdir.IsOpened()) {
        wxMessageBox("Fail to load Marker!!");
        this->Close();
        return;
    }
    else {
        wxImage omarker[80];
        wxString tmp;

        for (int i = 0; i < 15; i++) {
            if (markerdir.GetFirst(&tmp,
                                   "*normal_" + convertToString(i)
                                   + ".png")) {
                omarker[i].LoadFile(markerdir.GetName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 7; i++) {
            if (markerdir.GetFirst(&tmp,
                                   "*normalpassed_" + convertToString(i)
                                   + ".png")) {
                omarker[15 + i].LoadFile(markerdir.GetName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 15; i++) {
            if (markerdir.GetFirst(&tmp,
                                   "*perfect_" + convertToString(i)
                                   + ".png")) {
                omarker[22 + i].LoadFile(markerdir.GetName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 15; i++) {
            if (markerdir.GetFirst(&tmp,
                                   "*great_" + convertToString(i)
                                   + ".png")) {
                omarker[37 + i].LoadFile(markerdir.GetName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 14; i++) {
            if (markerdir.GetFirst(&tmp,
                                   "*good_" + convertToString(i)
                                   + ".png")) {
                omarker[52 + i].LoadFile(markerdir.GetName() + "/" + tmp);
            }
        }

        for (int i = 0; i < 13; i++) {
            if (markerdir.GetFirst(&tmp,
                                   "*bad_" + convertToString(i)
                                   + ".png")) {
                omarker[66 + i].LoadFile(markerdir.GetName() + "/" + tmp);
            }
        }

        markerdir.GetFirst(&tmp, "*prev.png");
        marker[79].LoadFile(markerdir.GetName() + "/" + tmp, format);

        for (int i = 0; i < 79; i++) {
            marker[i] = wxBitmap(omarker[i].Scale(side, side));
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
    wxDir themedir("theme/" + theme);
    wxDir imagedir(themedir.GetName() + "/images");
    wxDir sounddir(themedir.GetName() + "/sounds");

    if (!imagedir.IsOpened() || !sounddir.IsOpened()) {
        wxMessageBox("Fail to Load Theme!!");
        this->Close();
        return;
    }
    else {
        wxImage ojackets[16];
        wxImage onoJacket;
        wxString tmp;

        if (imagedir.GetFirst(&tmp, "*LArrow.*"))
            ojackets[12].LoadFile(imagedir.GetName() + "/" + tmp);

        if (imagedir.GetFirst(&tmp, "*RArrow.*"))
            ojackets[13].LoadFile(imagedir.GetName() + "/" + tmp);

        if (imagedir.GetFirst(&tmp, "*Option.*"))
            ojackets[14].LoadFile(imagedir.GetName() + "/" + tmp);

        if (imagedir.GetFirst(&tmp, "*Start.*"))
            ojackets[15].LoadFile(imagedir.GetName() + "/" + tmp);

        if (imagedir.GetFirst(&tmp, "*NoJacket.*"))
            onoJacket.LoadFile(imagedir.GetName() + "/" + tmp);

        noJacket = wxBitmap(onoJacket.Scale(side * 0.85, side * 0.85));

        for (int i = 0; i < 12; i++) {
            jackets[i] = noJacket;
        }

        for (int i = 12; i < 16; i++) {
            jackets[i] = wxBitmap(ojackets[i].Scale(side, side));
        }

        wxImage playBG;
        wxImage chooseBG;

        if (imagedir.GetFirst(&tmp, "*playBG.*")) {
            playBG.LoadFile(imagedir.GetName() + "/" + tmp);
            //playBG = playBG.Scale(width, width);
            bgImage_[1] = wxBitmap(playBG.Scale(board * 21, board * 21));
        }

        if (imagedir.GetFirst(&tmp, "*chooseBG.*")) {
            chooseBG.LoadFile(imagedir.GetName() + "/" + tmp);
            //chooseBG = chooseBG.Scale(width, width);
            bgImage_[0] = wxBitmap(chooseBG.Scale(board * 21, board * 21));
        }

        bg->setTheme(bgImage_);

        if (sounddir.GetFirst(&tmp, "beat.wav")) {
            beatfile_ = sounddir.GetName() + "/" + tmp;
        }
    }

    return;
}

void Jubeta::loadSongs()
{
    wxDir songdir("songs");

    if (!songdir.IsOpened()) {
        wxMessageBox("Fail to load Songs!!");
        this->Close();
        return;
    }
    else {
        songCount = 0; // 計算有幾首曲目
        wxString songname; //曲目名稱
        //一個一個 folder 檢查
        bool cont = songdir.GetFirst(&songname, wxEmptyString, wxDIR_DIRS);

        while (cont) {
            //wxPuts (songname); //debug用
            songs[songCount] = new Song(songname);  // 加入曲目

            if (songs[songCount]->isOk())
                songCount++; // 如果這個曲目是可用的，將計數器加一

            cont = songdir.GetNext(&songname);
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
    status = S_CH;
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
    Refresh();

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

    isstart = musicbar->NowRefresh(position_);
    if (!isstart) {
        finish();
        stop();
        return;
    }

    for (int i = 0; i < 16; i++) {
        buttons[i]->runMarker(position_);
    }

    wxString tmp;
    tmp.Printf("Jubeta - Score : %d Notes : %d Combo : %d",
               now_->calculate(), now_->getNoteNumber(), now_->combo);
    SetTitle(tmp);

    if (!ispaused) {
        while (pointer < maxIndex_ &&
                now_->getPosition(pointer) <= position_) {
            for (int i = 0; i < 16; i++) {
                if (now_->getNotes(i, pointer)) 
                    buttons[i]->start(pointer,
                                        now_->getPosition(pointer));
            }

            pointer++;
        }
    }

    return;
}

void Jubeta::toggle(int s = 0)
{
    if (s < 0 && !ispaused) {
        ispaused = true;

        music->Pause();

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
    syncTimer->Stop();
    music->Stop();
    musicbar->Clean();
    chooseSong();
    return;
}

void Jubeta::finish()
{
    wxString out;
    out.Printf("Score : %d\nPerfect : %d\nGreat : %d\nGood : %d\nPoor : %d\nMiss : %d\n",
               now_->calculate() + now_->calBonus(),
               now_->perfect,
               now_->great,
               now_->good,
               now_->poor,
               now_->miss);
    wxMessageBox(out, "Result");
    return;
}

void Jubeta::setOption()
{
    //loadSongs();
    //itemPosition = 0;
    //currentPlace = 0;
    //chooseSong();
    status = S_CH;
    return;
}

void Jubeta::convert()
{
    Convert_sheet();
    return;
}

void Jubeta::onTimer(wxTimerEvent& evt)
{
    if (isstart) {
        if (position_ != music->Time()) {
            sync(music->Time());
        }
    }
    return;
}

void Jubeta::onLeftDown(wxMouseEvent& evt)
{
    if (status == S_PLAY && isstart) {
        jump(evt.GetX());
    }
}

void Jubeta::onLeftUp(wxMouseEvent& evt)
{
    if (status == S_PLAY) {
    }
}

void Jubeta::onKey(wxKeyEvent& evt)
{
    int keycode = evt.GetKeyCode();

    switch (status) {
        case S_WEL :
            break;

        case S_CH :
            if (keycode == WXK_ESCAPE) {
                //status = S_WEL;
            }
            else if (keycode == key[15]) {
                start();
            }
            else if (keycode == key[14]) {
                status = S_OPT;
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

        case S_OPT :
            break;
    }
    return;
}

void Jubeta::releaseKey(wxKeyEvent& evt)
{
    int keycode = evt.GetKeyCode();

    for (int i = 0; i < 16; i++) {
        if (keycode == key[i]) {
            buttons[i]->release();
            break;
        }
    }

    return;
}

void Jubeta::onAbout(wxCommandEvent& event)
{
    wxString tmp = "Jubeta Emulator" + VERSIONNO
                   + "\nmade by LeoMao and Kevin00036"
                   + "\npowered by wxWidgets 2.9 series.";
    wxMessageBox(tmp, "About Jubeta");
    return;
}

void Jubeta::onQuit(wxCommandEvent& event)
{
    this->Close();
    return;
}

