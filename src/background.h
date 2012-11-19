#ifndef BG_H
#define BG_H

#include <wx/wx.h>
#include <wx/custombgwin.h>

class BG : public wxPanel
{
    public:
        BG(wxWindow*, wxPoint, wxSize);

        void setTheme(wxBitmap*);
        void showBG();
        void hideBG();

    private:
        wxBitmap* bg_;
        bool isHidden_;

        void onPaint(wxPaintEvent& evt);
};

#endif
