// WorkspaceCmd.cc for Fluxbox - an X11 Window manager
// Copyright (c) 2003 - 2006 Henrik Kinnunen (fluxgen at fluxbox dot org)
//                and Simon Bowden (rathnor at users.sourceforge.net)
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
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "TaggingCmd.hh"
#include "WorkspaceCmd.hh"

#include "Layer.hh"
#include "MinOverlapPlacement.hh"
#include "Workspace.hh"
#include "Window.hh"
#include "Screen.hh"
#include "Slit.hh"
#include "Toolbar.hh"
#include "fluxbox.hh"
#include "WinClient.hh"
#include "FocusControl.hh"
#include "WindowCmd.hh"
#include "Tagging.hh"

#include "FbTk/KeyUtil.hh"
#include "FbTk/CommandParser.hh"
#include "FbTk/stringstream.hh"
#include "FbTk/StringUtil.hh"

#ifdef HAVE_CMATH
#include <cmath>
#else
#include <math.h>
#endif
#include <algorithm>
#include <functional>
#include <vector>

using std::string;

FbTk::Command<void> *parseTaggingCmd(const string &command, const string &args,
                                     bool trusted) {

    // default duration
    int duration = 30;
    FbTk_istringstream iss(args.c_str());
    iss >> duration;

    if (command == "tagworkspace")
        return new TagWorkspaceCmd(duration, false);
    else if (command == "tagwindow")
        return new TagWindowCmd(duration, false);
    else if (command == "toggletagworkspace")
        return new TagWorkspaceCmd(duration, true);
    else if (command == "toggletagwindow")
        return new TagWindowCmd(duration, true);
    return 0;
}

REGISTER_COMMAND_PARSER(toggletagworkspace, parseTaggingCmd, void);
REGISTER_COMMAND_PARSER(toggletagwindow, parseTaggingCmd, void);
REGISTER_COMMAND_PARSER(tagworkspace, parseTaggingCmd, void);
REGISTER_COMMAND_PARSER(tagwindow, parseTaggingCmd, void);

void TagWorkspaceCmd::execute() {
    BScreen *screen = Fluxbox::instance()->mouseScreen();
    if (screen == 0)
        return;

    uint64_t duration = m_duration * 1000;
    if (m_toggle && Tagging::instance().isAnyWorkspaceTagged()) {
        duration = 0;
    }
    Tagging::instance().tagWorkspace(screen->currentWorkspaceID(), duration);
}

void TagWindowCmd::execute() {
    uint64_t duration = m_duration * 1000;
    if (m_toggle && Tagging::instance().isAnyWindowTagged()) {
        duration = 0;        
    }
    Tagging::instance().tagWindow(FocusControl::focusedFbWindow(), duration);
}
