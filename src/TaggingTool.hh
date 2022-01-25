// TaggingTool.hh
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

#ifndef TAGGINGTOOL_HH
#define TAGGINGTOOL_HH


#include "ToolbarItem.hh"

#include "FbTk/FbString.hh"
#include "FbTk/Resource.hh"
#include "FbTk/Signal.hh"
#include "FbTk/TextButton.hh"
#include "FbTk/CachedPixmap.hh"

class IconbarTheme;
class BScreen;

namespace FbTk {
template <class T> class ThemeProxy;
}

class TaggingTool: public ToolbarItem {
public:
    enum ToolType
    {
        TAGGING_WORKSPACE,
        TAGGING_WINDOW
    };

    TaggingTool(const FbTk::FbWindow &parent, BScreen &screen,
                ToolType type,
            FbTk::ThemeProxy<IconbarTheme> &enabled_theme,
            FbTk::ThemeProxy<IconbarTheme> &disabled_theme);
    ~TaggingTool();

    virtual void move(int x, int y) { m_button.move(x, y); }
    virtual void resize(unsigned int width, unsigned int height) { m_button.resize(width, height); }
    virtual void moveResize(int x, int y, unsigned int width, unsigned int height) {
        m_button.moveResize(x, y, width, height);
    }

    virtual void show() { m_button.show(); }
    virtual void hide() { m_button.hide(); }
    virtual unsigned int width() const { return m_button.width(); }
    virtual unsigned int preferredWidth() const { return width(); }
    virtual unsigned int height() const { return m_button.height(); }
    virtual unsigned int borderWidth() const { return m_button.borderWidth(); }

    virtual void renderTheme(int alpha);

    // insist implemented, even if blank
    virtual void parentMoved() { m_button.parentMoved(); }

    // just update theme items that affect the size
    virtual void updateSizing();

    void updateState ();
    void reconfigTheme();

    FbTk::ThemeProxy<IconbarTheme> &theme() {
        return m_enabled ? m_enabled_theme : m_disabled_theme;
    }
private:
    FbTk::TextButton m_button;
    FbTk::ThemeProxy<IconbarTheme> &m_enabled_theme;
    FbTk::ThemeProxy<IconbarTheme> &m_disabled_theme;
    ToolType m_type;
    BScreen &m_screen;

    FbTk::CachedPixmap m_pm;
    
    bool m_enabled = false;
};

#endif // CLOCKTOOL_HH
