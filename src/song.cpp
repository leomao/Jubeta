#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "song.h"
#include "utility.h" 
#include <jb/jb.h>
#include <jb/dir.h>
#include <jb/textfile.h>

Song::Song()
{
    is_ok_ = false;
}

Song::Song(jb::String name)
{
    position_ = new int[10000];
    position_in_bar_ = new int[10000];
    int* onotes = new int[160000];
    jacket_ = "none";
    music_ = "none";
    max_index_ = 0;
    is_ok_ = false;
    highscore_ = 0;
    jb::String foldername = ("songs/" + name);
    jb::String filename;
    jb::Dir* folder = new wxDir(foldername);

    if (!folder->is_opened()) {
        wxMessageBox("Failed to load " + foldername);
        return;
    }
    else if (!folder->get_first(&filename, "*.txt", jb::Dir::FILES)) {
        wxMessageBox("Failed to load File in " + foldername);
        return;
    }

    is_ok_ = true;

    jb::TextFile song(foldername + "/" + filename);
    song.open();
    bool notesstart = false;
    int tempo = 60; // 預設tempo
    //tposition 為變速那拍的位置
    //tindex 變速後重算的 index
    //position 是換算前的拍點
    //delay 是譜面和音樂對拍的延遲

    int tposition(0), tindex(0);
    int position(0), index(0), delay(0);

    jb::String line, tmp; // 讀檔字串
    line = song.first();
    while (!song.is_eof()) {
        if (line[0] == 't') {
            // 設定tempo
            tempo = line.after_first('=').to_int();
        }
        else if (line[0] == 'm') {
            tmp = line.after_first('=');
            if (folder->find(tmp))
                music_ = foldername + "/" + tmp;
        }
        else if (line[0] == 'j') {
            tmp = line.after_first('=');
            if (folder->find(tmp))
                jacket_ = foldername + "/" + tmp;
        }
        else if (line[0] == 'd' || line[0] == 'r') {
            delay = line.after_first('=').to_int();
        }
        else if (line == "#start#") {
            break;
        }
        else if (line[0] == '#') {
            if (line[1] == 't') {
                title_ = line.after_first('=');
            }
            else if (line[1] == 'a') {
                artist_ = line.after_first('=');
            }
            else if (line[1] == 'l') {
                wxString tmp = line.after_first('=');
                lev_ = convertToInt(tmp);
            }
            else if (line[1] == 'd') {
                wxString tmp = line.after_first('=');
                dif_ = convertToInt(tmp);
            }
        }
        line = song.next();
    }
    while (!song.is_eof()) {
        index = line.before_first(' ').to_int();
        position = 60 * (index - tindex) / tempo;
        position += tposition;
        tmp = line.after_first(' ');

        if (tmp[0] == 't') {
            // 變速
            //計算此拍結束時的拍點
            tindex = index;
            //計算此拍的結束位置
            tposition = position;
            tempo = tmp.after_first('=').to_int();
        }
        else {
            int note = tmp.to_int();
            position_[max_index_] = position + delay;
            onotes[max_index_] = note;
            max_index_++;
        }
       
    }

    song.close();
    length_ = position_[max_index_ - 1];
    int barlength = length_ + 3000;

    for (int i = 0; i < max_index_; i++) {
        position_in_bar_[i] = position_[i] * 120 / barlength;
    }

    notes_ = new bool[16 * max_index_];
    judge_ = new int[16 * max_index_];
    //memset (notes_, 0, sizeof (bool) * 16 * (max_index_ + 1));
    musicbar_ = new int[120];
    note_number_ = 0;
    int pointer = 0;

    for (int i = 0; i < 120; ++i) {
        musicbar_[i] = 0;

        while (pointer < max_index_ && position_in_bar_[pointer] <= i) {
            for (int j = 0; j < 16; ++j) {
                if (onotes[pointer] & 1) {
                    notes_[16 * pointer + j] = true;
                    musicbar_[i]++;
                    note_number_++;
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
}

int Song::search_pointer(int position)
{
    int* low = std::lower_bound(position_, position_ + max_index_, position);
    return low - position_;
}

int Song::get_position(int pointer)
{
    return position_[pointer];
}

bool Song::get_notes(int place, int pointer)
{
    return notes_[16 * pointer + place];
}

int Song::judge(int place, int pointer, int result)
{
    int out = position_in_bar_[pointer];
    judge_[16 * pointer + place] = result;

    switch (result) {
        case 4:
            perfect++;
            break;
        case 3:
            great++;
            break;
        case 2:
            good++;
            break;
        case 1:
            poor++;
            break;
        case 0:
            miss++;
            break;
    }

    return out;
}

int Song::calculate()
{
    int score = 0;
    score += 900000 * perfect / note_number_;
    score += 630000 * great / note_number_;
    score += 360000 * good / note_number_;
    score += 90000 * poor / note_number_;
    return score;
}

int Song::get_length()
{
    return length_;
}

int Song::get_max_index()
{
    return max_index_;
}

int* Song::get_musicbar()
{
    return musicbar_;
}

int Song::get_note_number()
{
    return note_number_;
}

jb::String Song::get_music()
{
    return music_;
}

jb::String Song::get_title()
{
    return title_;
}

jb::String Song::get_jacket()
{
    return jacket_;
}

bool Song::is_ok()
{
    return is_ok_;
}
