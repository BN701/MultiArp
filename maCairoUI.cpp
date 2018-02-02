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


#include "maCairoUI.h"

#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

#define XK_MISCELLANY
#define XK_XKB_KEYS
#define XK_LATIN1
#include <X11/keysymdef.h>  // XK_ Unicode key name defines

using namespace std;

xcb_visualtype_t *find_visual(xcb_connection_t *c, xcb_visualid_t visual)
{
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(c));

    for (; screen_iter.rem; xcb_screen_next(&screen_iter)) {
        xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
        for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
            xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
            for (; visual_iter.rem; xcb_visualtype_next(&visual_iter))
                if (visual == visual_iter.data->visual_id)
                    return visual_iter.data;
        }
    }

    return NULL;
}



bool HasMonoFont(cairo_t * cr)
{
    cairo_text_extents_t te1, te2;

    cairo_text_extents (cr, "....", &te1);
    cairo_text_extents (cr, "WWWW", &te2);

    return te2.x_advance - te1.x_advance < 0.000001;
}
vector<string> fontNames = { "Just Checking", "Noto Mono", "Inconsolata", "Dejavu Sans Mono", "Ubuntu Mono", "monospace", "" };

bool FindMonoFont(cairo_t * cr)
{
    for ( auto name = fontNames.begin(); name != fontNames.end(); name++ )
    {
        cairo_select_font_face (cr, name->c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        if ( HasMonoFont(cr) )
            return true;
    }
    return false;
}

CairoUI::CairoUI(bool dummy):
    m_Dummy(dummy)
{
    if ( m_Dummy )
        return;

    m_Connection = xcb_connect (NULL, &m_ScreenNo);

    // Get the first screen

    const xcb_setup_t *setup = xcb_get_setup (m_Connection);
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator (setup);
    m_Screen = screen_iter.data;

    /*
        If we actually want to iterate through the screens, we do something like:

        while ( screen_iter.rem > 0 )   // I guess 'rem' is remaining.
        {
            ... ;
            xcb_screen_next(&screen_iter));
        }
     */

    // Create the IDs

    m_gcPixmap = xcb_generate_id(m_Connection);
    m_Pixmap = xcb_generate_id(m_Connection);

    m_Window    = xcb_generate_id (m_Connection);

    // Create the window

    uint32_t     mask      =    XCB_CW_BACK_PIXEL |
                                XCB_CW_EVENT_MASK;
    uint32_t     values[2] = {  m_Screen->white_pixel,                  // Back pixel
                                // XCB_EVENT_MASK_NO_EVENT |
                                XCB_EVENT_MASK_KEY_PRESS |
                                // XCB_EVENT_MASK_KEY_RELEASE |
                                // XCB_EVENT_MASK_BUTTON_RELEASE |
                                // XCB_EVENT_MASK_BUTTON_PRESS |
                                // XCB_EVENT_MASK_ENTER_WINDOW |
                                // XCB_EVENT_MASK_LEAVE_WINDOW |
                                // XCB_EVENT_MASK_POINTER_MOTION |
                                // XCB_EVENT_MASK_POINTER_MOTION_HINT |
                                // XCB_EVENT_MASK_BUTTON_1_MOTION |
                                // XCB_EVENT_MASK_BUTTON_2_MOTION |
                                // XCB_EVENT_MASK_BUTTON_3_MOTION |
                                // XCB_EVENT_MASK_BUTTON_4_MOTION |
                                // XCB_EVENT_MASK_BUTTON_5_MOTION |
                                // XCB_EVENT_MASK_BUTTON_MOTION |
                                XCB_EVENT_MASK_KEYMAP_STATE |
                                XCB_EVENT_MASK_EXPOSURE
                                // XCB_EVENT_MASK_VISIBILITY_CHANGE |
                                // XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                                // XCB_EVENT_MASK_RESIZE_REDIRECT |
                                // XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                                // XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                // XCB_EVENT_MASK_FOCUS_CHANGE |
                                // XCB_EVENT_MASK_PROPERTY_CHANGE |
                                // XCB_EVENT_MASK_COLOR_MAP_CHANGE |
                                // XCB_EVENT_MASK_OWNER_GRAB_BUTTON
                             };

    xcb_create_window (m_Connection,
//                       0,                             /* depth               */
                       XCB_COPY_FROM_PARENT,                             /* depth               */
                       m_Window,
                       m_Screen->root,                  /* parent window       */
                       20, 40,                          /* x, y                */
                       m_Width, m_Height,     /* width, height       */
                       1,                            /* border_width        */
                       XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
//                       xcWindow.screen->root_visual,           /* visual              */
                       XCB_COPY_FROM_PARENT,           /* visual              */
                       mask, values );                /* masks */

    // Create the offscreen buffer that we draw everything to.
    xcb_create_pixmap(m_Connection, m_Screen->root_depth, m_Pixmap, m_Window, m_Width, m_Height);

    // Create a graphics context for it (and XCB one, not a Cairo one).

    uint32_t gcMask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t gcValues[2];
    gcValues[0] = m_Screen->white_pixel;
    gcValues[1] = m_Screen->white_pixel;
    xcb_create_gc(m_Connection, m_gcPixmap, m_Pixmap, gcMask, gcValues);

    // Fill the screen.

    xcb_rectangle_t rects[] = {{ 0, 0, (uint16_t) m_Width, (uint16_t) m_Height}};
    xcb_poly_fill_rectangle(m_Connection, m_Pixmap, m_gcPixmap, 1, rects);

    // Map the window on the screen
    xcb_map_window (m_Connection, m_Window);

    // Attach the Cairo surface to the Window.

    m_Visual = find_visual(m_Connection, m_Screen->root_visual);
    if ( m_Visual == NULL) {
        xcb_disconnect(m_Connection);
        return;
    }

    // Flush ...

    xcb_flush (m_Connection);

    m_Surface = cairo_xcb_surface_create(m_Connection, m_Pixmap, m_Visual, m_Width, m_Height);

    // Set up a general purpose monospace font for the UI, then we can set a base unit cell width and height.
    // Perhaps, in a ideal world, we'd create the window using dimensions calculated from the font. However,
    // we're only using Cairo 'toy fonts' we can't font extents until we have a context, which means we must
    // already have a surface, which needs a window or a pixmap (which needs a window, anyway). Perhaps we
    // can resize the window and pixmap after this and defer map_window() until the end.

    vector<string> fontNames = { "sans", "Noto Mono", "Inconsolata", "Dejavu Sans Mono", "Ubuntu Mono", "monospace", "" };

    cairo_t *cr = cairo_create(m_Surface);  // Although the context is "created with a ref count
                                            // of one", it seems it won't persist indefinitely.
                                            // Maybe I should try increasing the ref count so I can
                                            // store the context in the class, here.

    cairo_scale (cr, m_Scale, m_Scale);

    for ( auto name = fontNames.begin(); name != fontNames.end(); name++ )
    {
        m_FontFace = cairo_toy_font_face_create (name->c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_face (cr, m_FontFace);
        if ( true || HasMonoFont(cr) )
        {
            // Get cell dimensions, here.
//            cairo_set_font_size (cr, m_FontHeight);
//            cairo_font_extents_t fe;
//            cairo_font_extents (cr, &fe);
//            m_CellWidth = fe.max_x_advance;
            break;
        }
        cairo_font_face_destroy (m_FontFace);
        m_FontFace = NULL;
    }

    cairo_destroy(cr);

    m_KeySymbols = xcb_key_symbols_alloc(m_Connection);

}


CairoUI::~CairoUI()
{
    if ( m_Dummy )
    {
        return;
    }
    xcb_key_symbols_free(m_KeySymbols);
    xcb_free_pixmap(m_Connection, m_Pixmap);
    xcb_disconnect(m_Connection);
}

xcb_keysym_t CairoUI::GetKeysym(int detail, int state)
{
    return xcb_key_symbols_get_keysym(m_KeySymbols, detail, state);
}

int CairoUI::GetFileDescriptor()
{
    if ( m_Connection != NULL )
        return xcb_get_file_descriptor(m_Connection);
    else
        return 0; // This is FD of stdin.
}

void CairoUI::Refresh(int x, int y, int width, int height)
{
    xcb_copy_area(m_Connection, m_Pixmap, m_Window, m_gcPixmap, x, y, x, y, width, height);
    xcb_flush(m_Connection);
}

void CairoUI::Refresh(Rectangle & r, bool useDouble)
{
    if ( !useDouble && (r.m_iWidth != 0 || r.m_iHeight != 0) )
        Refresh(r.m_iX, r.m_iY, r.m_iWidth, r.m_iHeight);
    else
        Refresh(floor(r.m_dX), floor(r.m_dY), ceil(r.m_dWidth), ceil(r.m_dHeight));

};

// Use these two maps to search for X Unicode symbols and raw state/keycode combinations
// that are used purely for menu navigation.

unordered_map<int, CursorKeys::key_type_t> g_xcbSymbolToCursorKey =
{
    {XK_Insert, CursorKeys::ins},
    {XK_Delete, CursorKeys::del},
    {XK_Down, CursorKeys::down},
    {XK_Up, CursorKeys::up},
    {XK_Left, CursorKeys::left},
    {XK_Right, CursorKeys::right},
    {XK_Escape, CursorKeys::escape}
};

// Key values are constructed from (state << 8) + keycode
unordered_map<int, CursorKeys::key_type_t> g_xcbKeycodeToCursorKey =
{
    {0x0170, CursorKeys::shift_page_up},
    {0x0175, CursorKeys::shift_page_down},
    {0x0870, CursorKeys::alt_page_up},
    {0x0875, CursorKeys::alt_page_down},
    {0x0474, CursorKeys::ctrl_down},
    {0x046F, CursorKeys::ctrl_up},
    {0x0471, CursorKeys::ctrl_left},
    {0x0472, CursorKeys::ctrl_right},
    {0x0174, CursorKeys::shift_down},
    {0x016F, CursorKeys::shift_up},
    {0x0171, CursorKeys::shift_left},
    {0x0172, CursorKeys::shift_right},
    {0x0571, CursorKeys::ctrl_shift_left},
    {0x0572, CursorKeys::ctrl_shift_right},
    {0x056F, CursorKeys::ctrl_shift_up},
    {0x0574, CursorKeys::ctrl_shift_down},
    {0x0177, CursorKeys::shift_delete},
    {0x0477, CursorKeys::ctrl_delete}
};

// Utility for diagnostics.

string show_modifiers (uint32_t mask)
{
    const char *MODIFIERS[] = {
            "Shift", "Lock", "Ctrl", "Alt",
            "Mod2", "Mod3", "Mod4", "Mod5",
            "Button1", "Button2", "Button3", "Button4", "Button5"
    };

    string result;
    for (const char **modifier = MODIFIERS ; mask; mask >>= 1, ++modifier) {
        if (mask & 1) {
            if ( ! result.empty() )
                 result += '+';
             result += *modifier;
        }
    }
    return result;
}



bool CairoUI::KeyInput(uint8_t keycode, uint16_t state, CursorKeys::key_type_t & curKey, xcb_keysym_t  & sym)
{
    bool result = true;

    curKey = CursorKeys::no_key;

    sym = GetKeysym(keycode, state);

    if ( sym != 0 )
    {
        // Look for symbols that are used purely for CursorKey navigation.
        // (Ins, Del, Up, Down, Left, Right, Esc).

        try
        {
            curKey = g_xcbSymbolToCursorKey.at(sym);
        }
        catch(...)
        {
            // Nothing found, so just fall through to check specific symbols below.
        }
    }
    else
    {
        // Look for raw decodes that map to CursorKeys.

        try
        {
            // unsigned int lookup = (kr->state << 8) + kr->detail;
            curKey = g_xcbKeycodeToCursorKey.at( (state << 8) + keycode);
        }
        catch(...)
        {
            // There's nothing here for us, so show some diagnostics and return.
//            set_status(STAT_POS_2, "X key NULL symbol. Detail/State: %#x/%#x (%s)", keycode, state, show_modifiers(state).c_str());
            return true;
        }
    }

//    result = handle_key_input(curKey, sym);

//    default:
//
//        if ( sym > 31 && sym < 127 )
//        {
//            commandString += sym;
//            set_status(COMMAND_HOME, commandString.c_str());
//            place_cursor(COMMAND_HOME + commandString.size());
//        }
//        else if ( true )
//        {
//            // We haven't used the symbol, so what was it?
//
//            string symbol = "X symbol: ";
//            char buff[20];
//            sprintf(buff, "%#x - ", sym);
//            symbol += buff;
//
//            if ( static_cast<unsigned char>(sym) < 32 )
//            {
//                sprintf(buff, "%i", static_cast<unsigned char>(sym));
//                symbol += buff;
//            }
//            else
//                symbol += static_cast<unsigned char>(sym);
//
//            symbol += " Cat:";
//
//            if ( xcb_is_keypad_key(sym) )
//                symbol += " keypad";
//            if ( xcb_is_private_keypad_key(sym) )
//                symbol += " private keypad";
//            if ( xcb_is_cursor_key(sym) )
//                symbol += " cursor";
//            if ( xcb_is_pf_key(sym) )
//                symbol += " pf";
//            if ( xcb_is_function_key(sym) )
//                symbol += " function";
//            if ( xcb_is_misc_function_key(sym) )
//                symbol += " misc function";
//            if ( xcb_is_modifier_key(sym) )
//                symbol += " modifier";
//
//            set_status(STAT_POS_2, "%s - Detail/State: %#x/%#x (%s)", symbol.c_str(), keycode, state, show_modifiers(state).c_str());
//
//        }
//        break;
//    }

    return result;
}

unordered_map<int, const char *> event_names =
{
    {XCB_KEY_PRESS, "XCB_KEY_PRESS"},
    {XCB_KEY_RELEASE, "XCB_KEY_RELEASE"},
    {XCB_BUTTON_PRESS, "XCB_BUTTON_PRESS"},
    {XCB_BUTTON_RELEASE, "XCB_BUTTON_RELEASE"},
    {XCB_MOTION_NOTIFY, "XCB_MOTION_NOTIFY"},
    {XCB_ENTER_NOTIFY, "XCB_ENTER_NOTIFY"},
    {XCB_LEAVE_NOTIFY, "XCB_LEAVE_NOTIFY"},
    {XCB_FOCUS_IN, "XCB_FOCUS_IN"},
    {XCB_FOCUS_OUT, "XCB_FOCUS_OUT"},
    {XCB_KEYMAP_NOTIFY, "XCB_KEYMAP_NOTIFY"},
    {XCB_EXPOSE, "XCB_EXPOSE"},
    {XCB_GRAPHICS_EXPOSURE, "XCB_GRAPHICS_EXPOSURE"},
    {XCB_NO_EXPOSURE, "XCB_NO_EXPOSURE"},
    {XCB_VISIBILITY_NOTIFY, "XCB_VISIBILITY_NOTIFY"},
    {XCB_CREATE_NOTIFY, "XCB_CREATE_NOTIFY"},
    {XCB_DESTROY_NOTIFY, "XCB_DESTROY_NOTIFY"},
    {XCB_UNMAP_NOTIFY, "XCB_UNMAP_NOTIFY"},
    {XCB_MAP_NOTIFY, "XCB_MAP_NOTIFY"},
    {XCB_MAP_REQUEST, "XCB_MAP_REQUEST"},
    {XCB_REPARENT_NOTIFY, "XCB_REPARENT_NOTIFY"},
    {XCB_CONFIGURE_NOTIFY, "XCB_CONFIGURE_NOTIFY"},
    {XCB_CONFIGURE_REQUEST, "XCB_CONFIGURE_REQUEST"},
    {XCB_GRAVITY_NOTIFY, "XCB_GRAVITY_NOTIFY"},
    {XCB_RESIZE_REQUEST, "XCB_RESIZE_REQUEST"},
    {XCB_CIRCULATE_NOTIFY, "XCB_CIRCULATE_NOTIFY"},
    {XCB_CIRCULATE_REQUEST, "XCB_CIRCULATE_REQUEST"},
    {XCB_PROPERTY_NOTIFY, "XCB_PROPERTY_NOTIFY"},
    {XCB_SELECTION_CLEAR, "XCB_SELECTION_CLEAR"},
    {XCB_SELECTION_REQUEST, "XCB_SELECTION_REQUEST"},
    {XCB_SELECTION_NOTIFY, "XCB_SELECTION_NOTIFY"},
    {XCB_COLORMAP_NOTIFY, "XCB_COLORMAP_NOTIFY"},
    {XCB_CLIENT_MESSAGE, "XCB_CLIENT_MESSAGE"},
    {XCB_MAPPING_NOTIFY, "XCB_MAPPING_NOTIFY"},
    {XCB_GE_GENERIC, "XCB_GE_GENERIC"}
};

//bool CairoUI::PollEvents(bool & gotKeyData, uint8_t & keycode, uint16_t & state)
bool CairoUI::PollEvents(bool & gotKeyData, CursorKeys::key_type_t & curKey, xcb_keysym_t & sym)
{
    bool result = true;
    gotKeyData = false;

    xcb_generic_event_t *event;
    while ( (event = xcb_poll_for_event (m_Connection)) )
    {
        switch (event->response_type & ~0x80)
        {

            case XCB_EXPOSE:
            {
                xcb_expose_event_t *ex = (xcb_expose_event_t *) event;

                // Avoid extra redraws by checking if this is the last expose event in the sequence.

                if (ex->count != 0)
                    break;

                //  static int i = 1;
                //  snprintf(text, 80, "X expose [%i]: x, y = %i, %i (%i x %i) ", i++, ex->x, ex->y, ex->width, ex->height);
                //  Text(whole_screen, 22, 0, text);

                Refresh(ex->x, ex->y, ex->width, ex->height);

                break;
            }

            //  case XCB_BUTTON_PRESS: {
            //      xcb_button_press_event_t *bp = (xcb_button_press_event_t *)event;
            //      print_modifiers (bp->state);
            //
            //      switch (bp->detail) {
            //      case 4:
            //          printf ("Wheel Button up in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
            //                  bp->event, bp->event_x, bp->event_y );
            //          break;
            //      case 5:
            //          printf ("Wheel Button down in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
            //                  bp->event, bp->event_x, bp->event_y );
            //          break;
            //      default:
            //          printf ("Button %" PRIu8 " pressed in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
            //                  bp->detail, bp->event, bp->event_x, bp->event_y );
            //          break;
            //      }
            //      break;
            //  }

            //  case XCB_BUTTON_RELEASE: {
            //      xcb_button_release_event_t *br = (xcb_button_release_event_t *)event;
            //      print_modifiers(br->state);
            //
            //      printf ("Button %" PRIu8 " released in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
            //              br->detail, br->event, br->event_x, br->event_y );
            //      break;
            //  }

            //  case XCB_MOTION_NOTIFY: {
            //      xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
            //
            //      printf ("Mouse moved in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
            //              motion->event, motion->event_x, motion->event_y );
            //      break;
            //  }

            //  case XCB_ENTER_NOTIFY: {
            //      xcb_enter_notify_event_t *enter = (xcb_enter_notify_event_t *)event;
            //
            //      printf ("Mouse entered window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
            //              enter->event, enter->event_x, enter->event_y );
            //      break;
            //  }

            //  case XCB_LEAVE_NOTIFY: {
            //      xcb_leave_notify_event_t *leave = (xcb_leave_notify_event_t *)event;
            //
            //      printf ("Mouse left window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
            //              leave->event, leave->event_x, leave->event_y );
            //      break;
            //  }

            case XCB_KEY_PRESS:
            {
                xcb_key_release_event_t *kr = (xcb_key_release_event_t *) event;
                gotKeyData = true;
                KeyInput(kr->detail, kr->state, curKey, sym);
                break;
            }

            case XCB_KEY_RELEASE:
                break;

            case XCB_NO_EXPOSURE:
                // I can't find docs for this one, but we get a lot of them, seemingly
                // after a text update, even though aren't registered to receive them.
                // Trap them here so we can leave other event reporting turned on in
                // the default block.
                break;

            default:
            {
                char text[80];
                try
                {
                    snprintf(text, 80, "X event: %s (Raw: %#0x)", event_names.at(event->response_type & ~0x80), event->response_type);
                }
                catch( ... )
                {
                    snprintf(text, 80, "X event: %i (Raw: %#0x)", event->response_type & ~0x80, event->response_type);
                }
                Text(whole_screen, 20, 0, text);
                break;
            }
        }

        free (event);
    }

    return result;
}


void CairoUI::Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute, double scale)
{
    if ( m_Dummy )
        return;

    cairo_t *cr = cairo_create(m_Surface);

	double x, y /*, px, ux=1, uy=1, dashlength */;

	cairo_font_extents_t fe;
//	cairo_text_extents_t te;

	/* Example is in 26.0 x 1.0 coordinate space */
	cairo_scale (cr, 500, 500);
	cairo_set_font_size (cr, m_FontHeight);

//	FindMonoFont(cr);

    cairo_set_font_face(cr, m_FontFace);

	Rectangle r(0.0, row * m_GridSize, 26 * m_GridSize, m_GridSize);

	/* Fill (clear) background rectangle */
    cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
    cairo_rectangle (cr, r.m_dX, r.m_dY, r.m_dWidth, r.m_dHeight );
    cairo_fill (cr);

//	/* Drawing code goes here */
//	cairo_select_font_face (cr, "gobbledegook", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//    status = cairo_status(cr);
//
//    cairo_font_face_t * toyFace = cairo_toy_font_face_create ("Inconsolate", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//    cairo_font_face_t * fontFace = cairo_get_font_face (cr);
//    cairo_scaled_font_t * scaledFace = cairo_get_scaled_font (cr);
//
//    cairo_set_font_face (cr,toyFace);
//    cairo_font_type_t fontType = cairo_font_face_get_type (fontFace);
//    cairo_status_t fontStatus = cairo_font_face_status (fontFace);
//
//    const char * fontFamily = cairo_toy_font_face_get_family (fontFace); // Just returns the string we supplied via cairo_select_font_face();
//    const char * toyFamily = cairo_toy_font_face_get_family (toyFace); // Just returns the string we supplied via cairo_select_font_face();
//
//	cairo_font_extents (cr, &fe);

//	cairo_device_to_user_distance (cr, &ux, &uy);
//	if (ux > uy)
//		px = ux;
//	else
//		px = uy;
	cairo_font_extents (cr, &fe);
//	cairo_text_extents (cr, text, &te);
	x = 0.1; // - te.x_bearing - te.width / 2;
	y = (1 + row) * m_GridSize - fe.descent; // + fe.height / 2);

	/* extents: width & height */
//	cairo_set_source_rgba (cr, 1, 1, 0.9, 0);
//	cairo_set_line_width (cr, px);
//	dashlength = 3*px;
//	cairo_set_dash (cr, &dashlength, 1, 0);
//	cairo_rectangle (cr, x + te.x_bearing, y + te.y_bearing, te.width, te.height);
//	cairo_fill (cr);

	/* text */
	cairo_move_to (cr, x, y);
	cairo_set_source_rgb (cr, 0, 0.5, 1);
	cairo_show_text (cr, text);

    cairo_surface_flush(m_Surface);

    cairo_destroy(cr);
//    xcb_flush(m_Connection);
    r.ScaleD2I(500);
    Refresh(r);
}

void CairoUI::SetTopLine(int midiChannel, double stepValue, double quantum, int runState, int midiInputMode)
{
    if ( m_Dummy )
    {
        return;
    }

	Rectangle rUpdate(0.0, 0.0, m_GridSize * 27, 2 * m_GridSize);

    cairo_t *cr = cairo_create(m_Surface);

	cairo_scale (cr, m_Scale, m_Scale);
    cairo_set_font_face(cr, m_FontFace);
	cairo_set_font_size (cr, m_FontHeight);

    cairo_font_extents_t fe;
	cairo_font_extents (cr, &fe);


	// Fill (clear) background rectangle

	switch ( midiInputMode )
	{
        case 1: // Quick
        case 2: // Full
            cairo_set_source_rgb(cr, 0.75, 0.3, 0.2);
            break;
        case 3: // Real time
            cairo_set_source_rgb(cr, 0.75, 0, 0);
            break;
        default:
            cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
            break;
    }

    cairo_rectangle (cr, rUpdate.m_dX, rUpdate.m_dY, rUpdate.m_dWidth, rUpdate.m_dHeight );
    cairo_fill (cr);

    // Run state highlight

    if ( runState != 0 )
        cairo_set_source_rgb(cr, 0, 0.75, 0);
    else
        cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_rectangle (cr, 23 * m_GridSize, 0, 3 * m_GridSize, 2 * m_GridSize);
    cairo_fill (cr);

    cairo_text_extents_t te;
	cairo_set_source_rgb (cr, 1, 1, 1);

	double x, y;

	// Draw labels.

//	y = m_GridSize/2 + fe.ascent;
	y = m_GridSize + fe.ascent - fe.height/2;

	vector<const char*> labels = {"Midi", "Step", "Link Quantum"};
	vector<double> widths = {3, 5, 10, 5};

	double xBase = 0;
	auto w = widths.begin();
	for ( auto label = labels.begin(); label != labels.end(); label++, w++ )
	{
        cairo_text_extents (cr, *label, &te);
        xBase += *w;
        x = xBase * m_GridSize - te.x_advance;
        cairo_move_to (cr, x, y);
        cairo_show_text (cr, *label);
    }


	// Draw values ...

	cairo_set_font_size (cr, 1.5 * m_FontHeight);
	cairo_font_extents (cr, &fe);
	y = m_GridSize + fe.ascent - fe.height/2;

    char value[10];
    int i;

    for (i = 0, xBase = 0, w = widths.begin(); w != widths.end(); i++, w++ )
    {
        switch (i)
        {
            case 0:
                snprintf(value, 10, "%02i", midiChannel);
                break;
            case 1:
                snprintf(value, 10, "%5.2f", stepValue);
                break;
            case 2:
                snprintf(value, 10, "%5.2f", quantum);
                break;
            case 3:
                snprintf(value, 10, "%s", "Run");
                // cairo_set_font_size (cr, 1.5 * m_FontHeight);
                // y -= m_FontHeight/4;
                if ( runState == 0 )
                    cairo_set_source_rgb (cr, 0.5, 0.5, 0.5);
                break;
        }
        xBase += *w;
        x = xBase * m_GridSize + m_GridSize/3;
        cairo_move_to (cr, x, y);
        cairo_show_text (cr, value);
    }

    cairo_surface_flush(m_Surface);

    cairo_destroy(cr);
    rUpdate.ScaleD2I(m_Scale);
    Refresh(rUpdate);

}

void CairoUI::Progress(double progress, double stepWidth, double beat, int pattern_progress,
                            double rtBeat, unsigned int queueSecs, unsigned int queueNano)
{
    if ( m_Dummy )
    {
        return;
    }

	double x, y;
    char text[80];
	cairo_font_extents_t fe;
    cairo_t *cr = cairo_create(m_Surface);

    //
    // Scratch area.
#if 0

    cairo_save(cr);

    cairo_translate(cr, 150, 300);
    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_rectangle(cr, -40, -25, 80, 50);

    cairo_rotate(cr, M_PI/2);

    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_rectangle(cr, -40, -25, 80, 50);
    cairo_stroke(cr);

    cairo_restore(cr);
#endif

	cairo_scale (cr, m_Scale, m_Scale);
    cairo_set_font_face(cr, m_FontFace);

    //
    // Progress.

	Rectangle rUpdate(26 * m_GridSize, 2 * m_GridSize, 13 * m_GridSize, 8 * m_GridSize);
    rUpdate.ScaleD2I(m_Scale);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle (cr, rUpdate.m_dX, rUpdate.m_dY, rUpdate.m_dWidth, rUpdate.m_dHeight );
    cairo_fill(cr);

    double ux=1, uy=1;
    cairo_device_to_user_distance (cr, &ux, &uy);
    if (ux < uy)
        ux = uy;
    cairo_set_source_rgb(cr, 0, 0.25, 0.5);
    cairo_set_line_width (cr, 2*ux);

    cairo_save (cr);

    cairo_translate(cr, rUpdate.m_dX + rUpdate.m_dWidth/2, rUpdate.m_dY + rUpdate.m_dHeight/2);

    // Draw circle and first tick.

    cairo_arc(cr, 0, 0, rUpdate.m_dWidth/4 + 10 * ux, 0,  2 * M_PI);

    // Draw tick(s)

    cairo_move_to(cr, - rUpdate.m_dWidth/4 - 10 * ux, 0 );
    cairo_rel_line_to(cr, 20 * ux, 0);

    cairo_stroke(cr);

    // Draw rest of the ticks.

    cairo_save(cr);

    cairo_set_source_rgb(cr, 0.7, 0.7, 0.9);
    cairo_set_line_width (cr, ux);

    double angle = 0;
    double inc = stepWidth * 2.0 * M_PI;
    while ( true )
    {
        angle += inc;
        if ( abs(angle - 2 * M_PI) < 0.0001 )
            break;
        if ( angle >= 2.0 * M_PI )
            angle -= 2.0 * M_PI;
        cairo_rotate(cr, inc);
        cairo_move_to(cr, - rUpdate.m_dWidth/4 - 10 * ux, 0 );
        cairo_rel_line_to(cr, 15 * ux, 0);
    }

    cairo_stroke(cr);
    cairo_restore (cr);

    // Draw progress indicator.

//    cairo_set_source_rgb(cr, 0, 0.25, 0.5);
    cairo_set_line_width (cr, 20 * ux);
    cairo_arc(cr, 0, 0, rUpdate.m_dWidth/4, 2.0 * M_PI * (progress + 0.5),  2.0 * M_PI * (progress + stepWidth + 0.5) );
    cairo_stroke(cr);

    //
    // Beat within quantum.

	cairo_set_font_size (cr, 2 * m_FontHeight);
	cairo_font_extents (cr, &fe);

	y = fe.ascent - fe.height/2;

    snprintf(text, 80, "%05.2f", beat);
    cairo_text_extents_t te;
	cairo_text_extents (cr, text, &te);
	x = - te.x_advance/2;

//    cairo_set_source_rgb(cr, 0, 0.5, 1.0);
    cairo_move_to (cr, x, y);
    cairo_show_text (cr, text);

    cairo_restore(cr);
    Refresh(rUpdate);

    //
    // Real time beat, queue time.

	rUpdate = Rectangle(27 * m_GridSize, 0.0, 13 * m_GridSize, 2 * m_GridSize);
    rUpdate.ScaleD2I(m_Scale);

	cairo_set_font_size (cr, m_FontHeight);
	cairo_font_extents (cr, &fe);

	x = rUpdate.m_dX;
//	y = m_GridSize + fe.ascent - fe.height/2;
	y = rUpdate.m_dY + rUpdate.m_dHeight/2 + fe.ascent - fe.height/2;

    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
    cairo_rectangle (cr, rUpdate.m_dX, rUpdate.m_dY, rUpdate.m_dWidth, rUpdate.m_dHeight );
    cairo_fill (cr);

    snprintf(text, 80, "Beat %9.2f (Sec %6u.%u)", rtBeat, queueSecs, queueNano);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_move_to (cr, x, y);
    cairo_show_text (cr, text);

    Refresh(rUpdate);

    cairo_destroy(cr);
}

