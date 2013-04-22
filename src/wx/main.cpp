///////////////////////////////////////////////////////////
// Filename:   wx/main.cpp
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#include "main.h"

#include "../jubeta.h"

// MainFrame Constructor
MainFrame::MainFrame()
{
    jubeta_ = new Jubeta();
    Bind(wxEVT_LEFT_DOWN, &MainFrame::onLeftDown, this);
    Bind(wxEVT_LEFT_UP, &MainFrame::onLeftUp, this);
}

// MainFrame destructor
MainFrame::~MainFrame()
{
    delete jubeta_;
}

// pass the keycode to jubeta's pressKey function
void MainFrame::pressKey(wxKeyEvent& evt)
{
    int keycode = evt.GetKeyCode();
    jubeta_->pressKey(keycode);
}

// pass the keycode to jubeta's releasekey function
void mainframe::releasekey(wxkeyevent& evt)
{
    int keycode = evt.GetKeyCode();
    jubeta_->releaseKey(keycode);
}

// pass the position of the click to jubeta's onLeftDown function
void MainFrame::onLeftDown(wxMouseEvent& evt)
{
    jubeta_->onLeftDown(evt.GetX(), evt.GetY());
}

// pass the position of the click to jubeta's onLeftUp function
void MainFrame::onLeftUp(wxMouseEvent& evt)
{
    jubeta_->onLeftUp(evt.GetX(), evt.GetY());
}
