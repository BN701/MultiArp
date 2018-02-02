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

// Todo: move X key code dependency and set up our own codes.

#define XK_MISCELLANY
#define XK_XKB_KEYS
#define XK_LATIN1
#include <X11/keysymdef.h>  // XK_ Unicode key name defines

//#include <csignal>

#include "maAlsaSequencer.h"
#include "maAlsaSequencerQueue.h"
#include "maCommand.h"
#include "maCursorKeys.h"
#include "maFeelMap.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maStep.h"
#include "maUtility.h"

using namespace std;

// Global Link instance.

ableton::Link g_Link(120.);
chrono::microseconds g_LinkStartTime(-1);

// Global instances.

CursorKeys g_CursorKeys;
ListBuilder g_ListBuilder(&g_Link);
PatternStore g_PatternStore;
AlsaSequencer g_Sequencer;
State g_State;



extern TextUI g_TextUI;
extern CairoUI g_CairoUI;

int g_DeferStop = 0;

//void do_UI_updates()
//{
//
//    // If pattern changed last step ...
//
//    if ( g_PatternStore.PatternChanged(true) )
//    {
//        set_top_line();
//        update_edit_panels();
//        set_status(STAT_POS_2, "Pattern changed ...");
//    }
//
//    // If phase zero last step ...
//
//    if ( g_State.PhaseIsZero() )
//    {
//        update_pattern_status_panel();
//    }
//
//    // Every step ...
//
//    update_progress_bar();
//
////    char text[80];
////    sprintf(text, "Beat%9.2f (Sec%6i:%i)",
////               g_PatternStore.LastRealTimeBeat(),
////               g_Sequencer.ScheduleTimeSeconds(),
////               g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
////
////    set_status_w(STAT_POS_STEP, " Beat%9.2f\n (Sec%6i:%i)",
////               /*g_State.Phase(),*/
//////               g_State.Beat(),
////               g_PatternStore.LastRealTimeBeat(),
////               g_Sequencer.ScheduleTimeSeconds(),
////               g_Sequencer.ScheduleTimeNanoSeconds() / 100000000);
//}

//void do_phase0_updates()
//{
//    g_PatternStore.TranslateTableForPlay().Diags().ResetLog();
//
//    g_State.SetCurrentStepValue(g_PatternStore.StepValue());
//
//    if ( g_State.NewQuantumPending() )
//    {
//        set_status(STAT_POS_2, "New quantum value set.");
//    }
//
//    if ( g_PatternStore.TranslateTableForPlay().NewTransposePending() )
//    {
//        set_status(STAT_POS_2, "Transpose set.");
//    }
//
//    if ( g_PatternStore.NewPatternPending() )
//    {
//        set_status(STAT_POS_2, "Pattern changed.");
//    }
//
//    if ( g_State.NewRunStatePending() )
//    {
//        if ( g_State.RunState() )
//            g_PatternStore.ResetAllPatterns();
//        g_DeferStop = g_State.DeferStop();
//    }
//
//    if ( g_State.PatternReset() != RESET_NONE )
//    {
//        switch ( g_State.PatternReset() )
//        {
//            case RESET_ALL :
//                g_PatternStore.ResetAllPatterns();
//                set_status(STAT_POS_2, "All patterns were reset.");
//                break;
//            case RESET_CURRENT :
//                g_PatternStore.ResetCurrentPattern();
//                set_status(STAT_POS_2, "Current pattern was reset.");
//                break;
//        }
//        g_State.SetPatternReset(RESET_NONE);
//    }
//
//    set_top_line();
//}


