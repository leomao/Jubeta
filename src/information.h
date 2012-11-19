#ifndef INF_H
#define INF_H

#include <wx/wx.h>

class INF : public wxPanel
{
    public:
        INF(wxWindow*, wxPoint, wxSize);

    private:

        void onPaint(wxPaintEvent& evt);
};

#endif
