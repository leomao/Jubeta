#ifndef JB_TIMER_H
#define JB_TIMER_H

#include <jb/jb.h>

#include <wx/wx.h>
#include <wx/timer.h>

class jb::Timer : public wxTimer
{
public:
    Timer();
    ~Timer();
    bool start();
    void stop();
    bool toggle();
    bool isRunning();
    virtual void tick();
    void Notify();

};

#endif // JB_TIMER_H
