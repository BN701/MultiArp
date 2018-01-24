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
//
//    ******************************************************************
//    * Apologies for the alarming mix of coding styles. I'm an ad-hoc *
//    * coder and I've been away from the game for several years.      *
//    * BN, Oct 2017                                                   *
//    ******************************************************************
//
//    Starting from miniArp.c by Matthias Nagorni, a great place to get
//    going with the ALSA Sequencer.
//


//#define _USE_MATH_DEFINES

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>
#include <csignal>
//#include <unordered_map>

//#include <xcb/xcb.h>
//#include <xcb/xcb_keysyms.h>
//#include <cairo-xcb.h>

#define XK_MISCELLANY
#define XK_XKB_KEYS
#define XK_LATIN1
#include <X11/keysymdef.h>  // XK_ Unicode key name defines


#include "maAlsaSequencer.h"
#include "maAlsaSequencerQueue.h"
#include "maCommand.h"
#include "maCursorKeys.h"
#include "maFeelMap.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maUtility.h"

using namespace std;


//typedef struct {
//    int width, height;
//    int scrno;
//    xcb_screen_t *screen;
//    xcb_connection_t *connection;
//    xcb_drawable_t window;
//    unsigned int white;
//    xcb_visualtype_t *visual;
//
//    cairo_surface_t *surface;
//    cairo_t *cr;
//} xcb_Cairo_Window;

//xcb_Cairo_Window xcWindow;

// Global Link instance.

ableton::Link g_Link(120.);
chrono::microseconds g_LinkStartTime(-1);

// Global instances.

CursorKeys g_CursorKeys;
ListBuilder g_ListBuilder(&g_Link);
PatternStore g_PatternStore;
AlsaSequencer g_Sequencer;
State g_State;

//xcb_key_symbols_t * g_xcbKeySymbols = NULL;


extern TextUI g_TextUI;
extern CairoUI g_CairoUI;

int g_DeferStop = 0;
//#define ROWHEIGHT 0.04
//#define FONTHEIGHT 0.03

//vector<string> fontNames = { "Just Checking", "Noto Mono", "Inconsolata", "Dejavu Sans Mono", "Ubuntu Mono", "monospace", "" };
//
//bool HasMonoFont(cairo_t * cr)
//{
//    cairo_text_extents_t te1, te2;
//
//    cairo_text_extents (cr, "....", &te1);
//    cairo_text_extents (cr, "WWWW", &te2);
//
//    return te2.x_advance - te1.x_advance < 0.000001;
//}
//
//bool FindMonoFont(cairo_t * cr)
//{
//    for ( auto name = fontNames.begin(); name != fontNames.end(); name++ )
//    {
//        cairo_select_font_face (cr, name->c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//        if ( HasMonoFont(cr) )
//            return true;
//    }
//    return false;
//}
//
//void drawUI_TextRow(xcb_connection_t *connection, cairo_surface_t *surface, int row, const char * text)
//{
//    cairo_t *cr = cairo_create(surface);
//
//	double x, y /*, px, ux=1, uy=1, dashlength */;
//
//	cairo_font_extents_t fe;
////	cairo_text_extents_t te;
//
//	/* Example is in 26.0 x 1.0 coordinate space */
//	cairo_scale (cr, 500, 500);
//	cairo_set_font_size (cr, FONTHEIGHT);
//
//	FindMonoFont(cr);
//
//	/* Fill (clear) background rectangle */
//    cairo_set_source_rgb(cr, 1, 1, 0.9);
//	cairo_rectangle (cr, 0, row * ROWHEIGHT, 1.6, ROWHEIGHT);
//	cairo_fill (cr);
//
////	/* Drawing code goes here */
////	cairo_select_font_face (cr, "gobbledegook", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
////    status = cairo_status(cr);
////
////    cairo_font_face_t * toyFace = cairo_toy_font_face_create ("Inconsolate", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
////    cairo_font_face_t * fontFace = cairo_get_font_face (cr);
////    cairo_scaled_font_t * scaledFace = cairo_get_scaled_font (cr);
////
////    cairo_set_font_face (cr,toyFace);
////    cairo_font_type_t fontType = cairo_font_face_get_type (fontFace);
////    cairo_status_t fontStatus = cairo_font_face_status (fontFace);
////
////    const char * fontFamily = cairo_toy_font_face_get_family (fontFace); // Just returns the string we supplied via cairo_select_font_face();
////    const char * toyFamily = cairo_toy_font_face_get_family (toyFace); // Just returns the string we supplied via cairo_select_font_face();
////
////	cairo_font_extents (cr, &fe);
//
////	cairo_device_to_user_distance (cr, &ux, &uy);
////	if (ux > uy)
////		px = ux;
////	else
////		px = uy;
//	cairo_font_extents (cr, &fe);
////	cairo_text_extents (cr, text, &te);
//	x = 0.1; // - te.x_bearing - te.width / 2;
//	y = (1 + row) * ROWHEIGHT - fe.descent; // + fe.height / 2);
//
//	/* extents: width & height */
////	cairo_set_source_rgba (cr, 1, 1, 0.9, 0);
////	cairo_set_line_width (cr, px);
////	dashlength = 3*px;
////	cairo_set_dash (cr, &dashlength, 1, 0);
////	cairo_rectangle (cr, x + te.x_bearing, y + te.y_bearing, te.width, te.height);
////	cairo_fill (cr);
//
//	/* text */
//	cairo_move_to (cr, x, y);
//	cairo_set_source_rgb (cr, 1, 0, 0);
//	cairo_show_text (cr, text);
//
//    cairo_surface_flush(surface);
//    cairo_destroy(cr);
//    xcb_flush(connection);
//}
//

