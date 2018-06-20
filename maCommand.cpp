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

#include "maCommand.h"
#include "maCommandMenu.h"

#include <algorithm>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include <array>

#ifdef MA_BLUE
#include <cstdio>
#include <cstdlib>
#include "maSequencer.h"
#else
#include "platform_Linux/maAlsaSequencer.h"
#include "platform_Linux/maAlsaSequencerQueue.h"
#endif // MA_BLUE

//#include "maItemMenu.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maTranslateTable.h"
#include "maUtility.h"

using namespace std;

// Global objects.

#ifndef MA_BLUE
#endif

extern AnsiUI g_TextUI;

unordered_map<string, command_t> g_CommandList =
{
#if !defined(MA_BLUE) || defined(MA_BLUE_PC)
    {"exit", C_EXIT},
    {"quit", C_EXIT},
#endif

    {"/", C_MENU},

    {"help", C_HELP},
    {"cls", C_RESET_SCREEN},

    {"crash", C_CRASH_TEST},

    {"label", C_SET_LABEL},
    {"name", C_SET_LABEL},
    {"n", C_SET_LABEL},

    {"page one", C_PAGE_ONE},
    {"page 1", C_PAGE_ONE},
    {"p 1", C_PAGE_ONE},
    {"page two", C_PAGE_TWO},
    {"page 2", C_PAGE_TWO},
    {"p 2", C_PAGE_TWO},
    {"page three", C_PAGE_THREE},
    {"page 3", C_PAGE_THREE},
    {"p 3", C_PAGE_THREE},
    {"page hold", C_PAGE_HOLD},
    {"p hold", C_PAGE_HOLD},

    {"run", C_RUN},
    {"stop", C_STOP},
    {"stop now", C_HALT},
    {"halt", C_HALT},
    {"rec", C_REC_TOGGLE},

    {"reset", C_RESET},
    {"reset beat", C_RESET_BEAT},
    {"autoreset", C_SET_RESETONPATTERNCHANGE},

    {"note", C_STEPVAL},
    {"step", C_STEPVAL},
    {"quantum", C_QUANTUM},
    {"q", C_QUANTUM},
    {"gate", C_GATELENGTH},
    {"gate hold", C_GATE_HOLD,},
    {"gate h", C_GATE_HOLD,},
    {"gate normal", C_GATE_NORMAL,},
    {"gate n", C_GATE_NORMAL,},
    {"tempo", C_TEMPO},
    {"te", C_TEMPO},

    {"velocity", C_VELOCITY},
    {"vel", C_VELOCITY},

    {"tran", C_TRANSPOSE},
    {"transpose", C_TRANSPOSE},

    {"trigs", C_TRIGS},
    {"t", C_TRIGS},
    {"trigs arp", C_TRIGS_ARPEGGIO},
    {"t arp", C_TRIGS_ARPEGGIO},

    {"scale", C_SCALE},
    {"s", C_SCALE},
    {"scale from list", C_SCALE_FROM_LIST},
    {"scale show", C_SCALE_SHOW},
    {"scale help", C_SCALE_HELP},
    {"help scale", C_SCALE_HELP},
    {"scale controls", C_SCALE_CONTROLS},
    {"scale c", C_SCALE_CONTROLS},
    {"scale k", C_SCALE_CONTROLS},
    {"root", C_SETROOT},

    {"feel", C_FEEL},
    {"f", C_FEEL},
    {"feel help", C_FEEL_HELP},
    {"f help", C_FEEL_HELP},
    {"help feel", C_FEEL_HELP},
    {"feel on", C_FEEL_ON},
    {"f on", C_FEEL_ON},
    {"feel off", C_FEEL_OFF},
    {"f off", C_FEEL_OFF},
    {"feel new", C_FEEL_NEW},
    {"f new", C_FEEL_NEW},
    {"feel add", C_FEEL_ADD},
    {"f add", C_FEEL_ADD},
    {"feel remove", C_FEEL_REMOVE},
    {"f remove", C_FEEL_REMOVE},
    {"feel new", C_FEEL_NEW},
    {"f new", C_FEEL_NEW},
    {"feel respace", C_FEEL_RESPACE},
    {"f respace", C_FEEL_RESPACE},

    {"channel", C_MIDI},
    {"chan", C_MIDI},
    {"midi", C_MIDI},
    {"m", C_MIDI},
    {"midi real time", C_MIDI_REAL_TIME},
    {"midi rt", C_MIDI_REAL_TIME},
    {"m rt", C_MIDI_REAL_TIME},
    {"midi step", C_MIDI_STEP},
    {"midi s", C_MIDI_STEP},
    {"m s", C_MIDI_STEP},
    {"midi quick", C_MIDI_QUICK},
    {"midi q", C_MIDI_QUICK},
    {"m q", C_MIDI_QUICK},
//    {"midi off", C_MIDI_OFF},
//    {"m off", C_MIDI_OFF},

    {"edit", C_EDIT},
    {"e", C_EDIT},
    {"cue", C_CUE},
    {"play", C_CUE},
    {"new", C_NEW_PATTERN},
    {"copy", C_COPY},
    {"delete", C_DELETE},
    {"del", C_DELETE},
    {"clear", C_CLEAR},

    {"chain", C_PATTERN_CHAIN},
    {"c", C_PATTERN_CHAIN},
    {"chain:", C_PATTERN_CHAIN},

    {"chain off", C_PATTERN_CHAIN_OFF},
    {"chain natural", C_PATTERN_CHAIN_NATURAL},
    {"chain n", C_PATTERN_CHAIN_NATURAL},
    {"c n", C_PATTERN_CHAIN_NATURAL},
    {"chain quantum", C_PATTERN_CHAIN_QUANTUM},
    {"chain q", C_PATTERN_CHAIN_QUANTUM},
    {"c q", C_PATTERN_CHAIN_QUANTUM},
    {"chain clear", C_PATTERN_CHAIN_CLEAR},
//    {"chain new", C_PATTERN_CHAIN_NEW},
    {"chain delete", C_PATTERN_CHAIN_DELETE},
    {"chain jump", C_PATTERN_CHAIN_JUMP},
    {"chain help", C_PATTERN_CHAIN_HELP},

    {"lock on", C_EDIT_CURSOR_LOCK},
    {"lock off", C_EDIT_CURSOR_UNLOCK},
    {"lock", C_EDIT_CURSOR_LOCK_STATUS},

    {"list", C_LIST},
    {"l", C_LIST},
    {"import", C_LIST_IMPORT},
    {"i", C_LIST_IMPORT},
    {"real time", C_LIST_RT},
    {"rt", C_LIST_RT},
    {"real time delete", C_LIST_RT_DELETE},
    {"rt delete", C_LIST_RT_DELETE},
    {"rt del", C_LIST_RT_DELETE},
    {"real time rate", C_LIST_RT_RATE},
    {"rt rate", C_LIST_RT_RATE},
    {"rt r", C_LIST_RT_RATE},
    {"real time quantum", C_LIST_RT_QUANTUM},
    {"rt quantum", C_LIST_RT_QUANTUM},
    {"rt q", C_LIST_RT_QUANTUM},
    {"real time phase", C_LIST_RT_START_PHASE},
    {"rt phase", C_LIST_RT_START_PHASE},
    {"rt p", C_LIST_RT_START_PHASE},
    {"real time echo", C_LIST_RT_ECHO},
    {"rt echo", C_LIST_RT_ECHO},
    {"rt e", C_LIST_RT_ECHO},

    {"status", C_STATUS},
    {"stat", C_STATUS},
    {"undo", C_UNDO_PATTERN_DELETE},

    {"store", C_STORE},
    {"store step", C_STORE_STEP},
    {"store gate", C_STORE_GATE},
    {"store vel", C_STORE_VELOCITY},
    {"store scale", C_STORE_SCALE},
    {"store all", C_STORE_ALL},
    {"store help", C_STORE_HELP},
    {"help store", C_STORE_HELP},
    {"load help", C_STORE_HELP},
    {"help load", C_STORE_HELP},
    {"load", C_LOAD},
    {"load step", C_LOAD_STEP},
    {"load gate", C_LOAD_GATE},
    {"load vel", C_LOAD_VELOCITY},
    {"load scale", C_LOAD_SCALE},
    {"load all", C_LOAD_ALL},

    {"use", C_USE},
    {"use globals", C_USE_GLOBAL_PLAYDATA},
    {"use g", C_USE_GLOBAL_PLAYDATA},
    {"use pattern", C_USE_PATTERN_PLAYDATA},
    {"use p", C_USE_PATTERN_PLAYDATA},
    {"use help", C_USE_HELP},
    {"help use", C_USE_HELP},

    {"misc", C_HELP_1},
    {"pattern", C_HELP_3},
    {"control", C_HELP_2}
};

