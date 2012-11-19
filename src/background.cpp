#include "background.h"
#include "utility.h"
#include "status.h"
#include "object.h"

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/dcbuffer.h>

using namespace std;

BG::BG(wxWindow* parent, wxPoint bgpos, wxSize bgsize)
    : wxPanel(parent, ID_BACKGROUND,
              bgpos, bgsize, wxBORDER_NONE, "BackGround")
{
    isHidden_ = true;
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &BG::onPaint, this);
}

void BG::setTheme(wxBitmap* bg)
{
    bg_ = bg;
    return;
}

void BG::showBG()
{
    isHidden_ = false;
}

void BG::hideBG()
{
    isHidden_ = true;
}

void BG::onPaint(wxPaintEvent& evt)
{
    wxBufferedPaintDC dc(this);
    dc.SetBackground(*wxTRANSPARENT_BRUSH);
    dc.Clear();

    if (!isHidden_) {
        if (status == S_PLAY)
            render(dc, bg_[1], 0, 0);
        else
            render(dc, bg_[0], 0, 0);
    }

    return;
}
