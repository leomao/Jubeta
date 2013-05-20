#ifndef UTILITY_H
#define UTILITY_H

#include <string>

#include <wx/wx.h>
#include <wx/utils.h>

using namespace std;

int convert_to_int(wxString);
string convert_to_string(int);
void render(wxDC&, wxBitmap, int x, int y);
void render(wxDC&, wxBitmap, wxPoint);

#endif
