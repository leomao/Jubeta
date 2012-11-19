#include <string>
#include <iostream>

#include "main.h"
#include "jubeta.h"
#include "utility.h"

#include <wx/wx.h>
#include <wx/sizer.h>

IMPLEMENT_APP(JuApp);

bool JuApp::OnInit()
{
    wxInitAllImageHandlers();

    Jubeta* Frame = new Jubeta();
    Frame->Show();

    Bind(wxEVT_KEY_DOWN, &Jubeta::onKey, Frame);
    Bind(wxEVT_KEY_UP, &Jubeta::releaseKey, Frame);
    Frame->SetFocus();
    //SetTopWindow (Frame);

    return true;
}
