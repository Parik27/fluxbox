// Remember.hh for Fluxbox Window Manager
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

#ifndef TAGGING_HH
#define TAGGING_HH

#include "AtomHandler.hh"
#include "ClientPattern.hh"

#include "FbTk/Signal.hh"
#include "FbTk/Timer.hh"

#include <cstdint>
#include <map>
#include <list>
#include <memory>

class FluxboxWindow;
class BScreen;
class WinClient;

/**
 * Tagging class to tag a workspace or a window to open all the new windows in for a certain amount of time.
 */
class Tagging : public AtomHandler {
public:

    Tagging();
    ~Tagging();

    // Functions relating to AtomHandler

    // Functions we actually use
    void setupFrame(FluxboxWindow &win);
    void setupClient(WinClient &winclient);
    void updateClientClose(WinClient &winclient);

    void initForScreen(BScreen &screen);

    // Functions we ignore (zero from AtomHandler)
    // Leaving here in case they might be useful later

    void updateFocusedWindow(BScreen &, Window) { }
    void updateClientList(BScreen &screen) {}
    void updateWorkspaceNames(BScreen &screen) {}
    void updateCurrentWorkspace(BScreen &screen) {}
    void updateWorkspaceCount(BScreen &screen) {}
    void updateWorkarea(BScreen &) { }

    void updateWorkspace(FluxboxWindow &win) {}
    void updateState(FluxboxWindow &win) {}
    void updateHints(FluxboxWindow &win) {}
    void updateLayer(FluxboxWindow &win) {}
    void updateFrameClose(FluxboxWindow &win) {}

    bool isAnyWindowTagged();
    bool isAnyWorkspaceTagged();

    bool checkClientMessage(const XClientMessageEvent &ce, 
        BScreen * screen, WinClient * const winclient) { return false; }
    // ignore this
    bool propertyNotify(WinClient &winclient, Atom the_property) { return false; }

    static Tagging &instance() { return *s_instance; }

    void tagWorkspace(unsigned int ws, uint64_t duration);
    void tagWindow(FluxboxWindow *window, uint64_t duration);

    FbTk::Signal<> &taggingChangeSig() { return m_tagging_sig; }

    unsigned int taggedWorkspace () { return m_tagged_workspace; }
    
private:

    void emitTaggingSig () { m_tagging_sig.emit (); }
    
    static Tagging *s_instance;

    unsigned int m_tagged_workspace;
    uint64_t m_workspace_tag_end_time;
    uint64_t m_workspace_tag_duration;

    FluxboxWindow* m_tagged_window;
    uint64_t m_window_tag_end_time;
    uint64_t m_window_tag_duration;

    FbTk::Timer m_tag_window_timer;
    FbTk::Timer m_tag_workspace_timer;

    FbTk::Signal<> m_tagging_sig;
};

#endif // TAGGING_HH