command_t command_from_string(string commandName)
{
    command_t command;

#ifdef MA_BLUE
    for(auto i = commandName.begin(); i != commandName.end(); i++)
    {
        *i = tolower(*i);
    }

    if ( g_CommandList.count(commandName) == 1 )
        command = g_CommandList[commandName];
    else
        command = C_NONE;
#else
    try
    {
        transform(commandName.begin(), commandName.end(), commandName.begin(), ::tolower);
        if ( g_CommandList.count(commandName) > 0 )
            command = g_CommandList.at(commandName);
        else
            command = C_NONE;
    }
    catch ( out_of_range const & e )
    {
        command = C_NONE;
    }
    catch( ... )
    {
        command = C_NONE;
    }
#endif

    return command;
}

command_t command_from_string(vector<string> & tokens, uint32_t count)
{
    if ( count > tokens.size() )
        return C_NONE;

    string phrase = tokens[0];

    for ( uint32_t i = 1; i < count; i++ )
    {
        phrase += " ";
        phrase += tokens[i];
    }

    return command_from_string(phrase);

//    try
//    {
//        string commandName = tokens[0];
//
//        for ( int i = 1; i < count; i++ )
//        {
//            commandName += " ";
//            commandName += tokens[i];
//        }
//
//        command = command_from_string(commandName);
//    }
//    catch ( out_of_range const & e )
//    {
//        command = C_NONE;
//    }
//
//    return command;
}

void do_help(string topicName)
{
    command_t topic = command_from_string(topicName);

    switch ( topic )
    {
        case C_CUE :
            set_status(STAT_POS_2, "Set the next pattern to play.");
            break;

        case C_EDIT :
            set_status(STAT_POS_2, "Set focus for copy/paste and other operations.");
            break;

        case C_NEW_PATTERN :
            set_status(STAT_POS_2, "Create new (empty) pattern.");
            break;

        case C_COPY :
            set_status(STAT_POS_2, "Copy current pattern into a new entry at the end of the pattern list.");
            break;

        case C_DELETE :
            set_status(STAT_POS_2, "Delete pattern and reduce size of pattern list.");
            break;

        case C_CLEAR :
            set_status(STAT_POS_2, "Clear pattern but leave empty entry in pattern list.");
            break;

        case C_UNDO_PATTERN_DELETE :
            set_status(STAT_POS_2, "Restore deleted or cleared pattern to end of the pattern list.");
            break;

        case C_HELP_1 :
            set_status(STAT_POS_2, "Misc: exit/quit, status/stat, help, run, stop");
            break;

        case C_HELP_2 :
            set_status(STAT_POS_2, "Control: midi/channel/chan, step/note, transpose/tran, quantum/quan. gate");
            break;

        case C_HELP_3 :
            set_status(STAT_POS_2, "Pattern: play/cue, reset, autoreset, edit, new, copy, clear, delete/del, undo, list");
            break;

        case C_HELP_4 :
            set_status(STAT_POS_2, "");
            break;

        case C_NONE :
            if ( ItemMenu::MenuActive() )
                set_status(STAT_POS_2, ItemMenu::Help().c_str());
            else
                set_status(STAT_POS_2, "Topics: 'control', 'pattern', 'misc' or try a command name");
            break;
        default:
            break;
    }
}