//void queue_next_step(int queueId)
//{
//    // We're called when ALSA has played the events we scheduled last time we were here,
//    // so updating position info at this point should reflect what we are hearing.
//
//    do_UI_updates();
//
//    // Now incrememt the step/beat and get on with scheduling the next events.
//
//    g_State.Step(g_PatternStore.StepValueMultiplier());
//
//    // Get time of next step from Link.
//
//    ableton::Link::Timeline timeline = g_Link.captureAppTimeline();
//
//    double nextBeat = g_State.Beat();
//
//    nextBeat = g_PatternStore.FeelMapForPlay().Adjust(nextBeat);
//    chrono::microseconds t_next_usec = timeline.timeAtBeat(nextBeat, g_State.Quantum());
//
//    g_State.SetPhase(timeline.phaseAtTime(t_next_usec, g_State.Quantum()));
//
//    if ( g_State.PhaseIsZero() )
//    {
//        do_phase0_updates();
//        g_PatternStore.SetPhaseIsZero();
//        g_ListBuilder.SetPhaseIsZero(g_State.Beat(), g_State.Quantum());
//    }
//
//    // Set next schedule time on the queue
//
//    int64_t queue_time_usec;
//    if ( g_LinkStartTime.count() < 0 )
//    {
//        g_LinkStartTime = t_next_usec;
//        queue_time_usec = 0;
//    }
//    else
//    {
//        queue_time_usec = (t_next_usec.count() - g_LinkStartTime.count());
//    }
//
//    if ( queue_time_usec < 0 )
//    {
//        // Sometimes at start up link appears to go backwards, especially if
//        // there's another instance of the app running. For now, just keep
//        // reschedule and hope things settle down.
////        raise(SIGINT);
//        queue_time_usec = 0;
//    }
//
//    g_Sequencer.SetScheduleTime(queue_time_usec);
//
//    // Schedule an event to be fired back to our own app which prompts another
//    // arpeggio to be placed in the queue.
//
//    // TODO: We used to do this after scheduling all midi events. Have there
//    //       been any noticable effects of doing it first.
//
//    g_Sequencer.ScheduleEcho(queueId);
//
//    // Step the Pattern Store
//
//    Cluster nextCluster;
//    TrigRepeater repeater;
//    TranslateTable & translator = g_PatternStore.TranslateTableForPlay();
//
//    if ( g_State.RunState() || g_DeferStop-- > 0 )
//    {
//        g_PatternStore.Step(nextCluster, repeater, g_State.Phase(), g_State.LastUsedStepValue(), nextBeat);
//        if ( g_ListBuilder.RealTimeRecord() )
//            nextCluster += *g_ListBuilder.Step(g_State.Phase(), g_State.LastUsedStepValue());
//    }
//
//    if ( nextCluster.Empty() )
//        return;
//
//    double tempo = timeline.tempo();
//
//    /*
//          V, Step Value, is 4 x 'steps per beat'. (This gives the familiar
//          eighth, sixteenths, etc). T, tempo, is 'beats per minute'.
//
//          Steps per beat, v = V/4.
//          Steps per minute = Tv = TV/4
//          Steps per second = TV/240
//          Step length in mSec = 1000*240/TV
//     */
//
//    double stepLengthMilliSecs = 240000.0/(tempo * g_State.LastUsedStepValue());
//    unsigned int duration = lround(stepLengthMilliSecs * (nextCluster.StepsTillNextNote() + g_PatternStore.GateLength()));
//
//    repeater.Init(tempo, stepLengthMilliSecs);
//
//    for ( auto note = nextCluster.m_Notes.begin(); note != nextCluster.m_Notes.end(); note++ )
//    {
//        int noteNumber = note->m_NoteNumber;
//
//        if ( noteNumber < 0 )
//            continue;
//
//        unsigned char noteVelocity;
//
//        // For real time events, move the note ahead or behind
//        // the phase value of the step itself. (We can't move
//        // too far ahead, obviously, but there's no mechanism
//        // yet for dealing with that situation if it happens.)
//
//        double phaseAdjust = note->Phase() - g_State.Phase();
//        int64_t timeAdjust = llround(60000000.0 * phaseAdjust/tempo);
//
//        int64_t queue_time_adjusted = queue_time_usec + timeAdjust;
//
//        if ( note->m_NoteVelocity > 0 )
//            noteVelocity = note->m_NoteVelocity;
//        else
//            noteVelocity = g_PatternStore.NoteVelocity();
//
//        double noteLength = note->Length();
//        if ( lround(noteLength * 100) > 0 )
//        {
//            // Note length here is in beats. Convert to milliseconds.
//            duration = lround(60000.0 * noteLength / tempo);
//        }
//
//        int64_t queue_time_delta = 0;
//        int interval = 0;
//        repeater.Reset(noteVelocity);
//
//        do
//        {
//            int note = translator.TranslateUsingNoteMap(noteNumber, interval);
//            g_Sequencer.SetScheduleTime(queue_time_adjusted + queue_time_delta);
//            g_Sequencer.ScheduleNote(queueId, note, noteVelocity, duration);
//        }
//        while ( repeater.Step(queue_time_delta, interval, noteVelocity) );
//    }
//
//}

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