void do_UI_updates()
{

    // If pattern changed last step ...

    if ( g_PatternStore.PatternChanged(true) )
    {
        set_top_line();
        update_edit_panels();
        set_status(STAT_POS_2, "Pattern changed ...");
    }

    // If phase zero last step ...

    if ( g_State.PhaseIsZero() )
    {
        update_pattern_status_panel();
    }

    // Every step ...

//    update_pattern_panel();     // TODO: Revert to updating this when updates are actually made.
//    highlight_pattern_panel();  // Moves note highlight.
    update_pattern_panel();
//    g_PatternStore.TranslateTableForPlay().Diags().ResetLog();

    update_progress_bar();

    char text[80];
    sprintf(text, "Beat%9.2f (Sec%6i:%i)",
               g_PatternStore.LastRealTimeBeat(),
               g_Sequencer.ScheduleTimeSeconds(),
               g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);

//    drawUI_TextRow(xcWindow.connection, xcWindow.surface, 5, text);

    set_status_w(STAT_POS_STEP, " Beat%9.2f\n (Sec%6i:%i)",
               /*g_State.Phase(),*/
//               g_State.Beat(),
               g_PatternStore.LastRealTimeBeat(),
               g_Sequencer.ScheduleTimeSeconds(),
               g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
}

void do_phase0_updates()
{
    g_PatternStore.TranslateTableForPlay().Diags().ResetLog();

    g_State.SetCurrentStepValue(g_PatternStore.StepValue());

    if ( g_State.NewQuantumPending() )
    {
        set_status(STAT_POS_2, "New quantum value set.");
    }

    if ( g_PatternStore.TranslateTableForPlay().NewTransposePending() )
    {
        set_status(STAT_POS_2, "Transpose set.");
    }

    if ( g_PatternStore.NewPatternPending() )
    {
        set_status(STAT_POS_2, "Pattern changed.");
    }

    if ( g_State.NewRunStatePending() )
    {
        if ( g_State.RunState() )
            g_PatternStore.ResetAllPatterns();
        g_DeferStop = g_State.DeferStop();
    }

    if ( g_State.PatternReset() != RESET_NONE )
    {
        switch ( g_State.PatternReset() )
        {
            case RESET_ALL :
                g_PatternStore.ResetAllPatterns();
                set_status(STAT_POS_2, "All patterns were reset.");
                break;
            case RESET_CURRENT :
                g_PatternStore.ResetCurrentPattern();
                set_status(STAT_POS_2, "Current pattern was reset.");
                break;
        }
        g_State.SetPatternReset(RESET_NONE);
    }

    set_top_line();
}


void queue_next_step(int queueId)
{
    // We're called when ALSA has played the events we scheduled last time we were here,
    // so updating position info at this point should reflect what we are hearing.

    do_UI_updates();

    // Now incrememt the step/beat and get on with scheduling the next events.

    g_State.Step(g_PatternStore.StepValueMultiplier());

    // Get time of next step from Link.

    ableton::Link::Timeline timeline = g_Link.captureAppTimeline();

    double nextBeat = g_State.Beat();

    nextBeat = g_PatternStore.FeelMapForPlay().Adjust(nextBeat);
    chrono::microseconds t_next_usec = timeline.timeAtBeat(nextBeat, g_State.Quantum());

    g_State.SetPhase(timeline.phaseAtTime(t_next_usec, g_State.Quantum()));

    if ( g_State.PhaseIsZero() )
    {
        do_phase0_updates();
        g_PatternStore.SetPhaseIsZero();
        g_ListBuilder.SetPhaseIsZero(g_State.Beat(), g_State.Quantum());
    }

    // Set next schedule time on the queue

    int64_t queue_time_usec;
    if ( g_LinkStartTime.count() < 0 )
    {
        g_LinkStartTime = t_next_usec;
        queue_time_usec = 0;
    }
    else
    {
        queue_time_usec = (t_next_usec.count() - g_LinkStartTime.count());
    }

    if ( queue_time_usec < 0 )
    {
        // Sometimes at start up link appears to go backwards, especially if
        // there's another instance of the app running. For now, just keep
        // reschedule and hope things settle down.
//        raise(SIGINT);
        queue_time_usec = 0;
    }

    g_Sequencer.SetScheduleTime(queue_time_usec);

    // Schedule an event to be fired back to our own app which prompts another
    // arpeggio to be placed in the queue.

    // TODO: We used to do this after scheduling all midi events. Have there
    //       been any noticable effects of doing it first.

    g_Sequencer.ScheduleEcho(queueId);

    // Step the Pattern Store

    Cluster nextCluster;
    TrigRepeater repeater;
    TranslateTable & translator = g_PatternStore.TranslateTableForPlay();

    if ( g_State.RunState() || g_DeferStop-- > 0 )
    {
        g_PatternStore.Step(nextCluster, repeater, g_State.Phase(), g_State.LastUsedStepValue(), nextBeat);
        if ( g_ListBuilder.RealTimeRecord() )
            nextCluster += *g_ListBuilder.Step(g_State.Phase(), g_State.LastUsedStepValue());
    }

    if ( nextCluster.Empty() )
        return;

    double tempo = timeline.tempo();

    /*
          V, Step Value, is 4 x 'steps per beat'. (This gives the familiar
          eighth, sixteenths, etc). T, tempo, is 'beats per minute'.

          Steps per beat, v = V/4.
          Steps per minute = Tv = TV/4
          Steps per second = TV/240
          Step length in mSec = 1000*240/TV
     */

    double stepLengthMilliSecs = 240000.0/(tempo * g_State.LastUsedStepValue());
    unsigned int duration = lround(stepLengthMilliSecs * (nextCluster.StepsTillNextNote() + g_PatternStore.GateLength()));

    repeater.Init(tempo, stepLengthMilliSecs);

    for ( auto note = nextCluster.m_Notes.begin(); note != nextCluster.m_Notes.end(); note++ )
    {
        int noteNumber = note->m_NoteNumber;

        if ( noteNumber < 0 )
            continue;

        unsigned char noteVelocity;

        // For real time events, move the note ahead or behind
        // the phase value of the step itself. (We can't move
        // too far ahead, obviously, but there's no mechanism
        // yet for dealing with that situation if it happens.)

        double phaseAdjust = note->Phase() - g_State.Phase();
        int64_t timeAdjust = llround(60000000.0 * phaseAdjust/tempo);

        int64_t queue_time_adjusted = queue_time_usec + timeAdjust;

        if ( note->m_NoteVelocity > 0 )
            noteVelocity = note->m_NoteVelocity;
        else
            noteVelocity = g_PatternStore.NoteVelocity();

        double noteLength = note->Length();
        if ( lround(noteLength * 100) > 0 )
        {
            // Note length here is in beats. Convert to milliseconds.
            duration = lround(60000.0 * noteLength / tempo);
        }

        int64_t queue_time_delta = 0;
        int interval = 0;
        repeater.Reset(noteVelocity);

        do
        {
            int note = translator.TranslateUsingNoteMap(noteNumber, interval);
            g_Sequencer.SetScheduleTime(queue_time_adjusted + queue_time_delta);
            g_Sequencer.ScheduleNote(queueId, note, noteVelocity, duration);
        }
        while ( repeater.Step(queue_time_delta, interval, noteVelocity) );
    }

}

void midi_action(int queueId)
{
    snd_seq_event_t *ev;

    // static ListBuilder listBuilder;

    static int otherEvents = 0; // Just for interest.

    do
    {
        g_Sequencer.EventInput(&ev);
        switch (ev->type)
        {
        case SND_SEQ_EVENT_ECHO:
            // This is our 'tick', so schedule everything
            // that should happen next, including the
            // next tick.
            queue_next_step(queueId);
            break;

        case SND_SEQ_EVENT_NOTEON:
        case SND_SEQ_EVENT_NOTEOFF:
            if ( g_ListBuilder.HandleMidi(ev) )
            {
                // HandleMidi() only returns true in QUICK entry
                // mode, where midi input alone is used to manage
                // notelist updates.

                g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
                g_ListBuilder.Clear();
                set_status(STAT_POS_2, "");
                update_pattern_panel();
            }
            else /*if ( ev->type == SND_SEQ_EVENT_NOTEON )*/
            {
                show_listbuilder_status();
            }
            break;
            default:
                otherEvents += 1;
                break;
        }
        snd_seq_free_event(ev);
    }
    while ( g_Sequencer.EventInputPending() );
}

enum global_element_names_t
{
     global_heading,
     global_name_midi_channel,
     global_name_link_quantum,
     num_global_element_names
};


unordered_map<global_element_names_t, const char *> global_element_names = {
    {global_heading, "Global"},
    {global_name_midi_channel, "Midi Channel"},
    {global_name_link_quantum, "Link Quantum"},

};

string globals_to_string()
{
    string result = "<< ";
    result += global_element_names.at(global_heading);
    result += " >>\n\n";

    char buff[200];

    sprintf(buff, "%s %i\n", global_element_names.at(global_name_midi_channel), g_Sequencer.MidiChannel() + 1);
    result += buff;
    sprintf(buff, "%s %.2f\n", global_element_names.at(global_name_link_quantum), g_State.Quantum());
    result += buff;

    result += '\n';

    return result;
}


void load_from_string(string s, int & created, int & updated )
{
    created = 0;
    updated = 0;

    vector<string> errors;
    vector<string> rows = split(s.c_str(), '\n');

    // paste_target_t target = paste_pattern;
    // g_PatternStore.SetPasteTargetPatterns();

    for ( vector<string>::iterator i = rows.begin(); i != rows.end(); i++ )
    {
        try
        {
            if ( g_PatternStore.FromString(*i, created, updated) ) // Give Pattern Store a chance to see the header.
                continue;

            // Handle global fields here (for the time being).

            for ( global_element_names_t e = static_cast<global_element_names_t>(1);
                  e < num_global_element_names;
                  e = static_cast<global_element_names_t>(static_cast<int>(e) + 1) )
            {
                string token = find_token(*i, global_element_names.at(e));

                if ( token.empty() )
                    continue;

                transform(token.begin(), token.end(), token.begin(), ::tolower);

                try
                {
                    switch (e)
                    {
                    case global_name_midi_channel:
                        g_Sequencer.SetMidiChannel(stoi(token) - 1);
                        break;
                    case global_name_link_quantum:
                        g_State.SetNewQuantumPending(stod(token));
                        break;
                    default:
                        break;
                    }
                }
                catch(invalid_argument ex)
                {

                }
                catch(out_of_range ex)
                {
                }
            }
        }
        catch (invalid_argument e)
        {
        }
        catch (string error )
        {
            errors.push_back(error);
        }
    }

    if ( !errors.empty() )
        throw string("Pattern parse error: At least one row contained errors.");

}

//unordered_map<int, CursorKeys::key_type_t> g_CursorKeyMap =
//{
//    {KEY_INSERT, CursorKeys::ins},
//    {KEY_DELETE, CursorKeys::del},
//    {KEY_SDELETE, CursorKeys::shift_delete},
//    {KEY_CDELETE, CursorKeys::ctrl_delete},
//    {KEY_TAB, CursorKeys::tab},
//    {KEY_SHTAB, CursorKeys::shift_tab},
//    {KEY_DOWN, CursorKeys::down},
//    {KEY_UP, CursorKeys::up},
//    {KEY_LEFT, CursorKeys::left},
//    {KEY_RIGHT, CursorKeys::right},
//    {KEY_SPGUP, CursorKeys::shift_page_up},
//    {KEY_SPGDOWN, CursorKeys::shift_page_down},
//    {KEY_APGUP, CursorKeys::alt_page_up},
//    {KEY_APGDOWN, CursorKeys::alt_page_down},
//    {KEY_CDOWN, CursorKeys::ctrl_down},
//    {KEY_CUP, CursorKeys::ctrl_up},
//    {KEY_CLEFT, CursorKeys::ctrl_left},
//    {KEY_CRIGHT, CursorKeys::ctrl_right},
//    {KEY_SDOWN, CursorKeys::shift_down},
//    {KEY_SUP, CursorKeys::shift_up},
//    {KEY_SLEFT, CursorKeys::shift_left},
//    {KEY_SRIGHT, CursorKeys::shift_right},
//    {KEY_CSLEFT, CursorKeys::ctrl_shift_left},
//    {KEY_CSRIGHT, CursorKeys::ctrl_shift_right},
//    {KEY_CSUP, CursorKeys::ctrl_shift_up},
//    {KEY_CSDOWN, CursorKeys::ctrl_shift_down}
//};


//bool key_input_action_ncurses()
//{
//    bool result = true;
//
//    static string commandString;
//
//    int c = getch();
////    const char * key = getkey();
//
//    switch (c)
//    {
//    case 1: // Ctrl-A
//        move(COMMAND_HOME);
//        clrtoeol();
//        refresh();
//        copy_clipboard(globals_to_string() + g_PatternStore.ToString());
//        set_status(STAT_POS_2, "All Data copied to clipboard ...");
//        break;
//        break;
//    case 3:  // Ctrl-C, Copy
//        move(COMMAND_HOME);
//        clrtoeol();
//        refresh();
//        copy_clipboard(g_PatternStore.EditPatternToString());
//        set_status(STAT_POS_2, "Edit Pattern copied to clipboard ...");
//        break;
//
//        // After using Alt-Enter, we seem to get additional Enter - char(10) - messages
//        // on the next keypress, regardless of what that key press actually is. Disabling
//        // this bit of code (probably) doesn't prevent that peculiar behaviour, but will
//        // at least discourage using Alt-Enter or Esc all round.
//
////    case 27: // Alt-Enter sends this without a delay, otherwise it takes about a second to arrive.
////        if ( g_CursorKeys.RouteKey(CursorKeys::escape) )
////        {
////            show_status_after_navigation();
////        }
////        break;
//
//    case 22: // Ctrl-V, Paste
//        move(COMMAND_HOME);
//        clrtoeol();
//        refresh();
//        try
//        {
//            int created, updates;
//            load_from_string(get_clipboard(), created, updates);
//            set_status(STAT_POS_2, "Paste: %i updates, %i new patterns created.", updates, created);
//        }
//        catch (string errorMessage)
//        {
//            set_status(STAT_POS_2, "%s", errorMessage.c_str());
//        }
//        update_pattern_status_panel();
//        update_edit_panels();
//        update_pattern_panel();
//        break;
//
//    case 10: // Enter (Ctl-J *and* Ctl-M will also fire this one.)
//        if ( !commandString.empty() )
//        {
//            result = do_command(commandString/*.c_str()*/);
//            commandString.clear();
//        }
//        else if ( g_ListBuilder.HandleKeybInput(c) )
//        {
//            if ( g_ListBuilder.RealTimeRecord() )
//                g_PatternStore.UpdatePattern(g_ListBuilder.RealTimeList(), g_State.Quantum());
//            else
//                g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
//            g_ListBuilder.Clear();
//            update_pattern_panel();
//            set_status(STAT_POS_2, "");
//        }
//        else if ( g_CursorKeys.RouteKey(CursorKeys::enter) )
//        {
//            show_status_after_navigation();
//        }
//        move(COMMAND_HOME);
//        clrtoeol();
//        break;
//
//    case 32: // Space bar.
//        if ( commandString.empty() )
//        {
//            if ( g_ListBuilder.HandleKeybInput(c) )
//                show_listbuilder_status();
//            move(COMMAND_HOME);
//        }
//        else
//            commandString += c;
//        break;
//
//    case KEY_TAB:
//        move(COMMAND_HOME + commandString.size());
//        g_TextUI.NextBigPanelPage(1);
//        break;
//
//    case KEY_SHTAB:
//        g_TextUI.NextBigPanelPage(-1);
//        break;
//
//    case KEY_SPGUP:
//    case KEY_SPGDOWN:
//    case KEY_APGUP:
//    case KEY_APGDOWN:
//    case KEY_CDOWN:
//    case KEY_CUP:
//    case KEY_CLEFT:
//    case KEY_CRIGHT:
//    case KEY_SDOWN:
//    case KEY_SUP:
//    case KEY_SLEFT:
//    case KEY_SRIGHT:
//    case KEY_CSLEFT:
//    case KEY_CSRIGHT:
//    case KEY_CSUP:
//    case KEY_CSDOWN:
//    case KEY_DOWN:
//    case KEY_UP:
//    case KEY_LEFT:
//    case KEY_RIGHT:
//    case KEY_INSERT:
//    case KEY_DELETE:
//    case KEY_SDELETE:
//    case KEY_CDELETE:
//        g_CursorKeys.RouteKey(g_CursorKeyMap.at(c));
//        show_status_after_navigation();
//        update_edit_panels();
//        break;
//
//    case KEY_BACKSPACE: // 263
//        if ( commandString.size() > 0 )
//            commandString.pop_back();
//        else if ( g_ListBuilder.HandleKeybInput(c) )
//            show_listbuilder_status();
//        else if ( g_CursorKeys.RouteKey(CursorKeys::back_space) )
//            show_status_after_navigation();
//        move(COMMAND_HOME + commandString.size());
//        clrtoeol(); // Assuming the cursor has been put back to correct location.
//        break;
//
//    default:
//        if ( true )
//        {
//            set_status(STAT_POS_2, "Key: %i", c);
//        }
//        else if ( commandString.size() == 0 )
//        {
//            set_status(STAT_POS_2, "");
//        }
//        if ( c > 31 && c < 127 )
//            commandString += c;
//        break;
//    }
//
//    refresh();
//
//    return result;
//}

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


bool key_input_action_xcb(uint8_t & keycode, uint16_t & state)
{
    bool result = true;

    static string commandString;

    // Convert to the character IDs we've been using with ncurses. We may drop this at some point.

    CursorKeys::key_type_t curKey = CursorKeys::no_key;

//    xcb_keysym_t sym = xcb_key_symbols_get_keysym(g_xcbKeySymbols, kr->detail, kr->state);
    xcb_keysym_t sym = g_CairoUI.GetKeysym(keycode, state);

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
            set_status(STAT_POS_2, "X key NULL symbol. Detail/State: %#x/%#x (%s)", keycode, state, show_modifiers(state).c_str());
            return true;
        }
    }

    if ( curKey != CursorKeys::no_key )
    {
        g_CursorKeys.RouteKey(curKey);
        show_status_after_navigation();
        update_edit_panels();
        return true;
    }

    switch (sym)
    {
    case 0xE6: // Ctrl-A
//        move(COMMAND_HOME);
//        clrtoeol();
//        refresh();
        copy_clipboard(globals_to_string() + g_PatternStore.ToString());
        set_status(STAT_POS_2, "All Data copied to clipboard ...");
        set_status(COMMAND_HOME, "");
        break;

    case 0xA2:  // Ctrl-C, Copy
//        move(COMMAND_HOME);
//        clrtoeol();
//        refresh();
        copy_clipboard(g_PatternStore.EditPatternToString());
        set_status(STAT_POS_2, "Edit Pattern copied to clipboard ...");
        set_status(COMMAND_HOME, "");
        break;

    case 0xAD2: // Ctrl-V, Paste
//        move(COMMAND_HOME);
//        clrtoeol();
//        refresh();
        set_status(COMMAND_HOME, "");
        try
        {
            int created, updates;
            load_from_string(get_clipboard(), created, updates);
            set_status(STAT_POS_2, "Paste: %i updates, %i new patterns created.", updates, created);
        }
        catch (string errorMessage)
        {
            set_status(STAT_POS_2, "%s", errorMessage.c_str());
        }
        update_pattern_status_panel();
        update_edit_panels();
        update_pattern_panel();
        break;

    case XK_Return: // Enter
        if ( !commandString.empty() )
        {
            result = do_command(commandString);
            commandString.clear();
//            drawUI_TextRow(xcWindow.connection, xcWindow.surface, 2, commandString.c_str());
        }
        else if ( g_ListBuilder.HandleKeybInput(10) )
        {
            if ( g_ListBuilder.RealTimeRecord() )
                g_PatternStore.UpdatePattern(g_ListBuilder.RealTimeList(), g_State.Quantum());
            else
                g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
            g_ListBuilder.Clear();
            update_pattern_panel();
            set_status(STAT_POS_2, "");
        }
        else if ( g_CursorKeys.RouteKey(CursorKeys::enter) )
        {
            show_status_after_navigation();
        }
//        move(COMMAND_HOME);
//        clrtoeol();
        set_status(COMMAND_HOME, "");
        break;

    case XK_space: // Space bar.
        if ( commandString.empty() )
        {
            if ( g_ListBuilder.HandleKeybInput(XK_space) )
                show_listbuilder_status();
        }
        else
        {
            commandString += sym;
            set_status(COMMAND_HOME, commandString.c_str());
            place_cursor(COMMAND_HOME + commandString.size());
//            drawUI_TextRow(xcWindow.connection, xcWindow.surface, 2, commandString.c_str());
        }
        break;

    case XK_Tab:
        place_cursor(COMMAND_HOME + commandString.size());
        g_TextUI.NextBigPanelPage(1);
        break;

    case XK_ISO_Left_Tab:   // Shift-Tab
        g_TextUI.NextBigPanelPage(-1);
        break;

    case XK_BackSpace:
        if ( commandString.size() > 0 )
            commandString.pop_back();
        else if ( g_ListBuilder.HandleKeybInput(CursorKeys::back_space) )
            show_listbuilder_status();
        else if ( g_CursorKeys.RouteKey(CursorKeys::back_space) )
            show_status_after_navigation();
        set_status(COMMAND_HOME, commandString.c_str());
        place_cursor(COMMAND_HOME + commandString.size());
//        drawUI_TextRow(xcWindow.connection, xcWindow.surface, 2, commandString.c_str());
        break;

    default:

        if ( sym > 31 && sym < 127 )
        {
            commandString += sym;
            set_status(COMMAND_HOME, commandString.c_str());
            place_cursor(COMMAND_HOME + commandString.size());
//            drawUI_TextRow(xcWindow.connection, xcWindow.surface, 2, commandString.c_str());
        }
        else if ( true )
        {
            // We haven't used the symbol, so what was it?

            string symbol = "X symbol: ";
            char buff[20];
            sprintf(buff, "%#x - ", sym);
            symbol += buff;

            if ( static_cast<unsigned char>(sym) < 32 )
            {
                sprintf(buff, "%i", static_cast<unsigned char>(sym));
                symbol += buff;
            }
            else
                symbol += static_cast<unsigned char>(sym);

            symbol += " Cat:";

            if ( xcb_is_keypad_key(sym) )
                symbol += " keypad";
            if ( xcb_is_private_keypad_key(sym) )
                symbol += " private keypad";
            if ( xcb_is_cursor_key(sym) )
                symbol += " cursor";
            if ( xcb_is_pf_key(sym) )
                symbol += " pf";
            if ( xcb_is_function_key(sym) )
                symbol += " function";
            if ( xcb_is_misc_function_key(sym) )
                symbol += " misc function";
            if ( xcb_is_modifier_key(sym) )
                symbol += " modifier";

            set_status(STAT_POS_2, "%s - Detail/State: %#x/%#x (%s)", symbol.c_str(), keycode, state, show_modifiers(state).c_str());

        }
        break;
    }

