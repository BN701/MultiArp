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


#include "maCairoUI.h"

#include <string>
#include <vector>

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


CairoUI::CairoUI()
{
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

    // Create the window
    m_Window    = xcb_generate_id (m_Connection);

    uint32_t     mask      = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t     values[2] = {m_Screen->white_pixel,
                                XCB_EVENT_MASK_EXPOSURE       |
                                XCB_EVENT_MASK_VISIBILITY_CHANGE |
                                XCB_EVENT_MASK_FOCUS_CHANGE      |
                                // XCB_EVENT_MASK_BUTTON_PRESS   |
                                // XCB_EVENT_MASK_BUTTON_RELEASE |
                                // XCB_EVENT_MASK_POINTER_MOTION |
                                // XCB_EVENT_MASK_ENTER_WINDOW   |
                                // XCB_EVENT_MASK_LEAVE_WINDOW   |
                                XCB_EVENT_MASK_KEY_PRESS      |
                                XCB_EVENT_MASK_KEY_RELEASE };

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

    // Map the window on the screen
    xcb_map_window (m_Connection, m_Window);

    // Attach the Cairo surface to the Window.

    m_Visual = find_visual(m_Connection, m_Screen->root_visual);
    if ( m_Visual == NULL) {
//        fprintf(stderr, "Some weird internal error...?!");
        xcb_disconnect(m_Connection);
        return;
    }


    // Flush ...

    xcb_flush (m_Connection);

    m_Surface = cairo_xcb_surface_create(m_Connection, m_Window, m_Visual, m_Width, m_Height);
//    xcWindow.cr = cairo_create(xcWindow.surface);



    m_KeySymbols = xcb_key_symbols_alloc(m_Connection);

}


CairoUI::~CairoUI()
{
    xcb_key_symbols_free(m_KeySymbols);
    xcb_disconnect(m_Connection);
}

xcb_keysym_t CairoUI::GetKeysym(int detail, int state)
{
    return xcb_key_symbols_get_keysym(m_KeySymbols, detail, state);
}

int CairoUI::GetFileDescriptor()
{
    return xcb_get_file_descriptor(m_Connection);
}

