///////////////////////////////////////////////////////////
// Filename:   wx/main.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef MAIN_H
#define MAIN_H

#include <jb/jb.h>
#include <jb/panel.h>

#include "../jubeta.h"

#include <wx/wx.h>
#include <wx/sizer.h>

// MainFrame class definition
class MainFrame : public wxFrame
{
public:
    MainFrame();
    ~MainFrame();

    void pressKey(wxKeyEvent& evt);
    void releaseKey(wxKeyEvent& evt);

private:
    Jubeta* jubeta_;

    void onLeftDown(wxMouseEvent& evt);
    void onLeftUp(wxMouseEvent& evt);

};

#endif // MAIN_H
