///////////////////////////////////////////////////////////
// Filename:   wx/jb/panel.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_PANEL_H
#define JB_PANEL_H

#include <jb/jb.h>
#include <jb/vector.h>
#include <jb/string.h>
#include <jb/image.h>


// definition of class jb::Panel
class jb::Panel
{
public:
    Panel();
    Panel(const Vector& pos, const Vector& size, Panel* parent);
    ~Panel();
    void set_color(const String&);
    void set_color(int r, int g, int b, int a);
    void draw_text(const String&, int x, int y);
    void draw_image(const Image&, int x, int y);
    void draw_rectangle(int width, int height, int x, int y);

private:

};

#endif // JB_PANEL_H
