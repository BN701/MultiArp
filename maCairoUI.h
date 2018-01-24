//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017  Barry Neilsen
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef MACAIROUI_H_INCLUDED
#define MACAIROUI_H_INCLUDED

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <cairo-xcb.h>

#include "maBaseUI.h"

class CairoUI : public BaseUI
{
public:
    CairoUI();
    ~CairoUI();

    xcb_connection_t * Connection() { return m_Connection; }

    bool PollEvents(bool & gotKeyData, uint8_t & keycode, uint16_t & state);

    virtual void Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute = BaseUI::attr_normal);
    virtual void Progress(double progress, double stepWidth, double beat, int pattern_progress);

    xcb_keysym_t GetKeysym(int detail, int state);
    int GetFileDescriptor();

private:
    int m_Width = 800;
    int m_Height = 500;

    double m_FontHeight = 0.03;
    double m_RowHeight = 0.04;

    xcb_connection_t * m_Connection;
    xcb_screen_t * m_Screen;
    int m_ScreenNo;
    xcb_drawable_t m_Window;
    xcb_visualtype_t * m_Visual;

    xcb_key_symbols_t * m_KeySymbols;

    // Cairo surface.

    cairo_surface_t * m_Surface;
};

#endif // MACAIROUI_H_INCLUDED
