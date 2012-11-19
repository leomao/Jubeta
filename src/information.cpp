#include "information.h"
#include "status.h"
#include "object.h"

#include <wx/wx.h>
#include <wx/dcbuffer.h>

using namespace std;

INF::INF(wxWindow* parent, wxPoint infpos, wxSize infsize)
    : wxPanel(parent, ID_INFORMATION,
              infpos, infsize, wxBORDER_NONE, "Information")
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &INF::onPaint, this);
}

void INF::onPaint(wxPaintEvent& evt)
{
    wxBufferedPaintDC dc(this);
    dc.SetBackground(*wxTRANSPARENT_BRUSH);
    dc.Clear();
    return;
}