bool CairoUI::PollEvents(bool & gotKeyData, uint8_t & keycode, uint16_t & state)
{
    bool result = true;
    gotKeyData = false;

    xcb_generic_event_t *event;
    while ( (event = xcb_poll_for_event (m_Connection)) ) {
        char text[80];
        snprintf(text, 80, "X event: %i (Raw: %#0x)", event->response_type & ~0x80, event->response_type);
//        set_status(STAT_POS_2, text);
//        drawUI_TextRow(connection, surface, 3, text);
        Text(whole_screen, 25, 0, text);
        switch (event->response_type & ~0x80) {

        case XCB_EXPOSE: {
//            xcb_expose_event_t *expose = (xcb_expose_event_t *)event;
//
//            printf ("Window %" PRIu32 " exposed. Region to be redrawn at location (%" PRIu16 ",%" PRIu16 "), with dimension (%" PRIu16 ",%" PRIu16 ")\n",
//                    expose->window, expose->x, expose->y, expose->width, expose->height );
             /* Avoid extra redraws by checking if this is
             * the last expose event in the sequence
             */
            if (((xcb_expose_event_t *) event)->count != 0)
                break;

//            drawUI(connection, surface, cr);
//            draw_something(surface, cr);
//            xcb_flush(connection);

            break;
        }
//        case XCB_BUTTON_PRESS: {
//            xcb_button_press_event_t *bp = (xcb_button_press_event_t *)event;
//            print_modifiers (bp->state);
//
//            switch (bp->detail) {
//            case 4:
//                printf ("Wheel Button up in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
//                        bp->event, bp->event_x, bp->event_y );
//                break;
//            case 5:
//                printf ("Wheel Button down in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
//                        bp->event, bp->event_x, bp->event_y );
//                break;
//            default:
//                printf ("Button %" PRIu8 " pressed in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
//                        bp->detail, bp->event, bp->event_x, bp->event_y );
//                break;
//            }
//            break;
//        }
//        case XCB_BUTTON_RELEASE: {
//            xcb_button_release_event_t *br = (xcb_button_release_event_t *)event;
//            print_modifiers(br->state);
//
//            printf ("Button %" PRIu8 " released in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
//                    br->detail, br->event, br->event_x, br->event_y );
//            break;
//        }
//        case XCB_MOTION_NOTIFY: {
//            xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
//
//            printf ("Mouse moved in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
//                    motion->event, motion->event_x, motion->event_y );
//            break;
//        }
//        case XCB_ENTER_NOTIFY: {
//            xcb_enter_notify_event_t *enter = (xcb_enter_notify_event_t *)event;
//
//            printf ("Mouse entered window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
//                    enter->event, enter->event_x, enter->event_y );
//            break;
//        }
//        case XCB_LEAVE_NOTIFY: {
//            xcb_leave_notify_event_t *leave = (xcb_leave_notify_event_t *)event;
//
//            printf ("Mouse left window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
//                    leave->event, leave->event_x, leave->event_y );
//            break;
//        }
//        case XCB_KEY_PRESS: {
//            xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
//            print_modifiers(kp->state);
//
//            printf ("Key pressed in window %" PRIu32 "\n",
//                    kp->event);
//            break;
//        }
//        case XCB_KEY_RELEASE:
        case XCB_KEY_PRESS:
            {
                xcb_key_release_event_t *kr = (xcb_key_release_event_t *) event;
//                result = key_input_action_xcb(kr);
                gotKeyData = true;
                keycode = kr->detail;
                state = kr->state;
            }
            break;

        default:
//            /* Unknown event type, ignore it */
//            printf ("Unknown event: %" PRIu8 "\n",
//                    event->response_type);
//            drawUI(connection, surface, cr);
            break;
        }

        free (event);
    }

    return result;
}


vector<string> fontNames = { "Just Checking", "Noto Mono", "Inconsolata", "Dejavu Sans Mono", "Ubuntu Mono", "monospace", "" };

bool HasMonoFont(cairo_t * cr)
{
    cairo_text_extents_t te1, te2;

    cairo_text_extents (cr, "....", &te1);
    cairo_text_extents (cr, "WWWW", &te2);

    return te2.x_advance - te1.x_advance < 0.000001;
}

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

void CairoUI::Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute)
//void drawUI_TextRow(xcb_connection_t *connection, cairo_surface_t *surface, int row, const char * text)
{
    cairo_t *cr = cairo_create(m_Surface);

	double x, y /*, px, ux=1, uy=1, dashlength */;

	cairo_font_extents_t fe;
//	cairo_text_extents_t te;

	/* Example is in 26.0 x 1.0 coordinate space */
	cairo_scale (cr, 500, 500);
	cairo_set_font_size (cr, m_FontHeight);

	FindMonoFont(cr);

	/* Fill (clear) background rectangle */
    cairo_set_source_rgb(cr, 1, 1, 0.9);
	cairo_rectangle (cr, 0, row * m_RowHeight, 1.6, m_RowHeight);
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
	y = (1 + row) * m_RowHeight - fe.descent; // + fe.height / 2);

	/* extents: width & height */
//	cairo_set_source_rgba (cr, 1, 1, 0.9, 0);
//	cairo_set_line_width (cr, px);
//	dashlength = 3*px;
//	cairo_set_dash (cr, &dashlength, 1, 0);
//	cairo_rectangle (cr, x + te.x_bearing, y + te.y_bearing, te.width, te.height);
//	cairo_fill (cr);

	/* text */
	cairo_move_to (cr, x, y);
	cairo_set_source_rgb (cr, 1, 0, 0);
	cairo_show_text (cr, text);

    cairo_surface_flush(m_Surface);
    cairo_destroy(cr);
    xcb_flush(m_Connection);
}


void CairoUI::Progress(double progress, double stepWidth, double beat, int pattern_progress)
{
}

