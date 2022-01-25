// TaggingTool.cc
// Copyright (c) 2003 - 2006 Henrik Kinnunen (fluxgen at fluxbox dot org)
//                and Simon Bowden    (rathnor at users.sourceforge.net)
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

#include "TaggingTool.hh"
#include "CommandDialog.hh"
#include "ImageControl.hh"
#include "RefCount.hh"
#include "Tagging.hh"
#include "ToolbarItem.hh"
#include "Screen.hh"

#include "Workspace.hh"

#include "FbTk/MemFun.hh"
#include "FbTk/Signal.hh"
#include "FbTk/CommandParser.hh"

#include "IconbarTheme.hh"

TaggingTool::TaggingTool(const FbTk::FbWindow &parent, BScreen &screen, ToolType type,
        FbTk::ThemeProxy<IconbarTheme> &enabled_theme,
        FbTk::ThemeProxy<IconbarTheme> &disabled_theme) :
    ToolbarItem(Type::FIXED),
    m_button(parent, enabled_theme->text().font(), std::string("")), m_enabled_theme(enabled_theme),
    m_disabled_theme(disabled_theme), m_type(type), m_screen(screen), m_pm(screen.imageControl()) {
    m_button.setText(std::string(type == TAGGING_WORKSPACE ? "WS" : "WIN"));
    m_button.setTextPadding(10);

    FbTk::CommandParser<void> &cp = FbTk::CommandParser<void>::instance();

    FbTk::RefCount<FbTk::Command<void> > cmd(
            cp.parse(type == TAGGING_WORKSPACE ? "toggletagworkspace" : "toggletagwindow"));

    m_button.setOnClick(cmd);

    Tagging::instance().taggingChangeSig().connect(FbTk::MemFun(*this, &TaggingTool::updateState));
    updateState();
}

void TaggingTool::updateState() {
    switch (m_type) {

    case TAGGING_WORKSPACE: {
        m_enabled = Tagging::instance().isAnyWorkspaceTagged();
        break;
    }

    case TAGGING_WINDOW: {
        m_enabled = Tagging::instance().isAnyWindowTagged();
        break;
    }
    }

    reconfigTheme();
}

void TaggingTool::reconfigTheme () {
    m_button.setBorderColor(theme()->border().color());
    m_button.setBorderWidth(theme()->border().width());
    
    m_button.setGC(theme()->text().textGC());
    m_button.updateTheme(*theme());
    m_button.setBackgroundColor(theme()->texture().color());

    if (theme()->texture().usePixmap()) {
        m_pm.reset(m_screen.imageControl().renderImage(
                width(), height(), theme()->texture(), orientation()));
        m_button.setBackgroundPixmap(m_pm);
    } else {
        m_pm.reset(0);
        m_button.setBackgroundColor(theme()->texture().color());
    }

    m_button.clear();
}

void TaggingTool::renderTheme(int alpha) {
    m_button.setAlpha(alpha);
    reconfigTheme();
}

TaggingTool::~TaggingTool() { }

void TaggingTool::updateSizing() {    
    int bw = theme()->border().width();
    m_button.setBorderWidth(bw);

    unsigned int new_width = m_button.preferredWidth() + 2 * bw;
    unsigned int new_height = m_enabled_theme->text().font().height() + 2 * bw;

    new_width += 5;
    
    if (orientation() == FbTk::ROT0 || orientation() == FbTk::ROT180) {
        resize(new_width, new_height);
  } else {
    resize(new_height, new_width);
  }
}
