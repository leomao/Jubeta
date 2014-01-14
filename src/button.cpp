#include <cstdio>

#include "button.h"
#include "object.h"
#include "utility.h"
#include "status.h"

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/graphics.h>

bool isBGHidden = false;

Button::Button(wxWindow* parent, int place,
               wxPoint pos, wxSize size,
               wxBitmap* marker, wxBitmap* jackets, wxBitmap* bg,
               MusicBar* musicbar, Song** now)
    : wxPanel(parent, ID_BUTTON1 + place,
              pos, size, wxBORDER_NONE, "Button")
{
    if (place >= 12)
        canChose_ = true;
    else
        canChose_ = false;


    startPosition_ = 0;
    nowPosition_   = 0;
    pointer_       = 0;
    frame_         = 80;

    isPaused_   = false;
    isPressed_  = false;
    isSelected_ = false;

    now_      = now;
    pos_      = pos;
    size_     = size;
    marker_   = marker;
    jackets_  = jackets;
    musicbar_ = musicbar;
    place_    = place;
    bg_       = bg;

    jacketPosition_ = wxPoint(size_.x * 0.075 + 1,
                              size_.y * 0.075 + 1);

    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &Button::onPaint, this);
}



void Button::setCanChose(bool canChose, Song* item)
{
    canChose_ = canChose;
    item_ = item;

    if (!canChose_) {
        isSelected_ = false;
    }

    return;
}



bool Button::getCanChose()
{
    return canChose_;
}



void Button::choose(bool isSelected)
{
    if (canChose_) {
        isSelected_ = isSelected;
        Refresh();
    }

    return;
}



void Button::start(int po, int st)
{
    startPosition_ = st;
    nowPosition_   = startPosition_;
    pointer_  = po;
    frame_    = 0;
    time_     = 0;
    isPaused_ = false;
    Refresh();
    //printf("%d ", place_);
    return;
}



void Button::toggle()
{
    if (isPaused_) {
        isPaused_ = false;
    }
    else if (frame_ < 80) {
        isPaused_ = true;
    }

    return;
}



void Button::jump(int position)
{
    
}



void Button::press(int pressPosition)
{
    isPressed_ = true;

    if (frame_ < 22) {
        int index;
        int judgeLevel;
        int interval = pressPosition - 504 - startPosition_;

        if (interval < 0)
            interval = -interval;

        if (interval <= 37) {
            frame_ = 22;
            judgeLevel = 4;
        }
        else if (interval <= 74) {
            frame_ = 37;
            judgeLevel = 3;
        }
        else if (interval <= 148) {
            frame_ = 52;
            judgeLevel = 2;
        }
        else {
            frame_ = 66;
            judgeLevel = 1;
        }

        index = (*now_)->judge(place_, pointer_, judgeLevel);
        musicbar_->Result(index, judgeLevel);
    }

    Refresh();
    return;
}



void Button::release()
{
    isPressed_ = false;
    Refresh();
    return;
}



void Button::stop()
{
    frame_ = 80;
    isPaused_ = false;
    Refresh();
}



int  Button::getFrame()
{
    return frame_;
}



void Button::runMarker(int nowPosition)
{
    if (frame_ != 80 && nowPosition - nowPosition_ >= 800 / 22) {
        frame_++;
        nowPosition_ = nowPosition;

        if (frame_ == 22) {
            frame_ = 80;
            int index = (*now_)->judge(place_, pointer_, 0);
            musicbar_->Result(index, 0);
        }
        else if (frame_ == 37 || frame_ == 52
                 || frame_ == 66 || frame_ == 79) {
            frame_ = 80;
        }

        Refresh();
    }

    return;
}



void Button::onPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC pdc(this);
    wxGCDC dc(pdc);
    dc.SetBackground(*wxTRANSPARENT_BRUSH);
    dc.Clear();
    
    dc.SetFont(dc.GetFont().Smaller());
    switch (status) {
        case S_WEL:
            if (!isBGHidden)
                render(dc, bg_[1], -pos_);
            break;
        case S_CH:
            if (!isBGHidden)
                render(dc, bg_[0], -pos_);

            if (canChose_) {
                wxColour col;

                if (isSelected_) {
                    col.Set(255, 255, 0, 150);
                    wxBrush brush(col, wxSOLID);
                    dc.SetBrush(brush);
                    dc.SetPen(wxPen(col, 3, wxSOLID));
                    dc.DrawRectangle(wxPoint(0, 0), size_);
                }

                //col.Set("#0099CC");
                //wxBrush brush(col, wxSOLID);
                //dc.SetBrush(brush);
                //dc.SetPen(wxPen(col, 3, wxSOLID));

                if (place_ < 12 && item_->isOk()) {
                    // dc.DrawText(item_->getTitle(), 2, 0);
                    render(dc, jackets_[place_], jacketPosition_);
                }
                else
                    render(dc, jackets_[place_], 0, 0);
            }

            break;
        case S_PLAY:
            if (!isBGHidden)
                render(dc, bg_[1], -pos_);

            if (frame_ < 80) {
                render(dc, marker_[frame_], 0, 0);
            }

            if (isPressed_) {
                wxColour col;
                col.Set("#FB81FF");
                wxBrush brush(wxColour(255, 255, 255), wxTRANSPARENT);
                dc.SetBrush(brush);
                dc.SetPen(wxPen(col, 3, wxSOLID));
                dc.DrawRectangle(wxPoint(0, 0), size_);
            }

            break;
        case S_OPT:
            if (!isBGHidden)
                render(dc, bg_[1], -pos_);
            break;
    }

    return;
}