bool handle_key_input(CursorKeys::key_type_t curKey, xcb_keysym_t sym)
{
    bool result = true;

    if ( curKey != CursorKeys::no_key )
    {
        g_CursorKeys.RouteKey(curKey);
        show_status_after_navigation();
        update_edit_panels();
        return true;
    }

    static string commandString;

    switch (sym)
    {
    case 0xE6: // Ctrl-A
        copy_clipboard(globals_to_string() + g_PatternStore.ToString());
        set_status(STAT_POS_2, "All Data copied to clipboard ...");
        set_status(COMMAND_HOME, "");
        break;

    case 0xA2:  // Ctrl-C, Copy
        copy_clipboard(g_PatternStore.EditPatternToString());
        set_status(STAT_POS_2, "Edit Pattern copied to clipboard ...");
        set_status(COMMAND_HOME, "");
        break;

    case 0xAD2: // Ctrl-V, Paste
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
        place_cursor(COMMAND_HOME);
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
            place_cursor(COMMAND_HOME + commandString.size());
            set_status(COMMAND_HOME, commandString.c_str());
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
        place_cursor(COMMAND_HOME + commandString.size());
        set_status(COMMAND_HOME, commandString.c_str());
        break;

    default:

        if ( sym > 31 && sym < 127 )
        {
            commandString += sym;
            place_cursor(COMMAND_HOME + commandString.size());
            set_status(COMMAND_HOME, commandString.c_str());
        }
#if 0
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
#endif
        break;
    }

    return result;
}

/*
    KEEP these in case we want to keep the option of
    building a purely ncurses version.
*/



//bool key_input_action_ncurses()
//{
//    bool result = true;
//
//    static string commandString;
//
//    int c = getch();
//    const char * key = getkey();

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