bool do_command(string commandString, int directCommand)
{
    bool bResult = true;
    command_t command;
    int firstParameter = -1;

    // 'tokens' will either contain the entire command string, or just
    // the parameters for a direct command (from a menu).

    vector<string> tokens = split(commandString.c_str());;

    if ( directCommand != C_NONE )
    {
        command = (command_t) directCommand;
        if ( ! tokens.empty() )
            firstParameter = 0;
    }
    else
    {
        if ( tokens.empty() )
            return bResult;

        // Try for combinations first. Hopefully this makes the following switch
        // statement easier to read (though longer).

        for ( int i = tokens.size(); i >= 0; i-- )
        {
            if ( command = command_from_string(tokens, i), command != C_NONE )
            {
                if ( static_cast<unsigned>(i) < tokens.size() )
                    firstParameter = i;
                break;
            }
        }
    }

#ifndef MA_BLUE
    try
    {
#endif
        int iTemp;
        double fTemp;

        switch ( command )
        {

        case C_MENU:
            g_CommandMenu.Open(ItemMenu::CurrentPopUpMenuID());
            break;

        case C_RUN :
//            g_State.SetNewRunStatePending(true);
            if ( g_State.RunState() )
            {
                g_State.SetPatternReset(RESET_ALL);
                set_status(STAT_POS_2, "All patterns will be reset.");
            }
            else
            {
                g_State.SetNewRunStatePending(true);
                set_status(STAT_POS_2, "Starting ...");
            }
            break;

        case C_STOP :
            g_State.SetNewRunStatePending(false, 1);
            set_status(STAT_POS_2, "Stopping ...");
            break;

        case C_REC_TOGGLE:
            g_State.SetRecState(!g_State.RecState());
            set_status(STAT_POS_2, "Record: %s", g_State.RecState() ? "ON" : "Off");
            set_top_line();
            break;

        case C_HALT:
            g_State.SetRunState(false);
            break;

        case C_RESET_SCREEN:
            g_TextUI.ResetScreen();
            set_top_line();
            break;

        case C_CRASH_TEST:
            while (true)
                ;
            break;


       case C_SET_LABEL:
           if ( tokens.size() > 1 )
           {
               size_t pos = commandString.find(' ') + 1;
               g_PatternStore.CurrentEditPattern().SetLabel(commandString.substr(pos));
           }
           set_status(STAT_POS_2, "Pattern %s: %s",
               g_PatternStore.CurrentEditPatternID(),
               g_PatternStore.CurrentEditPattern().Label(50).c_str());
           break;

        case C_PAGE_ONE:
            g_TextUI.SetBigPanelPage(AnsiUI::one);
            break;
        case C_PAGE_TWO:
            g_TextUI.SetBigPanelPage(AnsiUI::two);
            break;
        case C_PAGE_THREE:
            g_TextUI.SetBigPanelPage(AnsiUI::three);
            break;
        case C_PAGE_HOLD:
            g_TextUI.ToggleBigPanelHold();
            break;


//        case C_CUE :
//            if ( tokens.size() < 2 )
//            {
//                set_status(STAT_POS_2, "Hint: play nn, where 'nn' is pattern number.");
//                break;
//            }
//#if defined(MA_BLUE)
//            iTemp = strtol(tokens[1].c_str(), NULL, 0);
//            if ( iTemp == 0 )
//            {
//               set_status(STAT_POS_2, "Invalid pattern number.");
//               break;
//            }
//#else
//            iTemp = stoi(tokens[1].c_str());
//#endif
//            iTemp -= 1;
//            if ( ! g_PatternStore.ValidPosition(iTemp) )
//            {
//               set_status(STAT_POS_2, "Requested pattern number out of range at the moment.");
//               break;
//            }
//            set_status(STAT_POS_2, "Cueing pattern: %s", tokens[1].c_str());
//            g_PatternStore.SetNewPatternPending(iTemp);
//            break;

        case C_CUE_CURRENT :
            if ( ! g_PatternStore.Empty() )
            {
                set_status(STAT_POS_2, "Cueing pattern: %s", g_PatternStore.CurrentEditPattern().ShortLabel());
                g_PatternStore.SetNewPatternPending(g_PatternStore.CurrentEditPattern().ShortLabelHash());
                g_PatternStore.SetRedraw();
            }
            break;

//        case C_EDIT :
//            if ( tokens.size() < 2 )
//            {
//                // No value specified, just open the menu.
//                g_PatternStore.SetFocus();
//                g_PatternStore.SetStatus();
//            }
//            else
//            {
//#if defined(MA_BLUE)
//                iTemp = strtol(tokens[1].c_str(), NULL, 0);
//                if ( iTemp == 0 )
//                {
//                   set_status(STAT_POS_2, "Invalid pattern number.");
//                   break;
//                }
//#else
//                iTemp = stoi(tokens[1].c_str());
//#endif
//                iTemp -= 1;
//                if ( ! g_PatternStore.ValidPosition(iTemp) )
//                {
//                    set_status(STAT_POS_2, "Requested pattern number out of range at the moment.");
//                    break;
//                }
//                set_status(STAT_POS_2, "Editing pattern: %s", tokens[1].c_str());
//                g_PatternStore.SetEditPos(iTemp);
////                update_pattern_status_panel();
//            }
//            break;

//        case C_EDIT_ITEM:
////            g_PatternStore.OpenCurrentItemMenu();
//            break;

//       case C_EDIT_CURSOR_LOCK:
//           g_PatternStore.SetEditFocusFollowsPlay(true);
//           set_status(STAT_POS_2, "Edit focus locked with playing pattern.");
////           update_pattern_status_panel();
//           break;
//       case C_EDIT_CURSOR_UNLOCK:
//           g_PatternStore.SetEditFocusFollowsPlay(false);
//           set_status(STAT_POS_2, "Edit focus unlocked.");
//           break;
//       case C_EDIT_CURSOR_LOCK_STATUS:
//           set_status(STAT_POS_2, "Edit focus %s.", g_PatternStore.EditFocusFollowsPlay() ? "locked" : "unlocked");
//           break;

       case C_NEW_PATTERN :
            g_PatternStore.AddEmptyPattern(tokens, firstParameter);
            // g_PatternStore.AddEmptyPattern(tokens.begin() + firstParameter, tokens.end());
            // g_PatternStore.SetStepValCurrentEditPattern(g_State.StepValue());
            set_status(STAT_POS_2, "Empty pattern added at position %i.", g_PatternStore.m_Patterns.size());
//            update_pattern_status_panel();
//            show_status_after_navigation();
//            update_big_panel();
            break;

       case C_COPY :
           if ( g_PatternStore.Empty() )
           {
               set_status(STAT_POS_2, "Nothing to copy.");
               break;
           }
           g_PatternStore.CopyCurrentPattern();
           set_status(STAT_POS_2, "Current pattern copied at position %i.", g_PatternStore.m_Patterns.size());
//           update_pattern_status_panel();
           break;

       case C_DELETE :
           if ( g_PatternStore.Empty() )
           {
               set_status(STAT_POS_2, "Nothing to delete.");
               break;
           }
           if ( tokens.size() >= 2 && tokens.at(1) == "all" )
           {
               g_PatternStore.DeleteAllPatterns();
               set_status(STAT_POS_2, "All patterns deleted. (\'Undo\' retrieves them one at a time.)");
           }
           else
           {
               g_PatternStore.DeleteCurrentPattern();
               set_status(STAT_POS_2, "Current pattern deleted. (\'Undo\' puts it at the end of the list.)");
           }
//           update_pattern_status_panel();
           break;

       case C_UNDO_PATTERN_DELETE :
           g_PatternStore.PopDeletedPattern();
           set_status(STAT_POS_2, "Pattern restored at position %i.", g_PatternStore.m_Patterns.size());
//           update_pattern_status_panel();
           break;

       case C_CLEAR :
           g_PatternStore.ClearCurrentPattern();
           // g_PatternStore.SetStepValCurrentEditPattern(g_State.StepValue());
           set_status(STAT_POS_2, "Current edit pattern cleared. (\'Undo\' restores it to the end of the list.)");
           break;

        case C_RESET :
            if ( tokens.size() >= 2 && tokens.at(1) == "all" )
            {
                g_State.SetPatternReset(RESET_ALL);
                set_status(STAT_POS_2, "All patterns will be reset.");
            }
//           else if ( g_PatternStore.EditPatternIsPlayPattern() )
//           {
//
//               g_State.SetPatternReset(RESET_CURRENT);
//               set_status(STAT_POS_2, "Current pattern will be reset.");
//           }
           else
           {
               // Reset the edit pattern immediately.
               g_PatternStore.ResetCurrentPattern();
               set_status(STAT_POS_2, "Edit pattern was reset.");
           }
            break;

        case C_RESET_BEAT:
            g_State.SetBeat(ceil(g_State.Beat()));
            break;

        case C_STEPVAL :
            if ( tokens.size() < 2 )
            {
                set_status(STAT_POS_2, "Hint: step n");
                break;
            }
#if defined(MA_BLUE)
            fTemp = strtod(tokens[1].c_str(), NULL);
#else
            fTemp = stod(tokens[1].c_str());
#endif
            if ( fTemp != 0 )
            {
               set_status(STAT_POS_2, "Setting %s Step Value: %s", g_PatternStore.UsePatternPlayData() ? "pattern" : "global", tokens[1].c_str());
               g_PatternStore.SetStepValue(fTemp);
            }
            else
            {
                set_status(STAT_POS_2, "Step Value cannot be zero.", tokens[1].c_str());
            }
            break;

        case C_GATELENGTH :
            if ( tokens.size() >= 2 )
            {
#if defined(MA_BLUE)
                fTemp = strtod(tokens[1].c_str(), NULL);
#else
                fTemp = stod(tokens[1].c_str());
#endif
                if ( fTemp >= 0 )
                {
                   set_status(STAT_POS_2, "Setting %s Gate Length: %s", g_PatternStore.UsePatternPlayData() ? "pattern" : "global", tokens[1].c_str());
                   g_PatternStore.SetGateLength(fTemp/100);
                }
                else
                {
                    set_status(STAT_POS_2, "Gate length cannot be negative.", tokens[1].c_str());
                }
            }
            else
            {
                set_status(STAT_POS_2, "Hint: gate h[old]|n[ormal]|n.n%");
                break;
            }
            break;
       case C_GATE_HOLD:
           g_PatternStore.SetGateHold(true);
           break;
       case C_GATE_NORMAL:
           g_PatternStore.SetGateHold(false);
           break;

        case C_VELOCITY :
            if ( tokens.size() < 2 )
            {
                set_status(STAT_POS_2, "Hint: vel[ocity] n");
                break;
            }
#if defined(MA_BLUE)
            iTemp = strtol(tokens[1].c_str(), NULL, 0);
#else
            iTemp = stoi(tokens[1].c_str());
#endif
            if ( iTemp > 0 && iTemp <= 127 )
            {
               set_status(STAT_POS_2, "Setting %s velocity: %s", g_PatternStore.UsePatternPlayData() ? "pattern" : "global", tokens[1].c_str());
               g_PatternStore.SetNoteVelocity(iTemp);
            }
            else
            {
                set_status(STAT_POS_2, "Velocity must be in the range 1 .. 127.", tokens[1].c_str());
            }
            break;

        case C_TRANSPOSE :
            if ( tokens.size() < 2 )
            {
                set_status(STAT_POS_2, "Hint: trans[pose] n [now]");
                break;
            }
#if defined(MA_BLUE)
            iTemp = strtol(tokens[1].c_str(), NULL, 0);
#else
            iTemp = stoi(tokens[1].c_str());
#endif
            if ( tokens.size() >= 3 && tokens[2] == "now")
            {
                set_status(STAT_POS_2, "Transpose value set.");
                g_PatternStore.TranslateTableForEdit().SetTranspose(iTemp);
                set_top_line();
            }
            else
            {
                set_status(STAT_POS_2, "Setting transpose value: %s", tokens[1].c_str());
                g_PatternStore.TranslateTableForEdit().SetNewTransposePending(iTemp);
            }
            break;

        case C_FEEL:
            g_PatternStore.FeelMapForEdit().SetStatus();
//            show_status_after_navigation();
            break;
        case C_FEEL_HELP:
            set_status(STAT_POS_2, "feel new[list]|on|off|add|remove|respace|bypass");
            break;
        case C_FEEL_ON:
            g_PatternStore.FeelMapForEdit().SetActive(true);
//            show_status_after_navigation();
            break;
        case C_FEEL_OFF:
            g_PatternStore.FeelMapForEdit().SetActive(false);
//            show_status_after_navigation();
            break;
        case C_FEEL_NEW:
            g_PatternStore.FeelMapForEdit().New(tokens);
//            show_status_after_navigation();
            break;
        case C_FEEL_ADD:
            g_PatternStore.FeelMapForEdit().Add();
//            show_status_after_navigation();
            break;
        case C_FEEL_REMOVE:
            g_PatternStore.FeelMapForEdit().Remove();
//            show_status_after_navigation();
            break;
        case C_FEEL_RESPACE:
            g_PatternStore.FeelMapForEdit().Respace();
//            show_status_after_navigation();
            break;

        case C_SCALE:
            g_PatternStore.TranslateTableForEdit().SetStatus();    // This automatically sets focus.
//            show_status_after_navigation();
            break;
        case C_SCALE_FROM_LIST:
//            g_PatternStore.TranslateTableForEdit().SetScale(g_PatternStore.CurrentEditStepList());
            show_translation_map_status();
            break;
        case C_SCALE_SHOW:
            show_translation_map_status();
            break;
        case C_SCALE_CONTROLS:
            set_status(STAT_POS_2, "Ctl: Tonal/Chromatic, Sh: Scale/Root, C+S: Accidentals/Premap");
            break;
        case C_SCALE_HELP:
            set_status(STAT_POS_2, "scale show|controls|[name|1..%i]|from list", static_cast<int>(scroll_top));
            break;

        case C_SETROOT :
           if ( tokens.size() < 2 || ! g_PatternStore.TranslateTableForEdit().SetRoot(tokens[1]) )
           {
               set_status(STAT_POS_2, "Hint: root C, C#, Eb, C5, F#6, etc.");
               break;
           }
           else
               show_translation_status();
            break;

        case C_MIDI:
            if ( tokens.size() >= 2 )
            {
                // Expect a number and use it to set midi channel.

#if defined(MA_BLUE)
                int iTemp = strtol(tokens[1].c_str(), NULL, 0);
#else
                int iTemp = stoi(tokens[1].c_str());
#endif
                if ( iTemp >= 1 && iTemp <= 16)
                {
                    set_status(STAT_POS_2, "Set midi channel: %s", tokens[1].c_str());
                    g_Sequencer.SetMidiChannel(iTemp - 1);
                    set_top_line();
                }
            }
            else
            {
                set_status(STAT_POS_2, "Hint: chan[nel]|mi[di] q[uick]|f[ull]|n (1..16)");
            }
            break;
        case C_MIDI_REAL_TIME:
            g_ListBuilder.SetMidiInputMode(MIDI_INPUT_REAL_TIME);
            set_status(STAT_POS_2, "Midi Input set to REAL TIME.");
            set_top_line();
            break;
        case C_MIDI_STEP:
            g_ListBuilder.SetMidiInputMode(MIDI_INPUT_STEP);
            set_status(STAT_POS_2, "Midi Input set to STEP mode.");
            set_top_line();
            break;
        case C_MIDI_QUICK:
            g_ListBuilder.SetMidiInputMode(MIDI_INPUT_QUICK);
            set_status(STAT_POS_2, "Midi Input set to QUICK mode.");
            set_top_line();
            break;
        case C_MIDI_CHORD:
            g_ListBuilder.SetMidiInputMode(MIDI_INPUT_CHORD);
            set_status(STAT_POS_2, "Midi Input set to CHORD mode.");
            set_top_line();
            break;
//        case C_MIDI_OFF:
//            g_ListBuilder.SetMidiInputMode(MIDI_INPUT_OFF);
//            set_status(STAT_POS_2, "Midi Input OFF.");
//            set_top_line();
//            break;

        case C_QUANTUM :
            if ( tokens.size() < 2 )
            {
                set_status(STAT_POS_2, "Hint: quan[tum] n.nn");
                break;
            }
#if defined(MA_BLUE)
            fTemp = strtod(tokens[1].c_str(), NULL);
#else
            fTemp = stod(tokens[1].c_str());
#endif
            if ( fTemp > 0 )
            {
                set_status(STAT_POS_2, "Setting new Quantum: %s", tokens[1].c_str());
                g_State.SetNewQuantumPending(fTemp);
                // newQuantum = fTemp;
            }
            break;

#if defined(MA_BLUE)
        case C_TEMPO :
            if ( tokens.size() < 2 )
            {
                set_status(STAT_POS_2, "Hint: te[mpo] n.nn");
                break;
            }
            fTemp = strtod(tokens[1].c_str(), NULL);
            if ( fTemp > 0 )
            {
                g_State.SetTempo(fTemp);
                set_status(STAT_POS_2, "New Tempo: %s", tokens[1].c_str());
                set_top_line();
            }
            break;
#endif

        case C_SET_RESETONPATTERNCHANGE : // Auto-reset
            if ( tokens.size() < 2 )
            {
                set_status(STAT_POS_2, "Hint: autoreset on|off");
                break;
            }
           if ( tokens[1] == "on")
               g_PatternStore.SetResetOnPatternChange(true);
           else if (tokens[1] == "off")
               g_PatternStore.SetResetOnPatternChange(false);
           else
           {
               set_status(STAT_POS_2, "Autoreset not changed.");
               break;
           }
            break;

        case C_PATTERN_CHAIN_NEW:
        case C_PATTERN_CHAIN_COPY:
        case C_PATTERN_CHAIN_MOVE_UP:
        case C_PATTERN_CHAIN_MOVE_DOWN:
        case C_PATTERN_CHAIN_DELETE:
            set_status(STAT_POS_2, g_PatternStore.PatternChainManager(command).c_str());
            break;

//        case C_PATTERN_CHAIN :
//           if ( tokens.size() >= 2 )
//               g_PatternStore.UpdatePatternChainFromSimpleString(commandString);
//           g_PatternStore.PatternChainForEdit().SetStatus();
//           g_PatternStore.PatternChainForEdit().SetFocus();
//            break;
//        case C_PATTERN_CHAIN_OFF:
//           g_PatternStore.SetPatternChainMode(PatternChain::off);
//            break;
//        case C_PATTERN_CHAIN_NATURAL:
//           g_PatternStore.SetPatternChainMode(PatternChain::natural);
//            break;
//        case C_PATTERN_CHAIN_QUANTUM:
//           g_PatternStore.SetPatternChainMode(PatternChain::quantum);
//            break;
//        case C_PATTERN_CHAIN_CLEAR:
//           g_PatternStore.PatternChainForEdit().Clear();
//            break;
//        case C_PATTERN_CHAIN_NEW:
//           g_PatternStore.PatternChainForEdit().New();
//            break;
//        case C_PATTERN_CHAIN_DELETE:
//           g_PatternStore.PatternChainForEdit().Delete();
//           g_PatternStore.PatternChainForEdit().SetStatus();
////            show_status_after_navigation();
//            break;
//        case C_PATTERN_CHAIN_JUMP:
//            break;
//        case C_PATTERN_CHAIN_HELP:
//            set_status(STAT_POS_2, "chain off|natural|quantum|jump|new|delete");
//            break;

        case C_LOAD:
        case C_STORE:
        case C_STORE_HELP:
            set_status(STAT_POS_2, "store|load step|scale|gate|vel|all");
            break;

        case C_STORE_STEP:
           g_PatternStore.StorePatternPlayData(PLAY_DATA_STEP);
           set_status(STAT_POS_2, "Step Value stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_GATE:
            break;
        case C_STORE_VELOCITY:
           g_PatternStore.StorePatternPlayData(PLAY_DATA_VELO);
           set_status(STAT_POS_2, "Velocity stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_SCALE:
           g_PatternStore.StorePatternPlayData(PLAY_DATA_SCALE);
           set_status(STAT_POS_2, "Scale settings stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_ALL:
           g_PatternStore.StorePatternPlayData();
            set_status(STAT_POS_2, "All play data stored to current edit pattern.");
            break;

        case C_LOAD_STEP:
           set_status(STAT_POS_2, "Pattern %i: %s",
               g_PatternStore.CurrentEditPatternID(),
               g_PatternStore.LoadPatternPlayData(PLAY_DATA_STEP).c_str());
            break;
        case C_LOAD_GATE:
           set_status(STAT_POS_2, "Pattern %i: %s",
               g_PatternStore.CurrentEditPatternID(),
               g_PatternStore.LoadPatternPlayData(PLAY_DATA_GATE).c_str());
            break;
        case C_LOAD_VELOCITY:
            set_status(STAT_POS_2, "Pattern %i: %s",
               g_PatternStore.CurrentEditPatternID(),
               g_PatternStore.LoadPatternPlayData(PLAY_DATA_VELO).c_str());
            break;
        case C_LOAD_SCALE:
            set_status(STAT_POS_2, "Pattern %i: %s",
               g_PatternStore.CurrentEditPatternID(),
               g_PatternStore.LoadPatternPlayData(PLAY_DATA_SCALE).c_str());
            break;
        case C_LOAD_ALL:
            set_status(STAT_POS_2, "Pattern %i: %s",
               g_PatternStore.CurrentEditPatternID(),
               g_PatternStore.LoadPatternPlayData().c_str());
            break;

        case C_USE:
            set_status(STAT_POS_2, g_PatternStore.ShowPatternPlayData().c_str());
            break;
        case C_USE_GLOBAL_PLAYDATA:
           g_PatternStore.SetUsePatternPlayData(false);
           set_status(STAT_POS_2, g_PatternStore.ShowPatternPlayData().c_str());
            break;
        case C_USE_PATTERN_PLAYDATA:
           g_PatternStore.SetUsePatternPlayData(true);
           set_status(STAT_POS_2, g_PatternStore.ShowPatternPlayData().c_str());
            break;
        case C_USE_HELP:
            set_status(STAT_POS_2, "use g[lobal]|p[attern]");
            break;

        case C_NEW_STEP_GROUP:
        case C_NEW_RT_GROUP:
        case C_COPY_GROUP:
        case C_DELETE_GROUP:
            if ( g_PatternStore.Empty() )
            {
                set_status(STAT_POS_2, "You need to create an empty pattern first.");
                break;
            }
            switch ( command )
            {
                case C_NEW_STEP_GROUP:
                    g_PatternStore.CurrentEditPattern().NewListGroup(ListGroup::lgtype_step);
                    break;
                case C_NEW_RT_GROUP:
                    g_PatternStore.CurrentEditPattern().NewListGroup(ListGroup::lgtype_realtime);
                    break;
                case C_COPY_GROUP:
                    g_PatternStore.CurrentEditPattern().CopyCurrentListGroup();
                    break;
                case C_DELETE_GROUP:
                    g_PatternStore.CurrentEditPattern().DeleteCurrentListGroup();
                    break;
                default:
                    break;
            }
//            update_big_panel();
            break;

        case C_GROUP_RUN:
            g_PatternStore.CurrentEditPattern().RunCurrentListGroup();
            break;
        case C_GROUP_STOP:
            g_PatternStore.CurrentEditPattern().StopCurrentListGroup();
            break;
        case C_PATTERN_RUN_ALL:
            g_PatternStore.CurrentEditPattern().RunAllListGroups(g_State.NextPhaseZero());
            break;
        case C_PATTERN_STOP_ALL:
            g_PatternStore.CurrentEditPattern().StopAllListGroups();
            break;
        case C_GROUP_RS_TOGGLE:
            break;

        case C_LIST:
            if ( tokens.size() < 2 )
            {
                set_status(STAT_POS_2, "Hint: list new|delete|n [clear|: n1, n2 ,...]");
                break;
            }
            set_status(STAT_POS_2, "%.60s", g_PatternStore.ListManager(commandString, tokens).c_str());
//            update_big_panel();
            break;

        case C_LIST_NEW:
        case C_LIST_COPY:
        case C_LIST_DELETE:
        case C_LIST_MOVE_UP:
        case C_LIST_MOVE_DOWN:
        case C_STEP_INSERT_LEFT:
        case C_STEP_INSERT_RIGHT:
        case C_STEP_COPY_LEFT:
        case C_STEP_COPY_RIGHT:
        case C_STEP_DELETE:
        case C_CLUSTER_INSERT_LEFT:
        case C_CLUSTER_INSERT_RIGHT:
        case C_CLUSTER_COPY_LEFT:
        case C_CLUSTER_COPY_RIGHT:
        case C_CLUSTER_DELETE:
            set_status(STAT_POS_2, "%.60s", g_PatternStore.CurrentEditPattern().StepListManager(command).c_str());
            break;

#ifndef MA_BLUE
        case C_LIST_IMPORT:
            if ( tokens.size() < 2 )
            {
                set_status(STAT_POS_2, "Hint: import filename[.mid]");
                break;
            }
            g_PatternStore.UpdatePatternFromMidiFile(commandString);
            set_status(STAT_POS_2, "File imported to Real Time list.");
//            update_big_panel();
            break;
#endif

       case C_LIST_RT:
// TODO:LG
//           g_PatternStore.CurrentEditRealTimeList().SetStatus();
//           g_PatternStore.CurrentEditRealTimeList().SetFocus();
//           show_status_after_navigation();
           break;

       case C_LIST_RT_DELETE:
// TODO:LG
//           g_PatternStore.DeleteCurrentRealTimeList();
//           g_PatternStore.SetFocus();
//           g_PatternStore.SetStatus();
//           show_status_after_navigation();
           break;

       case C_LIST_RT_RATE:
// TODO:LG
//           if ( firstParameter > 0 )
//           {
//               g_PatternStore.CurrentEditPattern().SetRealTimeMultipliers(tokens.begin() + firstParameter, tokens.end());
//               set_status(STAT_POS_2, "Pattern %i: Real Time playback rates set.", g_PatternStore.CurrentEditPatternID());
//               show_status_after_navigation();
//           }
//           else
//               set_status(STAT_POS_2, "help: real time rate \"rate\" [\"step\"]");
           break;

       case C_LIST_RT_QUANTUM:
// TODO:LG
//           if ( firstParameter > 0 )
//           {
//               g_PatternStore.CurrentEditPattern().SetRealTimeQuantum(tokens.at(firstParameter));
//               set_status(STAT_POS_2, "Pattern %i: loop length (quantum) set for Real Timelists.", g_PatternStore.CurrentEditPatternID());
//               show_status_after_navigation();
//           }
//           else
//               set_status(STAT_POS_2, "help: real time quantum [q]");
           break;

       case C_LIST_RT_START_PHASE:
// TODO:LG
//           if ( firstParameter > 0 )
//           {
//               g_PatternStore.CurrentEditPattern().SetRealTimeStartPhase(tokens.at(firstParameter));
//               set_status(STAT_POS_2, "Pattern %i: start phase set for Real Time lists.", g_PatternStore.CurrentEditPatternID());
//               show_status_after_navigation();
//           }
//           else
//               set_status(STAT_POS_2, "help: real time phase [0..99%]");
           break;

       case C_LIST_RT_ECHO:
// TODO:LG
//           if ( firstParameter > 0 )
//           {
//               g_PatternStore.CurrentEditPattern().StartRealTimeEcho(tokens.begin() + firstParameter, tokens.end());
//               set_status(STAT_POS_2, "Pattern %i: started echo on current Real Time list.", g_PatternStore.CurrentEditPatternID());
//               show_status_after_navigation();
//           }
//           else
//               set_status(STAT_POS_2, "help: real time echo inc[rement] n.nn int[erval] n t[arget] nn%");
           break;

        case C_HELP :
            if ( tokens.size() < 2 )
                do_help("");
            else
                do_help(tokens.at(1));
            break;

        case C_STATUS :
            show_status();
            break;

       case C_TRIGS:
//           if ( firstParameter > 0 )
//           {
////                g_PatternStore.CurrentEditPattern().PatternTrigList().FromSimpleString(commandString);
//               g_PatternStore.CurrentEditPattern().PatternTrigList().FromSimpleString(tokens.begin() + firstParameter, tokens.end());
//           }
//           g_PatternStore.CurrentEditPattern().PatternTrigList().SetStatus();
//           g_PatternStore.CurrentEditPattern().PatternTrigList().SetFocus();
//           show_status_after_navigation();
           break;

       case C_TRIGS_ARPEGGIO:
//           if ( firstParameter > 0 )
//           {
//               g_PatternStore.CurrentEditPattern().PatternTrigList().AddArpeggio(tokens.begin() + firstParameter, tokens.end());
//           }
//           g_PatternStore.CurrentEditPattern().PatternTrigList().SetStatus();
//           g_PatternStore.CurrentEditPattern().PatternTrigList().SetFocus();
//           show_status_after_navigation();
           break;

#if 0
        case C_TRIGS:
            if ( tokens.size() > 1 )
            {
                g_PatternStore.CurrentPlayPattern().PatternTrigList().FromSimpleString(commandString);
            }
            g_PatternStore.CurrentPlayPattern().PatternTrigList().SetStatus();
            g_PatternStore.CurrentPlayPattern().PatternTrigList().SetFocus();
            show_status_after_navigation();
            break;

        case C_TRIGS_ARPEGGIO:
            g_PatternStore.CurrentPlayPattern().PatternTrigList().AddArpeggio(commandString);
            g_PatternStore.CurrentPlayPattern().PatternTrigList().SetStatus();
            g_PatternStore.CurrentPlayPattern().PatternTrigList().SetFocus();
            show_status_after_navigation();
            break;
#endif

        case C_EXIT :
            bResult = false;
            break;

        case C_NONE :
#ifdef MA_BLUE
            iTemp = strtol(tokens[0].c_str(), NULL, 0);
            if ( iTemp == 0 )
            {
                set_status(STAT_POS_2, "Phrase not recognised.");
                break;
            }
#else
//            iTemp = stoi(tokens[0]);
            iTemp = strtol(tokens[0].c_str(), NULL, 0);
#endif
//            if ( iTemp > 0 )
//                // Function returns a format string. Too obfuscated?
//                set_status(STAT_POS_2, g_PatternStore.SetNewPatternOrJump(iTemp - 1).c_str(), iTemp);
//            else
            set_status(STAT_POS_2, "Phrase not recognised.");
            break;
        default :
            break;
        }
#ifndef MA_BLUE
    }
    catch (invalid_argument e)
    {
        set_status(STAT_POS_2, "Phrase not recognised. (%s)", e.what());
    }
    catch (out_of_range e)
    {
        set_status(STAT_POS_2, "Something went out of range ...");
    }
    catch ( string s )
    {
        set_status(STAT_POS_2, s.c_str());
    }
#endif

    // Keeping track of all the places I might need to do this
    // is nearly impossible, so might as well just do it every
    // time.

//    update_pattern_list_panels(true);
    commandString = "";
    return bResult;
}

#ifndef MA_BLUE
void do_command_line(int argc, char *argv[])
{

    // Set console window title. (This doesn't work after NCurses starts.)

    string appTitle;
    if ( argc > 1 )
        for ( int i = 1; i < argc; i++ )
        {
            if ( strstr(argv[i], "-midi=") == argv[i] )
            {
                int iTemp = strtol(argv[i] + 6, NULL, 10);
                if ( iTemp >= 1 && iTemp <= 16)
                    g_Sequencer.SetMidiChannel(iTemp - 1);
            }
            else if ( strstr(argv[i], "-transpose=") == argv[i] )
            {
                g_PatternStore.TranslateTableForEdit().SetTranspose(strtol(argv[i] + 11, NULL, 10));
            }
            else if ( strstr(argv[i], "-tempo=") == argv[i] )
            {
                // TODO: Involves updating the timeline, which I haven't tried yet.
            }
            else if ( strstr(argv[i], "-step=") == argv[i] )
            {
                g_PatternStore.SetStepValue(strtof(argv[i] + 6, NULL));

            }
            else
            {
                if ( !appTitle.empty() )
                    appTitle += ' ';
                appTitle += argv[i];
            }
        }

    if ( appTitle.empty() )
        appTitle += "MultiArp";

    printf("%c]0;%s%c", '\033', appTitle.c_str(), '\007');
    g_Sequencer.SetClientName(appTitle.c_str());


}
#endif

enum global_element_names_t
{
     global_heading,
     global_name_midi_channel,
     global_name_link_quantum,
     num_global_element_names
};


#if defined(MA_BLUE)
unordered_map <int, const char *> global_element_names =
#else
unordered_map <global_element_names_t, const char *> global_element_names =
#endif
{
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

    snprintf(buff, 200, "%s %i\n", global_element_names.at(global_name_midi_channel), g_Sequencer.MidiChannel() + 1);
    result += buff;
    snprintf(buff, 200, "%s %.2f\n", global_element_names.at(global_name_link_quantum), g_State.Quantum());
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
#ifndef MA_BLUE
        try
        {
#endif
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

#ifdef MA_BLUE
                switch (e)
                {
                case global_name_midi_channel:
                    g_Sequencer.SetMidiChannel(strtol(token.c_str(), NULL, 0) - 1);
                    break;
                case global_name_link_quantum:
                    g_State.SetNewQuantumPending(strtod(token.c_str(), NULL));
                    break;
                default:
                    break;
                }
#else
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
#endif
            }
#ifndef MA_BLUE
        }
        catch (invalid_argument e)
        {
        }
        catch (string error )
        {
            errors.push_back(error);
        }
#endif
    }

#ifndef MA_BLUE
    if ( !errors.empty() )
        throw string("Pattern parse error: At least one row contained errors.");
#endif

}

//bool handle_key_input(ItemMenu::key_type_t curKey, xcb_keysym_t sym)

map<int, int> g_KeyCommandMap =
{
    {'/', BaseUI::key_cmd_menu},
    {BaseUI::key_return, BaseUI::key_cmd_enter},
    {BaseUI::key_backspace, BaseUI::key_cmd_back},
    {BaseUI::key_left, BaseUI::key_cmd_left},
    {BaseUI::key_right, BaseUI::key_cmd_right},
    {BaseUI::key_up, BaseUI::key_cmd_up},
    {BaseUI::key_down, BaseUI::key_cmd_down},
    {BaseUI::key_shift_left, BaseUI::key_cmd_insert_left},
    {BaseUI::key_shift_right, BaseUI::key_cmd_insert_right},
    {BaseUI::key_shift_delete, BaseUI::key_cmd_delete},
    {BaseUI::key_ctrl_left, BaseUI::key_cmd_copy_left},
    {BaseUI::key_ctrl_right, BaseUI::key_cmd_copy_right},
    {BaseUI::key_alt_left, BaseUI::key_cmd_move_left},
    {BaseUI::key_alt_right, BaseUI::key_cmd_move_right},
    {BaseUI::key_alt_up, BaseUI::key_cmd_move_up},
    {BaseUI::key_alt_down, BaseUI::key_cmd_move_down},
    {BaseUI::key_ctrl_shift_left, BaseUI::key_cmd_shift_left},
    {BaseUI::key_ctrl_shift_right, BaseUI::key_cmd_shift_right},
    {BaseUI::key_ctrl_delete, BaseUI::key_cmd_undo},
    {BaseUI::key_shift_up, BaseUI::key_cmd_inc},
    {BaseUI::key_shift_down, BaseUI::key_cmd_dec},
    {BaseUI::key_ctrl_up, BaseUI::key_cmd_inc_2},
    {BaseUI::key_ctrl_down, BaseUI::key_cmd_dec_2},
};

bool handle_key_input(BaseUI::key_command_t key)
{
    static string commandString;

#if 1
        g_TextUI.SendSaveCursor();
        if ( key < BaseUI::key_none)
            g_TextUI.FWriteXY(40, 25, "Key: '%c' (%i)", key, key);
        else
            g_TextUI.FWriteXY(40, 25, "Key: %s (%i)", BaseUI::KeyName(key), key);
        g_TextUI.ClearEOL();
        g_TextUI.SendRestoreCursor();
#endif

    // Remap key strokes into generic ItemMenu commands.

    auto pos = g_KeyCommandMap.find(key);
    if ( pos != g_KeyCommandMap.end() )
        key = static_cast<BaseUI::key_command_t>(pos->second);

    // If command menu is open it eats all keys.

    if ( g_CommandMenu.HandleKey(key) )
    {
        return true;
    }

    // Look for menu key (currently '/') to open command menu, but only if
    // we haven't already started a command string.

    if ( commandString.empty() && key == BaseUI::key_cmd_menu )
    {
        do_command("", C_MENU);
        return true;
    }

    bool result = true;
    bool keyUsed = false;

    switch (key)
    {
#if !defined(MA_BLUE) || defined(MA_BLUE_PC)
    case BaseUI::key_ctrl_a: // 0xE6: // Ctrl-A
        copy_clipboard(globals_to_string() + g_PatternStore.ToString());
        set_status(STAT_POS_2, "All Data copied to clipboard ...");
        set_status(COMMAND_HOME, "");
        keyUsed = true;
        break;

    case BaseUI::key_ctrl_c: // 0xA2:  // Ctrl-C, Copy
        copy_clipboard(g_PatternStore.EditPatternToString());
        set_status(STAT_POS_2, "Edit Pattern copied to clipboard ...");
        set_status(COMMAND_HOME, "");
        keyUsed = true;
        break;

    case BaseUI::key_ctrl_v: // 0xAD2: // Ctrl-V, Paste
        set_status(COMMAND_HOME, "");
#ifndef MA_BLUE
        try
        {
#endif
            int created, updates;
            load_from_string(get_clipboard(), created, updates);
            set_status(STAT_POS_2, "Paste: %i updates, %i new patterns created.", updates, created);
#ifndef MA_BLUE
        }
        catch (string errorMessage)
        {
            set_status(STAT_POS_2, "%s", errorMessage.c_str());
        }
#endif
        keyUsed = true;
        break;
#endif

    case BaseUI::key_cmd_enter: // Enter
        if ( !commandString.empty() )
        {
            result = do_command(commandString);
            commandString.clear();
            keyUsed = true;
        }
        else if ( g_ListBuilder.HandleKeybInput(key) )
        {
            if ( g_ListBuilder.RealTimeRecord() )
                g_PatternStore.UpdatePattern(g_ListBuilder.RealTimeList(), g_State.Quantum());
            else
                g_PatternStore.UpdatePattern(g_ListBuilder.CurrentList());
            g_ListBuilder.Clear();
            set_status(STAT_POS_2, "");
            keyUsed = true;
        }
        if ( keyUsed )
        {
            place_cursor(COMMAND_HOME);
            set_status(COMMAND_HOME, "");
        }
//        else
//            key = BaseUI::key_cmd_enter;
        break;

    case BaseUI::key_cmd_back: // XK_BackSpace:
        if ( !commandString.empty() )
        {
            commandString.pop_back();
            keyUsed = true;
        }
        else if ( g_ListBuilder.HandleKeybInput(key) )
        {
            show_listbuilder_status();
            keyUsed = true;
        }
        if ( keyUsed )
        {
            place_cursor(COMMAND_HOME + commandString.size());
            set_status(COMMAND_HOME, commandString.c_str());
        }
//        else
//            key = BaseUI::key_cmd_back;
        break;

    case BaseUI::key_space: // XK_space: // Space bar.
        if ( !commandString.empty() )
        {
            commandString += ' ';
            place_cursor(COMMAND_HOME + commandString.size());
            set_status(COMMAND_HOME, commandString.c_str());
            keyUsed = true;
        }
        else if ( g_ListBuilder.HandleKeybInput(key) )
        {
            show_listbuilder_status();
            keyUsed = true;
        }
        break;

#ifndef MA_BLUE
    case BaseUI::key_tab: // XK_Tab:
        place_cursor(COMMAND_HOME + commandString.size());
        g_TextUI.NextBigPanelPage(1);
        keyUsed = true;
        break;

    case BaseUI::key_shift_tab: // XK_ISO_Left_Tab:   // Shift-Tab
        g_TextUI.NextBigPanelPage(-1);
        keyUsed = true;
        break;
#endif

    default:

        if ( key > 32 && key < 127 )
        {
            commandString += key;
            place_cursor(COMMAND_HOME + commandString.size());
            set_status(COMMAND_HOME, commandString.c_str());
            keyUsed = true;
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

    if ( keyUsed )
        return result;

//    // Remap key strokes into generic ItemMenu commands.
//
//    auto pos = g_KeyCommandMap.find(key);
//    if ( pos != g_KeyCommandMap.end() )
//        key = static_cast<BaseUI::key_command_t>(pos->second);

    // Send key strokes off to active ItemMenu.

    if ( ItemMenu::RouteKey(key) )
        return true;
    else
        return result;  // If result is set to false by do_command(), the program quits.
}