//    refresh();

    return result;
}

void sigterm_exit(int sig)
{

//    endwin();
    exit(0);
}

//static xcb_visualtype_t *find_visual(xcb_connection_t *c, xcb_visualid_t visual)
//{
//    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(c));
//
//    for (; screen_iter.rem; xcb_screen_next(&screen_iter)) {
//        xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
//        for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
//            xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
//            for (; visual_iter.rem; xcb_visualtype_next(&visual_iter))
//                if (visual == visual_iter.data->visual_id)
//                    return visual_iter.data;
//        }
//    }
//
//    return NULL;
//}

void draw_something(cairo_surface_t *surface, cairo_t *cr)
{
    cairo_set_source_rgb(cr, 1, 0.5, 0.5);
    cairo_paint(cr);

#if 0
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 150, 0);
    cairo_line_to(cr, 150, 150);
    cairo_close_path(cr);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_set_line_width(cr, 20);
    cairo_move_to(cr, 0, 150);
    cairo_line_to(cr, 150, 0);
    cairo_stroke(cr);
#else
	double x, y, px, ux=1, uy=1, dashlength;
	char text[]="-?-";
	cairo_font_extents_t fe;
	cairo_text_extents_t te;

	/* Prepare drawing area */
//	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 240);
//	cr = cairo_create (surface);
	/* Example is in 26.0 x 1.0 coordinate space */
	cairo_scale (cr, 500, 500);
	cairo_set_font_size (cr, 0.5);

	/* Drawing code goes here */
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_select_font_face (cr, "monospace",
			CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

	cairo_font_extents (cr, &fe);

	cairo_device_to_user_distance (cr, &ux, &uy);
	if (ux > uy)
		px = ux;
	else
		px = uy;
	cairo_font_extents (cr, &fe);
	cairo_text_extents (cr, text, &te);
	x = 0.5 - te.x_bearing - te.width / 2;
	y = 0.5 - fe.descent + fe.height / 2;

	/* baseline, descent, ascent, height */
	cairo_set_line_width (cr, 4*px);
	dashlength = 9*px;
	cairo_set_dash (cr, &dashlength, 1, 0);
	cairo_set_source_rgba (cr, 0, 0.6, 0, 0.5);
	cairo_move_to (cr, x + te.x_bearing, y);
	cairo_rel_line_to (cr, te.width, 0);
	cairo_move_to (cr, x + te.x_bearing, y + fe.descent);
	cairo_rel_line_to (cr, te.width, 0);
	cairo_move_to (cr, x + te.x_bearing, y - fe.ascent);
	cairo_rel_line_to (cr, te.width, 0);
	cairo_move_to (cr, x + te.x_bearing, y - fe.height);
	cairo_rel_line_to (cr, te.width, 0);
	cairo_stroke (cr);

	/* extents: width & height */
	cairo_set_source_rgba (cr, 0, 0, 0.75, 0.5);
	cairo_set_line_width (cr, px);
	dashlength = 3*px;
	cairo_set_dash (cr, &dashlength, 1, 0);
	cairo_rectangle (cr, x + te.x_bearing, y + te.y_bearing, te.width, te.height);
	cairo_stroke (cr);

	/* text */
	cairo_move_to (cr, x, y);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_show_text (cr, text);

	/* bearing */
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_set_line_width (cr, 2 * px);
	cairo_set_source_rgba (cr, 0, 0, 0.75, 0.5);
	cairo_move_to (cr, x, y);
	cairo_rel_line_to (cr, te.x_bearing, te.y_bearing);
	cairo_stroke (cr);

	/* text's advance */
	cairo_set_source_rgba (cr, 0, 0, 0.75, 0.5);
	cairo_arc (cr, x + te.x_advance, y + te.y_advance, 5 * px, 0, 2 * M_PI);
	cairo_fill (cr);

	/* reference point */
	cairo_arc (cr, x, y, 5 * px, 0, 2 * M_PI);
	cairo_set_source_rgba (cr, 0.75, 0, 0, 0.5);
	cairo_fill (cr);


#endif

    cairo_surface_flush(surface);
}

