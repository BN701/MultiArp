//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017, 2018  Barry Neilsen
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
#include "maCursorKeys.h"

class CairoUI : public BaseUI
{
    public:
        CairoUI(bool dummy = false);
        ~CairoUI();

        xcb_connection_t * Connection() { return m_Connection; }

//        bool PollEvents(bool & gotKeyData, uint8_t & keycode, uint16_t & state);
        bool PollEvents(bool & gotKeyData, CursorKeys::key_type_t & curKey, xcb_keysym_t & sym);
        bool KeyInput(uint8_t keycode, uint16_t state, CursorKeys::key_type_t & curKey, xcb_keysym_t  & sym);

        virtual void Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute = BaseUI::attr_normal, double scale = 1.0);
        void SetTopLine(int midiChannel, double stepValue, double quantum, int runState, int midiMode);
        virtual void Progress(double progress, double stepWidth, double beat,
                int pattern_progress, double rtBeat, unsigned int queueSecs, unsigned int queueNano);

        xcb_keysym_t GetKeysym(int detail, int state);
        int GetFileDescriptor();

        void Refresh(int x, int y, int width, int height);
        void Refresh(Rectangle & r, bool useDouble = false);

    private:
        bool m_Dummy = false;
        int m_Width = 800;          // These could be FP and we calculate pixel values on the fly.
        int m_Height = 500;

        double m_Aspect = static_cast<double>(m_Width)/static_cast<double>(m_Height);
        double m_Scale = m_Height;  // Cairo drawing uses FP maths, and a convention I picked up from
                                    // one of the examples uses drawing area height = 1.0. We may end
                                    // up with a different scale value for each UI element.
        double m_GridSize = 0.04;

        cairo_font_face_t * m_FontFace = NULL;
        double m_FontHeight = 0.03;

        xcb_connection_t * m_Connection = NULL;
        xcb_screen_t * m_Screen = NULL;
        int m_ScreenNo = -1;
        xcb_drawable_t m_Window = {0};
        xcb_visualtype_t * m_Visual = NULL;

        xcb_gcontext_t m_gcPixmap = {0};
        xcb_pixmap_t m_Pixmap = {0};

        xcb_key_symbols_t * m_KeySymbols = NULL;

        // Cairo surface.

        cairo_surface_t * m_Surface = NULL;
};

#endif // MACAIROUI_H_INCLUDED
