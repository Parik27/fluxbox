// Tagging.cc for Fluxbox Window Manager
// Copyright (c) 2003 - 2006 Henrik Kinnunen (fluxgen at fluxbox dot org)
//                     and Simon Bowden    (rathnor at users.sourceforge.net)
// Copyright (c) 2002 Xavier Brouckaert
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "Tagging.hh"
#include "FbTime.hh"
#include "Screen.hh"
#include "Window.hh"
#include "WinClient.hh"
#include "fluxbox.hh"
#include "Layer.hh"
#include "Debug.hh"

#include "FbTk/I18n.hh"
#include "FbTk/FbString.hh"
#include "FbTk/StringUtil.hh"
#include "FbTk/FileUtil.hh"
#include "FbTk/MenuItem.hh"
#include "FbTk/App.hh"
#include "FbTk/stringstream.hh"
#include "FbTk/Transparent.hh"
#include "FbTk/AutoReloadHelper.hh"
#include "FbTk/RefCount.hh"
#include "FbTk/Util.hh"

#include <cstring>
#include <set>

using std::cerr;
using std::endl;
using std::string;
using std::list;
using std::set;
using std::make_pair;
using std::ifstream;
using std::ofstream;
using std::hex;
using std::dec;

/*------------------------------------------------------------------*\
\*------------------------------------------------------------------*/

Tagging *Tagging::s_instance = 0;

Tagging::Tagging() :
    m_tagged_workspace(-1),
    m_workspace_tag_end_time(0),
    m_tagged_window(0),
    m_window_tag_end_time(0) {
    setName("tagging");

    if (s_instance != 0)
        throw string("Can not create more than one instance of Tagging");

    s_instance = this;

    FbTk::RefCount<FbTk::Command<void> > emit_tagging(
            new FbTk::SimpleCommand<Tagging>(*this, &Tagging::emitTaggingSig));

    m_tag_window_timer.setCommand(emit_tagging);
    m_tag_workspace_timer.setCommand(emit_tagging);
    
    enableUpdate();
}

Tagging::~Tagging() {}

void Tagging::setupFrame(FluxboxWindow &win) {
    if (m_tagged_workspace != -1u && FbTk::FbTime::mono() < m_workspace_tag_end_time) {
        win.setWorkspace(m_tagged_workspace);

        // refresh the timeout
        tagWorkspace (m_tagged_workspace, m_workspace_tag_duration);
        
        // In case a non-existent window was tagged (or the tagged window was closed),
        // tag the next window to appear to allow you to have all the newly created windows
        // organised in tabs.

        if (FbTk::FbTime::mono() < m_window_tag_end_time)
            tagWindow(&win, m_window_tag_duration);
    }
}

void Tagging::setupClient(WinClient &winclient) {

    if (winclient.fbwindow () != 0 || !m_tagged_window || !isAnyWindowTagged())
        return;

    // Don't wanna tab transient windows (looking at you pekwm)
    if (winclient.isTransient())
        return; 

    // Attach if workspace are the same or if a workspace is tagged.
    if ((winclient.screen().currentWorkspaceID() == m_tagged_window->workspaceNumber() ||
         isAnyWorkspaceTagged())) {
        tagWindow (m_tagged_window, m_window_tag_duration);
        m_tagged_window->attachClient(winclient);
    }
}

void Tagging::updateClientClose(WinClient &winclient) {
    if (winclient.fbwindow() == m_tagged_window && winclient.fbwindow()->numClients() == 0) {
        m_tagged_window = nullptr;
        
        // If a workspace was tagged, keep the tabbing behaviour but pass it over to any
        // window created in the future.
        if (!isAnyWorkspaceTagged()) {
            m_window_tag_end_time = 0;
        }
    }
}

void Tagging::tagWorkspace(unsigned int ws, uint64_t duration) {
    m_tagged_workspace = ws;
    m_workspace_tag_end_time = duration == 0 ? 0 : FbTk::FbTime::mono() + (duration * 1000);
    m_workspace_tag_duration = duration;

    if (duration != 0)
        m_tag_workspace_timer.setTimeout(duration, true);
    else
        m_tag_workspace_timer.stop();

    m_tagging_sig.emit();
}

void Tagging::tagWindow(FluxboxWindow *window, uint64_t duration) {
    m_tagged_window = window;
    m_window_tag_end_time = duration == 0 ? 0 : FbTk::FbTime::mono() + (duration * 1000);
    m_window_tag_duration = duration;

    if (duration != 0)
        m_tag_window_timer.setTimeout(duration, true);
    else
        m_tag_window_timer.stop();
    
    m_tagging_sig.emit();
}

void Tagging::initForScreen(BScreen &screen) { }

bool Tagging::isAnyWindowTagged() {
    return (m_tagged_window != 0 || isAnyWorkspaceTagged()) &&
           FbTk::FbTime::mono() < m_window_tag_end_time;
}

bool Tagging::isAnyWorkspaceTagged() {
    return m_tagged_workspace != -1u && FbTk::FbTime::mono() < m_workspace_tag_end_time;
}
