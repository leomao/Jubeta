///////////////////////////////////////////////////////////
// Filename:   wx/jb/panel.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_PANEL_H
#define JB_PANEL_H

#include <jb/jb.h>
#include <jb/string.h>
#include <jb/image.h>



// definition of class jb::Panel
class jb::Panel
{
public:
    Panel();
    ~Panel();
    void setColor(const String&);
    void setColor(int r, int g, int b, int a);
    void drawText(const String&, int x, int y);
    void drawImage(const Image&, int x, int y);
    void drawRectangle(int width, int height, int x, int y);

private:

};

#endif // JB_PANEL_H
