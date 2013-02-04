#include <wx/mediactrl.h>
#include <wx/filefn.h>

#include "music.h"
#include "object.h"

Music::Music(wxWindow* parent)
{
    mediaCtrl_ = new wxMediaCtrl(parent,
                                 ID_MUSIC,
                                 wxEmptyString,
                                 wxDefaultPosition,
                                 wxDefaultSize,
                                 wxNO_BORDER,
                                 wxEmptyString);
    mediaCtrl_->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
    stopWatch_ = new wxStopWatch();
    isMusic_ = false;
    isStop_ = true;
    now_ = 0;
}

void Music::Load(Song* song)
{
    if (song->GetMusic() != "none") {
        wxString file = wxGetCwd() + "/" + song->GetMusic();
        isMusic_ = mediaCtrl_->Load(file);

        if (!isMusic_)
            wxMessageBox("Failed to Load " + song->GetMusic(), "Failed");
    }
    else
        isMusic_ = false;

    return;
}

void Music::Start()
{
    if (isMusic_)
        mediaCtrl_->Play();

    stopWatch_->Start(now_);
    isStop_ = false;
    return;
}

void Music::Pause()
{
    if (isMusic_)
        mediaCtrl_->Pause();

    stopWatch_->Pause();
    now_ = stopWatch_->Time();
    return;
}

void Music::jump(int position)
{
    Pause();
    if (isMusic_ && position <= mediaCtrl_->Length())
        mediaCtrl_->Seek(position);

    now_ = position;
    return;
}

void Music::Stop()
{
    if (isMusic_)
        mediaCtrl_->Stop();

    stopWatch_->Pause();
    now_ = 0;
    isStop_ = true;
    return;

}

long Music::Time()
{
    if (isStop_)
        return 0;
    else {
        //if (isMusic_ && mediaCtrl_->GetState() != wxMEDIASTATE_STOPPED)
        //return mediaCtrl_->Tell();
        //else {
        return stopWatch_->Time();
        //}
    }
}

