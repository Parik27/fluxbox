// TaggingCmd.hh for Fluxbox - an X11 Window manager
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

#ifndef TAGGINGCMD_HH
#define TAGGINGCMD_HH
#include "FbTk/Command.hh"
#include "FbTk/RefCount.hh"

#include "ClientPattern.hh"
#include "FocusControl.hh"

class TagWorkspaceCmd: public FbTk::Command<void> {
public:
    TagWorkspaceCmd(uint64_t duration, bool toggle) :
        m_duration(duration), m_toggle(toggle) { }

    void execute();
private:
    uint64_t m_duration;
    bool m_toggle;
};

class TagWindowCmd: public FbTk::Command<void> {
public:
    TagWindowCmd(uint64_t duration, bool toggle) :
        m_duration(duration), m_toggle(toggle) { }

    void execute();
private:
    uint64_t m_duration;
    bool m_toggle;
};

#endif // TAGGINGCMD_HH
