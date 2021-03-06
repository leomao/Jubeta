#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "song.h"
#include "utility.h" 
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/textfile.h>

Song::Song()
{
    isOk_ = false;
}

Song::Song(wxString name)
{
    position_ = new int[10000]();
    positionInBar_ = new int[10000]();
    int* onotes = new int[160000]();
    jacket_ = "none";
    music_ = "none";
    maxIndex_ = 0;
    isOk_ = false;
    highScore_ = 0;
    wxString foldername = ("songs/" + name);
    wxString filename;
    wxDir* folder = new wxDir(foldername);

    if (!folder->IsOpened()) {
        wxMessageBox("Failed to load " + foldername);
        return;
    }
    else if (!folder->GetFirst(&filename, "*.txt", wxDIR_DEFAULT)) {
        wxMessageBox("Failed to load File in " + foldername);
        return;
    }

    isOk_ = true;

    wxTextFile song(foldername + "/" + filename);
    song.Open();
    bool notesstart = false;
    wxString tmp; // 讀檔字串
    int tempo = 60; // 預設tempo
    //tposition 為變速那拍的位置
    //tindex 變速後重算的 index
    //position 是換算前的拍點
    //delay 是譜面和音樂對拍的延遲

    int tposition(0), tindex(0);
    int position(0), index(0), delay(0);

    for (tmp = song.GetFirstLine();
            !song.Eof(); tmp = song.GetNextLine()) {
        if (tmp[0] == 't') {
            // 設定tempo
            wxString tt = tmp.AfterFirst('=');
            tempo = convertToInt(tt);
        }
        else if (tmp[0] == 'm') {
            wxString tt = tmp.AfterFirst('=');

            if (folder->HasFiles(tt))
                music_ = foldername + "/" + tt;
        }
        else if (tmp[0] == 'j') {
            wxString tt = tmp.AfterFirst('=');

            if (folder->HasFiles(tt))
                jacket_ = foldername + "/" + tt;
        }
        else if (tmp[0] == 'd' || tmp[0] == 'r') {
            wxString tt = tmp.AfterFirst('=');
            delay = convertToInt(tt);
        }
        else if (tmp == "#start#") {
            notesstart = true;
        }
        else if (tmp[0] == '#') {
            if (tmp[1] == 't') {
                title_ = tmp.AfterFirst('=');
            }
            else if (tmp[1] == 'a') {
                artist_ = tmp.AfterFirst('=');
            }
            else if (tmp[1] == 'l') {
                wxString tt = tmp.AfterFirst('=');
                lev_ = convertToInt(tt);
            }
            else if (tmp[1] == 'd') {
                wxString tt = tmp.AfterFirst('=');
                dif_ = convertToInt(tt);
            }
        }
        else if (notesstart) {
            wxString tt = tmp.BeforeFirst(' ');
            index = convertToInt(tt);
            position = 60 * (index - tindex) / tempo;
            position += tposition;
            tt = tmp.AfterFirst(' ');

            if (tt[0] == 't') {
                // 變速
                //計算此拍結束時的拍點
                tindex = index;
                //計算此拍的結束位置
                tposition = position;
                tt = tt.AfterFirst('=');
                tempo = convertToInt(tt);
            }
            else {
                int note = convertToInt(tt);
                position_[maxIndex_] = position + delay;
                onotes[maxIndex_] = note;
                maxIndex_++;
            }
        }
    }

    song.Close();
    length_ = position_[maxIndex_ - 1];
    int barLength = length_ + 3000;

    for (int i = 0; i < maxIndex_; i++) {
        positionInBar_[i] = position_[i] * 120 / barLength;
    }

    notes_ = new bool[16 * maxIndex_]();
    judge_ = new int[16 * maxIndex_]();
    musicbar_ = new int[120]();
    noteNumber_ = 0;
    int pointer = 0;

    for (int i = 0; i < 120; ++i) {
        musicbar_[i] = 0;

        while (pointer < maxIndex_ && positionInBar_[pointer] <= i) {
            for (int j = 0; j < 16; ++j) {
                if (onotes[pointer] & 1) {
                    notes_[16 * pointer + j] = true;
                    musicbar_[i]++;
                    noteNumber_++;
                }
                else {
                    notes_[16 * pointer + j] = false;
                }

                onotes[pointer] >>= 1;
            }

            pointer++;
        }
    }

    delete [] onotes;
    delete folder;

    reset();
}

Song::~Song()
{
    delete [] notes_, position_, musicbar_, judge_;
}

void Song::reset()
{
    perfect = 0;
    great = 0;
    good = 0;
    poor = 0;
    miss = 0;
    combo = 0;
    door = 0;
}

int Song::searchPointer(int position)
{
    int* low = std::lower_bound(position_, position_ + maxIndex_, position);
    return low - position_;
}

int Song::getPosition(int pointer)
{
    return position_[pointer];
}

bool Song::getNotes(int place, int pointer)
{
    return notes_[16 * pointer + place];
}

int Song::judge(int place, int pointer, int result)
{
    int out = positionInBar_[pointer];
    judge_[16 * pointer + place] = result;

    switch (result) {
        case 4:
            perfect++;
            door += 2;
            combo++;
            break;
        case 3:
            great++;
            door += 2;
            combo++;
            break;
        case 2:
            good++;
            door += 1;
            combo++;
            break;
        case 1:
            poor++;
            door -= 8;
            combo = 0;
            break;
        case 0:
            miss++;
            door -= 8;
            combo = 0;
            break;
    }
    if (door > noteNumber_)
        door = noteNumber_;
    else if(door < 0)
        door = 0;

    return out;
}

int Song::calculate()
{
    int score = 0;
    score += 900000 * perfect / noteNumber_;
    score += 630000 * great / noteNumber_;
    score += 360000 * good / noteNumber_;
    score += 90000 * poor / noteNumber_;
    return score;
}

int Song::calBonus()
{
     return 100000 * door / noteNumber_;
}

int Song::getLength()
{
    return length_;
}

int Song::getMaxIndex()
{
    return maxIndex_;
}

int* Song::getMusicBar()
{
    return musicbar_;
}

int Song::getNoteNumber()
{
    return noteNumber_;
}

wxString Song::getMusic()
{
    return music_;
}

wxString Song::getTitle()
{
    return title_;
}

wxString Song::getJacket()
{
    return jacket_;
}

bool Song::isOk()
{
    return isOk_;
}

bool Song::compareByLev(Song* s1, Song* s2)
{
    return s1->lev_ < s2->lev_;
}