//// Use these two maps to search for X Unicode symbols and raw state/keycode combinations
//// that are used purely for menu navigation.
//
//unordered_map<int, CursorKeys::key_type_t> g_xcbSymbolToCursorKey =
//{
//    {XK_Insert, CursorKeys::ins},
//    {XK_Delete, CursorKeys::del},
//    {XK_Down, CursorKeys::down},
//    {XK_Up, CursorKeys::up},
//    {XK_Left, CursorKeys::left},
//    {XK_Right, CursorKeys::right},
//    {XK_Escape, CursorKeys::escape}
//};
//
//// Key values are constructed from (state << 8) + keycode
//unordered_map<int, CursorKeys::key_type_t> g_xcbKeycodeToCursorKey =
//{
//    {0x0170, CursorKeys::shift_page_up},
//    {0x0175, CursorKeys::shift_page_down},
//    {0x0870, CursorKeys::alt_page_up},
//    {0x0875, CursorKeys::alt_page_down},
//    {0x0474, CursorKeys::ctrl_down},
//    {0x046F, CursorKeys::ctrl_up},
//    {0x0471, CursorKeys::ctrl_left},
//    {0x0472, CursorKeys::ctrl_right},
//    {0x0174, CursorKeys::shift_down},
//    {0x016F, CursorKeys::shift_up},
//    {0x0171, CursorKeys::shift_left},
//    {0x0172, CursorKeys::shift_right},
//    {0x0571, CursorKeys::ctrl_shift_left},
//    {0x0572, CursorKeys::ctrl_shift_right},
//    {0x056F, CursorKeys::ctrl_shift_up},
//    {0x0574, CursorKeys::ctrl_shift_down},
//    {0x0177, CursorKeys::shift_delete},
//    {0x0477, CursorKeys::ctrl_delete}
//};
//
//// Utility for diagnostics.
//
//string show_modifiers (uint32_t mask)
//{
//    const char *MODIFIERS[] = {
//            "Shift", "Lock", "Ctrl", "Alt",
//            "Mod2", "Mod3", "Mod4", "Mod5",
//            "Button1", "Button2", "Button3", "Button4", "Button5"
//    };
//
//    string result;
//    for (const char **modifier = MODIFIERS ; mask; mask >>= 1, ++modifier) {
//        if (mask & 1) {
//            if ( ! result.empty() )
//                 result += '+';
//             result += *modifier;
//        }
//    }
//    return result;
//}
//
//
//bool key_input_action_xcb(uint8_t & keycode, uint16_t & state)
//{
//    bool result = true;
//
////    static string commandString;
//
//    // Convert to the character IDs we've been using with ncurses. We may drop this at some point.
//
//    CursorKeys::key_type_t curKey = CursorKeys::no_key;
//
//    xcb_keysym_t sym = g_CairoUI.GetKeysym(keycode, state);
//
//    if ( sym != 0 )
//    {
//        // Look for symbols that are used purely for CursorKey navigation.
//        // (Ins, Del, Up, Down, Left, Right, Esc).
//
//        try
//        {
//            curKey = g_xcbSymbolToCursorKey.at(sym);
//        }
//        catch(...)
//        {
//            // Nothing found, so just fall through to check specific symbols below.
//        }
//    }
//    else
//    {
//        // Look for raw decodes that map to CursorKeys.
//
//        try
//        {
//            // unsigned int lookup = (kr->state << 8) + kr->detail;
//            curKey = g_xcbKeycodeToCursorKey.at( (state << 8) + keycode);
//        }
//        catch(...)
//        {
//            // There's nothing here for us, so show some diagnostics and return.
//            set_status(STAT_POS_2, "X key NULL symbol. Detail/State: %#x/%#x (%s)", keycode, state, show_modifiers(state).c_str());
//            return true;
//        }
//    }
//
//    result = handle_key_input(curKey, sym);
//
////    if ( curKey != CursorKeys::no_key )
////    {
////        g_CursorKeys.RouteKey(curKey);
////        show_status_after_navigation();
////        update_edit_panels();
////        return true;
////    }
////
////    switch (sym)
////    {
////    case 0xE6: // Ctrl-A
////        copy_clipboard(globals_to_string() + g_PatternStore.ToString());
////        set_status(STAT_POS_2, "All Data copied to clipboard ...");
////        set_status(COMMAND_HOME, "");
////        break;
////
////    case 0xA2:  // Ctrl-C, Copy
////        copy_clipboard(g_PatternStore.EditPatternToString());
////        set_status(STAT_POS_2, "Edit Pattern copied to clipboard ...");
////        set_status(COMMAND_HOME, "");
////        break;
////
////    case 0xAD2: // Ctrl-V, Paste
////        set_status(COMMAND_HOME, "");
////        try
////        {
////            int created, updates;
////            load_from_string(get_clipboard(), created, updates);
////            set_status(STAT_POS_2, "Paste: %i updates, %i new patterns created.", updates, created);
////        }
////        catch (string errorMessage)
////        {
////            set_status(STAT_POS_2, "%s", errorMessage.c_str());
////        }
////        update_pattern_status_panel();
////        update_edit_panels();
////        update_pattern_panel();
////        break;
////
////    case XK_Return: // Enter
////        if ( !commandString.empty() )
////        {
////            result = do_command(commandString);
////            commandString.clear();
////        }
////        else if ( g_ListBuilder.HandleKeybInput(10) )
////        {
////            if ( g_ListBuilder.RealTimeRecord() )
////                g_PatternStore.UpdatePattern(g_ListBuilder.RealTimeList(), g_State.Quantum());
////            else
////                g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
////            g_ListBuilder.Clear();
////            update_pattern_panel();
////            set_status(STAT_POS_2, "");
////        }
////        else if ( g_CursorKeys.RouteKey(CursorKeys::enter) )
////        {
////            show_status_after_navigation();
////        }
////        set_status(COMMAND_HOME, "");
////        break;
////
////    case XK_space: // Space bar.
////        if ( commandString.empty() )
////        {
////            if ( g_ListBuilder.HandleKeybInput(XK_space) )
////                show_listbuilder_status();
////        }
////        else
////        {
////            commandString += sym;
////            set_status(COMMAND_HOME, commandString.c_str());
////            place_cursor(COMMAND_HOME + commandString.size());
////        }
////        break;
////
////    case XK_Tab:
////        place_cursor(COMMAND_HOME + commandString.size());
////        g_TextUI.NextBigPanelPage(1);
////        break;
////
////    case XK_ISO_Left_Tab:   // Shift-Tab
////        g_TextUI.NextBigPanelPage(-1);
////        break;
////
////    case XK_BackSpace:
////        if ( commandString.size() > 0 )
////            commandString.pop_back();
////        else if ( g_ListBuilder.HandleKeybInput(CursorKeys::back_space) )
////            show_listbuilder_status();
////        else if ( g_CursorKeys.RouteKey(CursorKeys::back_space) )
////            show_status_after_navigation();
////        set_status(COMMAND_HOME, commandString.c_str());
////        place_cursor(COMMAND_HOME + commandString.size());
////        break;
////
////    default:
////
////        if ( sym > 31 && sym < 127 )
////        {
////            commandString += sym;
////            set_status(COMMAND_HOME, commandString.c_str());
////            place_cursor(COMMAND_HOME + commandString.size());
////        }
////        else if ( true )
////        {
////            // We haven't used the symbol, so what was it?
////
////            string symbol = "X symbol: ";
////            char buff[20];
////            sprintf(buff, "%#x - ", sym);
////            symbol += buff;
////
////            if ( static_cast<unsigned char>(sym) < 32 )
////            {
////                sprintf(buff, "%i", static_cast<unsigned char>(sym));
////                symbol += buff;
////            }
////            else
////                symbol += static_cast<unsigned char>(sym);
////
////            symbol += " Cat:";
////
////            if ( xcb_is_keypad_key(sym) )
////                symbol += " keypad";
////            if ( xcb_is_private_keypad_key(sym) )
////                symbol += " private keypad";
////            if ( xcb_is_cursor_key(sym) )
////                symbol += " cursor";
////            if ( xcb_is_pf_key(sym) )
////                symbol += " pf";
////            if ( xcb_is_function_key(sym) )
////                symbol += " function";
////            if ( xcb_is_misc_function_key(sym) )
////                symbol += " misc function";
////            if ( xcb_is_modifier_key(sym) )
////                symbol += " modifier";
////
////            set_status(STAT_POS_2, "%s - Detail/State: %#x/%#x (%s)", symbol.c_str(), keycode, state, show_modifiers(state).c_str());
////
////        }
////        break;
////    }
//
//    return result;
//}

