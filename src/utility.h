#ifndef UTILITY_H
#define UTILITY_H

#include <string>

#include <wx/wx.h>
#include <wx/utils.h>

using namespace std;

int convertToInt(wxString);
string convertToString(int);
void render(wxDC&, wxBitmap, int x, int y);
void render(wxDC&, wxBitmap, wxPoint);

#endif
