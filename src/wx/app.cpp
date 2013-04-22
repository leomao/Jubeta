///////////////////////////////////////////////////////////
// Filename:   wx/app.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#include "app.h"
#include "main.h"

#include <wx/wx.h>

IMPLEMENT_APP(JuApp);

bool JuApp::OnInit()
{
    wxInitAllImageHandlers();


    MainFrame* Frame = new MainFrame();
    Frame->Show();

    Bind(wxEVT_KEY_DOWN, &MainFrame::pressKey, Frame);
    Bind(wxEVT_KEY_UP, &MainFrame::releaseKey, Frame);
    Frame->SetFocus();
    //SetTopWindow (Frame);

    return true;
}