void sigterm_exit(int sig)
{

//    endwin();
    exit(0);
}

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


    /*
    * Set up polling ...
    *
    * Obtain number of descriptors
    * Allocate an appropriately sized array of struct pollfd (alloca puts them on the stack)
    * Fill in the array.
    *
    */

    int npfd = g_Sequencer.GetFileDescriptorCount();
    struct pollfd *pfd = (struct pollfd *)alloca((npfd + 2) * sizeof(struct pollfd));
    pfd[0].fd = 0;  // stdin
    pfd[0].events = POLLIN;
    pfd[1].fd = g_CairoUI.GetFileDescriptor();
    pfd[1].events = POLLIN;
    g_Sequencer.GetFileDescriptors(pfd + 2, npfd);

    bool gotXWindow = pfd[1].fd != 0;

    // Queue first events

    queue_next_step(queueId);

    // Polling loop

    bool keep_going = true;

    while ( keep_going )
    {
        if ( poll(pfd, npfd + 2, 10000) > 0 )
        {
            bool keyDataValid = false;
            CursorKeys::key_type_t curKey = CursorKeys::no_key;
            xcb_keysym_t sym = 0;
            if ( pfd[0].revents & POLLIN )
            {
                keyDataValid = true;
                g_TextUI.KeyInput(curKey, sym);
//                bool keyDataValid = false;
//                CursorKeys::key_type_t curKey = CursorKeys::no_key;
//                xcb_keysym_t sym = 0;
//                if ( gotXWindow )
//                {
////                    uint8_t keycode;
////                    uint16_t state;
////                    bool keyDataValid = false;
////                    keep_going = g_CairoUI.PollEvents(keyDataValid, keycode, state);
////                    if ( keep_going && keyDataValid )
////                        keep_going = key_input_action_xcb(keycode, state);
//                    keep_going = g_CairoUI.PollEvents(keyDataValid, curKey, sym);
//                }
//                else
//                {
//                    keyDataValid = true;
//                    g_TextUI.KeyInput(curKey, sym);
//                }
//                if ( keyDataValid )
//                {
//                    keep_going = handle_key_input(curKey, sym);
//                }
            }
            else if ( gotXWindow && (pfd[1].revents & POLLIN) )
            {
                keep_going = g_CairoUI.PollEvents(keyDataValid, curKey, sym);
            }
            if ( keep_going && keyDataValid )
            {
                keep_going = handle_key_input(curKey, sym);
            }

            for ( int i = 2; i < npfd + 2; i++ )
            {
                if ( pfd[i].revents > 0 )
                    midi_action(queueId);
            }
        }
    }

}