//bool xcbPollEvents(xcb_connection_t * connection)
//{
//    bool result = true;
//
//    xcb_generic_event_t *event;
//    while ( (event = xcb_poll_for_event (connection)) ) {
//        char text[80];
//        snprintf(text, 80, "X event: %i (Raw: %#0x)", event->response_type & ~0x80, event->response_type);
////        set_status(STAT_POS_2, text);
////        drawUI_TextRow(connection, surface, 3, text);
//        g_CairoUI.Text(BaseUI::whole_screen, 25, 0, text);
//        switch (event->response_type & ~0x80) {
//
//        case XCB_EXPOSE: {
////            xcb_expose_event_t *expose = (xcb_expose_event_t *)event;
////
////            printf ("Window %" PRIu32 " exposed. Region to be redrawn at location (%" PRIu16 ",%" PRIu16 "), with dimension (%" PRIu16 ",%" PRIu16 ")\n",
////                    expose->window, expose->x, expose->y, expose->width, expose->height );
//             /* Avoid extra redraws by checking if this is
//             * the last expose event in the sequence
//             */
//            if (((xcb_expose_event_t *) event)->count != 0)
//                break;
//
////            drawUI(connection, surface, cr);
////            draw_something(surface, cr);
////            xcb_flush(connection);
//
//            break;
//        }
////        case XCB_BUTTON_PRESS: {
////            xcb_button_press_event_t *bp = (xcb_button_press_event_t *)event;
////            print_modifiers (bp->state);
////
////            switch (bp->detail) {
////            case 4:
////                printf ("Wheel Button up in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
////                        bp->event, bp->event_x, bp->event_y );
////                break;
////            case 5:
////                printf ("Wheel Button down in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
////                        bp->event, bp->event_x, bp->event_y );
////                break;
////            default:
////                printf ("Button %" PRIu8 " pressed in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
////                        bp->detail, bp->event, bp->event_x, bp->event_y );
////                break;
////            }
////            break;
////        }
////        case XCB_BUTTON_RELEASE: {
////            xcb_button_release_event_t *br = (xcb_button_release_event_t *)event;
////            print_modifiers(br->state);
////
////            printf ("Button %" PRIu8 " released in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
////                    br->detail, br->event, br->event_x, br->event_y );
////            break;
////        }
////        case XCB_MOTION_NOTIFY: {
////            xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *)event;
////
////            printf ("Mouse moved in window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
////                    motion->event, motion->event_x, motion->event_y );
////            break;
////        }
////        case XCB_ENTER_NOTIFY: {
////            xcb_enter_notify_event_t *enter = (xcb_enter_notify_event_t *)event;
////
////            printf ("Mouse entered window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
////                    enter->event, enter->event_x, enter->event_y );
////            break;
////        }
////        case XCB_LEAVE_NOTIFY: {
////            xcb_leave_notify_event_t *leave = (xcb_leave_notify_event_t *)event;
////
////            printf ("Mouse left window %" PRIu32 ", at coordinates (%" PRIi16 ",%" PRIi16 ")\n",
////                    leave->event, leave->event_x, leave->event_y );
////            break;
////        }
////        case XCB_KEY_PRESS: {
////            xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
////            print_modifiers(kp->state);
////
////            printf ("Key pressed in window %" PRIu32 "\n",
////                    kp->event);
////            break;
////        }
////        case XCB_KEY_RELEASE:
//        case XCB_KEY_PRESS:
//            {
//                xcb_key_release_event_t *kr = (xcb_key_release_event_t *) event;
//                result = key_input_action_xcb(kr);
//            }
//            break;
//
//        default:
////            /* Unknown event type, ignore it */
////            printf ("Unknown event: %" PRIu8 "\n",
////                    event->response_type);
////            drawUI(connection, surface, cr);
//            break;
//        }
//
//        free (event);
//    }
//
//    return result;
//
//}

