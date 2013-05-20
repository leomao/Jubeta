///////////////////////////////////////////////////////////
// Filename:   wx/jb/timer.h
// Maintainer: LeoMao
///////////////////////////////////////////////////////////

#ifndef JB_TIMER_H
#define JB_TIMER_H

#include <jb/jb.h>

#include <wx/wx.h>
#include <wx/timer.h>

// definition of class jb::Timer
class jb::Timer : public wxTimer
{
public:
    Timer();
    ~Timer();
    bool start(int interval);
    void stop();
    bool toggle();
    bool is_running();
    virtual void tick();

    void Notify();

private:
    bool is_started_;
    bool is_paused_;
    int interval_;
    int last_tick_;

};



#endif // JB_TIMER_H
