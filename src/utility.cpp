#include <string>
#include <algorithm>

#include "utility.h"

using namespace std;

int convertToInt(wxString str)
{
    long tmp;
    str.ToLong(&tmp);
    int out = tmp;
    return out;
}

string convertToString(int num)
{
    string out = "";
    bool sgn = false;

    if (num < 0)
        sgn = true;

    if (num == 0)
        return "0";

    while (num != 0) {
        char c = '0' + num % 10;
        num /= 10;
        out += c;
    }

    if (sgn)
        out += '-';

    reverse(out.begin(), out.end());
    return out;
}

void render(wxDC& dc, wxBitmap image, int x, int y)
{
    if (image.IsOk())
        dc.DrawBitmap(image, x, y, true);

    return;
}

void render(wxDC& dc, wxBitmap image, wxPoint pos)
{
    if (image.IsOk())
        dc.DrawBitmap(image, pos, true);
    return;
}