//int windowHeight = 500;
//int windowWidth = 800;

int main(int argc, char *argv[])
{
    g_PatternStore.SetFocus();
    do_command_line(argc, argv);

    // Initialize ...

    g_Link.enable(true); // Start peer-to-peer interactions.

    int queueIndex = g_Sequencer.CreateQueue();
    int queueId = g_Sequencer.Queue(queueIndex).GetQueueId();

    update_pattern_status_panel();

    // Start the queue.

    g_Sequencer.Queue(queueIndex).Start();

    set_top_line();

    // Termination handlers.

    signal(SIGINT, sigterm_exit);
    signal(SIGTERM, sigterm_exit);

    // Define Cairo surface and context ...

//    cairo_surface_t *surface;
//    cairo_t *cr;

    // Open the connection to the X server
//    xcWindow.connection = xcb_connect (NULL, &xcWindow.scrno);
//
//
//    // Get the first screen
//    const xcb_setup_t *setup = xcb_get_setup (xcWindow.connection);
//    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator (setup);
//    xcWindow.screen = screen_iter.data;
//
//    xcWindow.width = 800;
//    xcWindow.height = 500;
//
//    /*
//        If we actually want to iterate through the screens, we do something like:
//
//        while ( screen_iter.rem > 0 )   // I guess 'rem' is remaining.
//        {
//            ... ;
//            xcb_screen_next(&screen_iter));
//        }
//     */
//
//    // Create the window
//    xcWindow.window    = xcb_generate_id (xcWindow.connection);
//
//    uint32_t     mask      = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
//    uint32_t     values[2] = {xcWindow.screen->white_pixel,
//                                XCB_EVENT_MASK_EXPOSURE       |
//                                XCB_EVENT_MASK_VISIBILITY_CHANGE |
//                                XCB_EVENT_MASK_FOCUS_CHANGE      |
//                                // XCB_EVENT_MASK_BUTTON_PRESS   |
//                                // XCB_EVENT_MASK_BUTTON_RELEASE |
//                                // XCB_EVENT_MASK_POINTER_MOTION |
//                                // XCB_EVENT_MASK_ENTER_WINDOW   |
//                                // XCB_EVENT_MASK_LEAVE_WINDOW   |
//                                XCB_EVENT_MASK_KEY_PRESS      |
//                                XCB_EVENT_MASK_KEY_RELEASE };
//
//    xcb_create_window (xcWindow.connection,
////                       0,                             /* depth               */
//                       XCB_COPY_FROM_PARENT,                             /* depth               */
//                       xcWindow.window,
//                       xcWindow.screen->root,                  /* parent window       */
//                       20, 40,                          /* x, y                */
//                       xcWindow.width, xcWindow.height,     /* width, height       */
//                       10,                            /* border_width        */
//                       XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
////                       xcWindow.screen->root_visual,           /* visual              */
//                       XCB_COPY_FROM_PARENT,           /* visual              */
//                       mask, values );                /* masks */
//
//    // Map the window on the screen
//    xcb_map_window (xcWindow.connection, xcWindow.window);
//
//    // Attach the Cairo surface to the Window.
//
//    xcWindow.visual = find_visual(xcWindow.connection, xcWindow.screen->root_visual);
//    if ( xcWindow.visual == NULL) {
//        fprintf(stderr, "Some weird internal error...?!");
//        xcb_disconnect(xcWindow.connection);
//        return 1;
//    }
//
//
//    // Flush ...
//
//    xcb_flush (xcWindow.connection);
//
//    xcWindow.surface = cairo_xcb_surface_create(xcWindow.connection, xcWindow.window, xcWindow.visual, xcWindow.width, xcWindow.height);
//    xcWindow.cr = cairo_create(xcWindow.surface);
//
//
//    typedef struct _XCBKeySymbols xcb_key_symbols_t;
//    xcb_key_symbols_get_keysym(connection);
//    g_xcbKeySymbols = xcb_key_symbols_alloc(xcWindow.connection);

    /*
    * Set up polling ...
    *
    * Obtain number of descriptors
    * Allocate an appropriately sized array of struct pollfd (alloca puts them on the stack)
    * Fill in the array.
    *
    */

    int npfd = g_Sequencer.GetFileDescriptorCount();
    struct pollfd *pfd = (struct pollfd *)alloca((npfd + 1) * sizeof(struct pollfd));
    pfd[0].fd = g_CairoUI.GetFileDescriptor();
    pfd[0].events = POLLIN;
    g_Sequencer.GetFileDescriptors(pfd + 1, npfd);

    // Queue first events

    queue_next_step(queueId);

    // Polling loop

    bool keep_going = true;

    while ( keep_going )
    {
        if ( poll(pfd, npfd + 1, 10000) > 0 )
        {
            if ( pfd[0].revents & POLLIN )
            {
                uint8_t keycode;
                uint16_t state;
                bool keyDataValid = false;
                keep_going = g_CairoUI.PollEvents(keyDataValid, keycode, state);
                if ( keep_going && keyDataValid )
                    keep_going = key_input_action_xcb(keycode, state);
            }
            for ( int i = 1; i < npfd + 1; i++ )
            {
                if ( pfd[i].revents > 0 )
                    midi_action(queueId);
            }
        }
    }

//    xcb_key_symbols_free(g_xcbKeySymbols);
}
