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

#include "maCommand.h"

#include<algorithm>
#include<cstring>
#include<cmath>
#include<unordered_map>

#include "maAlsaSequencer.h"
#include "maAlsaSequencerQueue.h"
#include "maListBuilder.h"
#include "maPatternStore.h"
#include "maScreen.h"
#include "maState.h"
#include "maTranslateTable.h"
#include "maUtility.h"

using namespace std;

// Global objects.

extern AlsaSequencer g_Sequencer;
extern CursorKeys g_CursorKeys;
extern State g_State;
extern PatternStore g_PatternStore;
extern Display g_Display;
extern ListBuilder g_ListBuilder;
//extern TranslateTable * pTranslateTable;

enum command_t
{
    C_NONE,
    C_EXIT,
    C_HELP,

    C_RUN,
    C_STOP,
    C_HALT,
    C_RESET,
    C_SET_RESETONPATTERNCHANGE,
    C_RESET_BEAT,

    C_SET_LABEL,

    C_TRIGS, // Open the trig menu on current pattern or enter a list.
    C_TRIGS_ARPEGGIO,

    C_STEPVAL,
    C_QUANTUM,
    C_GATELENGTH,
    C_GATE_HOLD,
    C_GATE_NORMAL,
    C_VELOCITY,
    C_TRANSPOSE,

    C_SCALE,
    C_SCALE_FROM_LIST,
    C_SCALE_SHOW,
    C_SCALE_HELP,
    C_SCALE_CONTROLS,
    C_SETROOT,

    C_PAGE_ONE,
    C_PAGE_TWO,
    C_PAGE_THREE,
    C_PAGE_HOLD,

    C_FEEL,
    C_FEEL_HELP,
    C_FEEL_NEW,
    C_FEEL_ADD,
    C_FEEL_REMOVE,
    C_FEEL_RESPACE,
    C_FEEL_ON,
    C_FEEL_OFF,

    C_MIDI,
    C_MIDI_REAL_TIME,
    C_MIDI_STEP,
    C_MIDI_QUICK,
    C_MIDI_OFF,

    C_CUE,              // Set the next pattern to play
    C_EDIT,             // Set focus for copy/paste
    C_EDIT_CURSOR_LOCK,
    C_EDIT_CURSOR_UNLOCK,
    C_NEW,              // Create new (empty) pattern.
    C_COPY,             // Copy current pattern into a new entry at the end of the pattern list.
    C_DELETE,           // Delete pattern and reduce size of pattern list.
    C_CLEAR,            // Clear pattern but leave empty entry in pattern list.
    C_UNDO,             // Restore deleted or cleared pattern to end of the pattern list.
    C_STATUS,           // Display current stats.

    C_PATTERN_CHAIN,    // Set pattern chain mode.
    C_PATTERN_CHAIN_OFF,
    C_PATTERN_CHAIN_NATURAL,
    C_PATTERN_CHAIN_QUANTUM,
    C_PATTERN_CHAIN_CLEAR,
    C_PATTERN_CHAIN_NEW,
    C_PATTERN_CHAIN_DELETE,
    C_PATTERN_CHAIN_JUMP,
    C_PATTERN_CHAIN_HELP,

    C_STORE,
    C_STORE_STEP,
    C_STORE_GATE,
    C_STORE_VELOCITY,
    C_STORE_SCALE,
    C_STORE_ALL,
    C_STORE_HELP,
    C_LOAD,
    C_LOAD_STEP,
    C_LOAD_GATE,
    C_LOAD_VELOCITY,
    C_LOAD_SCALE,
    C_LOAD_ALL,

    C_USE,
    C_USE_GLOBAL_PLAYDATA,
    C_USE_PATTERN_PLAYDATA,
    C_USE_HELP,

    C_LIST,             // Note list commands.
    C_LIST_IMPORT,      // Import from midi file.

    C_LIST_RT,          // Real time list commands.
    C_LIST_RT_DELETE,

    C_HELP_1,
    C_HELP_2,
    C_HELP_3,
    C_HELP_4,

    C_NUM_COMMANDS
};


unordered_map<string, command_t> gCommandList =
{
    {"exit", C_EXIT},
    {"quit", C_EXIT},
    {"help", C_HELP},

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
    {"midi off", C_MIDI_OFF},
    {"m off", C_MIDI_OFF},

    {"edit", C_EDIT},
    {"e", C_EDIT},
    {"cue", C_CUE},
    {"play", C_CUE},
    {"new", C_NEW},
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
    {"chain new", C_PATTERN_CHAIN_NEW},
    {"chain delete", C_PATTERN_CHAIN_DELETE},
    {"chain jump", C_PATTERN_CHAIN_JUMP},
    {"chain help", C_PATTERN_CHAIN_HELP},

    {"lock", C_EDIT_CURSOR_LOCK},
    {"unlock", C_EDIT_CURSOR_UNLOCK},

    {"list", C_LIST},
    {"l", C_LIST},
    {"import", C_LIST_IMPORT},
    {"i", C_LIST_IMPORT},
    {"real time", C_LIST_RT},
    {"rt", C_LIST_RT},
    {"real time delete", C_LIST_RT_DELETE},
    {"rt delete", C_LIST_RT_DELETE},
    {"status", C_STATUS},
    {"stat", C_STATUS},
    {"undo", C_UNDO},

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

    try
    {
        transform(commandName.begin(), commandName.end(), commandName.begin(), ::tolower);
        command = gCommandList.at(commandName);
    }
    catch ( out_of_range const & e )
    {
        command = C_NONE;
    }

    return command;
}

command_t command_from_string(vector<string> & tokens, int count = 1)
{

    command_t command;

    try
    {
        string commandName = tokens[0];

        for ( int i = 1; i < count; i++ )
        {
            commandName += " ";
            commandName += tokens[i];
        }

        command = command_from_string(commandName);
    }
    catch ( out_of_range const & e )
    {
        command = C_NONE;
    }

    return command;
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

        case C_NEW :
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

        case C_UNDO :
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
            if ( g_CursorKeys.MenuActive() )
                set_status(STAT_POS_2, g_CursorKeys.Help().c_str());
            else
                set_status(STAT_POS_2, "Topics: 'control', 'pattern', 'misc' or try a command name");
            break;
        default:
            break;
    }
}


bool do_command(string/*const char * */ commandString)
{
    bool bResult = true;

    vector<string> tokens = split(commandString.c_str());

    if ( tokens.empty() )
        return bResult;

    command_t command;
//    int firstParameter;

    // Try for combinations first. Hopefully this makes the following switch
    // statement easier to read (though longer).

    for ( int i = tokens.size(); i >= 0; i-- )
        if ( command = command_from_string(tokens, i), command != C_NONE )
        {
            // firstParameter = tokens.size() - i;
            break;
        }

    // int command = command_from_string(tokens[0]);

    try
    {
        int iTemp;
        double fTemp;

        switch ( command )
        {

        case C_RUN :
            g_State.SetNewRunStatePending(true);
            break;

        case C_STOP :
            g_State.SetNewRunStatePending(false, 1);
            break;

        case C_HALT:
            g_State.SetRunState(false);
            break;


        case C_SET_LABEL:
            if ( tokens.size() > 1 )
            {
                size_t pos = commandString.find(' ') + 1;
                g_PatternStore.CurrentEditPattern().SetLabel(commandString.substr(pos).c_str());
            }
            set_status(STAT_POS_2, "Pattern %i: %s",
                g_PatternStore.CurrentEditPatternID(),
                g_PatternStore.CurrentEditPattern().Label(50).c_str());
            break;

        case C_PAGE_ONE:
            g_Display.SetBigPanelPage(Display::one);
            break;
        case C_PAGE_TWO:
            g_Display.SetBigPanelPage(Display::two);
            break;
        case C_PAGE_THREE:
            g_Display.SetBigPanelPage(Display::three);
            break;
        case C_PAGE_HOLD:
            g_Display.ToggleBigPanelHold();
            break;


        case C_CUE  :
        case C_EDIT :
            if ( tokens.size() < 2 )
            {
                g_PatternStore.SetFocus();
                g_PatternStore.SetStatus();
                show_status_after_navigation();
            }
            else
            {
                iTemp = stoi(tokens[1].c_str()) - 1;
                if ( ! g_PatternStore.ValidPosition(iTemp) )
                    throw string("Requested pattern number out of range at the moment.");
                switch ( command )
                {
                case C_CUE :
                    set_status(STAT_POS_2, "Cueing pattern: %s", tokens[1].c_str());
                    g_PatternStore.SetNewPatternPending(iTemp);
                    // newPattern = iTemp;
                    break;
                case C_EDIT :
                    set_status(STAT_POS_2, "Editing pattern: %s", tokens[1].c_str());
                    g_PatternStore.SetEditPos(iTemp);
                    set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
                    break;
                default:
                    break;
                }
            }
            break;

        case C_EDIT_CURSOR_LOCK:
            g_PatternStore.SetEditFocusFollowsPlay(true);
            set_status(STAT_POS_2, "Edit focus locked with playing pattern.");
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;
        case C_EDIT_CURSOR_UNLOCK:
            g_PatternStore.SetEditFocusFollowsPlay(false);
            set_status(STAT_POS_2, "Edit focus unlocked.");
            break;

        case C_NEW :
            g_PatternStore.AddEmptyPattern();
            // g_PatternStore.SetStepValCurrentEditPattern(g_State.StepValue());
            set_status(STAT_POS_2, "Empty pattern added at position %i.", g_PatternStore.m_Patterns.size());
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;

        case C_COPY :
            g_PatternStore.CopyCurrentPattern();
            set_status(STAT_POS_2, "Current pattern copied at position %i.", g_PatternStore.m_Patterns.size());
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;

        case C_DELETE :
            if ( g_PatternStore.PatternCount() == 0 )
                throw string("Nothing to delete.");
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
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
            break;

        case C_UNDO :
            g_PatternStore.PopDeletedPattern();
            set_status(STAT_POS_2, "Pattern restored at position %i.", g_PatternStore.m_Patterns.size());
            set_status_w(STAT_POS_PATTERN, g_PatternStore.PatternStatus().c_str());
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
            else if ( g_PatternStore.EditPatternIsPlayPattern() )
            {

                g_State.SetPatternReset(RESET_CURRENT);
                set_status(STAT_POS_2, "Current pattern will be reset.");
            }
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
                throw string("Hint: step n");
            fTemp = stod(tokens[1].c_str());
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
                fTemp = stod(tokens[1].c_str());
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
                throw string("Hint: gate h[old]|n[ormal]|n.n%");
            break;
        case C_GATE_HOLD:
            g_PatternStore.SetGateHold(true);
            break;
        case C_GATE_NORMAL:
            g_PatternStore.SetGateHold(false);
            break;

        case C_VELOCITY :
            if ( tokens.size() < 2 )
                throw string("Hint: vel[ocity] n");
            iTemp = stoi(tokens[1].c_str());
            if ( iTemp >= 0 && iTemp <= 127 )
            {
                set_status(STAT_POS_2, "Setting %s velocity: %s", g_PatternStore.UsePatternPlayData() ? "pattern" : "global", tokens[1].c_str());
                g_PatternStore.SetNoteVelocity(iTemp);
            }
            else
            {
                set_status(STAT_POS_2, "Velocity must be in the range 0 .. 127.", tokens[1].c_str());
            }
            break;

        case C_TRANSPOSE :
            if ( tokens.size() < 2 )
                throw string("Hint: trans[pose] n [now]");
            iTemp = stoi(tokens[1].c_str());
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
            show_status_after_navigation();
            break;
        case C_FEEL_HELP:
            throw string("feel new[list]|on|off|add|remove|respace|bypass");
            break;
        case C_FEEL_ON:
            g_PatternStore.FeelMapForEdit().SetActive(true);
            show_status_after_navigation();
            break;
        case C_FEEL_OFF:
            g_PatternStore.FeelMapForEdit().SetActive(false);
            show_status_after_navigation();
            break;
        case C_FEEL_NEW:
            g_PatternStore.FeelMapForEdit().New(tokens);
            show_status_after_navigation();
            break;
        case C_FEEL_ADD:
            g_PatternStore.FeelMapForEdit().Add();
            show_status_after_navigation();
            break;
        case C_FEEL_REMOVE:
            g_PatternStore.FeelMapForEdit().Remove();
            show_status_after_navigation();
            break;
        case C_FEEL_RESPACE:
            g_PatternStore.FeelMapForEdit().Respace();
            show_status_after_navigation();
            break;

        case C_SCALE:
            g_PatternStore.TranslateTableForEdit().SetStatus();    // This automatically sets focus.
            show_status_after_navigation();
            break;
        case C_SCALE_FROM_LIST:
            g_PatternStore.TranslateTableForEdit().SetScale(g_PatternStore.CurrentEditStepList());
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
                throw string("Hint: root C, C#, Eb, C5, F#6, etc.");
            else
                show_translation_status();
            break;

        case C_MIDI:
            if ( tokens.size() >= 2 )
            {
                // Expect a number and use it to set midi channel.

                int iTemp = stoi(tokens[1].c_str());
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
        case C_MIDI_OFF:
            g_ListBuilder.SetMidiInputMode(MIDI_INPUT_OFF);
            set_status(STAT_POS_2, "Midi Input OFF.");
            set_top_line();
            break;

        case C_QUANTUM :
            if ( tokens.size() < 2 )
                throw string("Hint: quan[tum] n.nn");
            fTemp = stod(tokens[1].c_str());
            if ( fTemp > 0 )
            {
                set_status(STAT_POS_2, "Setting new Quantum: %s", tokens[1].c_str());
                g_State.SetNewQuantumPending(fTemp);
                // newQuantum = fTemp;
            }
            break;

        case C_SET_RESETONPATTERNCHANGE : // Auto-reset
            if ( tokens.size() < 2 )
                throw string("Hint: autoreset on|off");
            if ( tokens[1] == "on")
                g_PatternStore.SetResetOnPatternChange(true);
            else if (tokens[1] == "off")
                g_PatternStore.SetResetOnPatternChange(false);
            else
                throw string("Autoreset not changed.");
            break;

        case C_PATTERN_CHAIN :
            if ( tokens.size() >= 2 )
                g_PatternStore.UpdatePatternChainFromSimpleString(commandString);
            g_PatternStore.PatternChainForEdit().SetStatus();
            g_PatternStore.PatternChainForEdit().SetFocus();
            show_status_after_navigation();
            break;
        case C_PATTERN_CHAIN_OFF:
            g_PatternStore.SetPatternChainMode(PatternChain::off);
            break;
        case C_PATTERN_CHAIN_NATURAL:
            g_PatternStore.SetPatternChainMode(PatternChain::natural);
            break;
        case C_PATTERN_CHAIN_QUANTUM:
            g_PatternStore.SetPatternChainMode(PatternChain::quantum);
            break;
        case C_PATTERN_CHAIN_CLEAR:
            g_PatternStore.PatternChainForEdit().Clear();
            show_status_after_navigation();
            break;
        case C_PATTERN_CHAIN_NEW:
            g_PatternStore.PatternChainForEdit().New();
            show_status_after_navigation();
            break;
        case C_PATTERN_CHAIN_DELETE:
            g_PatternStore.PatternChainForEdit().Delete();
            g_PatternStore.PatternChainForEdit().SetStatus();
            show_status_after_navigation();
            break;
        case C_PATTERN_CHAIN_JUMP:
            break;
        case C_PATTERN_CHAIN_HELP:
            set_status(STAT_POS_2, "chain off|natural|quantum|jump|new|delete");
            break;

        case C_LOAD:
        case C_STORE:
        case C_STORE_HELP:
            set_status(STAT_POS_2, "store|load step|scale|gate|vel|all");
            break;

        case C_STORE_STEP:
            g_PatternStore.StorePatternPlayData(PLAY_DATA_STEP );
            set_status(STAT_POS_2, "Step Value stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_GATE:
            g_PatternStore.StorePatternPlayData(PLAY_DATA_GATE );
            set_status(STAT_POS_2, "Gate Length stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_VELOCITY:
            g_PatternStore.StorePatternPlayData(PLAY_DATA_VELO );
            set_status(STAT_POS_2, "Velociry stored with pattern %i.", g_PatternStore.CurrentEditPatternID());
            break;
        case C_STORE_SCALE:
            g_PatternStore.StorePatternPlayData(PLAY_DATA_SCALE );
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

        case C_LIST:
            if ( tokens.size() < 2 )
                throw string("Hint: list new|delete|n [clear|: n1, n2 ,...]");
            set_status(STAT_POS_2, "%.60s", g_PatternStore.ListManager(commandString, tokens).c_str());
            update_pattern_panel();
            break;

        case C_LIST_IMPORT:
            if ( tokens.size() < 2 )
                throw string("Hint: import filename[.mid]");
            g_PatternStore.UpdatePatternFromMidiFile(commandString);
            set_status(STAT_POS_2, "File imported to Real Time list.");
            update_pattern_panel();
            break;

        case C_LIST_RT:
            g_PatternStore.CurrentEditRealTimeList().SetStatus();
            g_PatternStore.CurrentEditRealTimeList().SetFocus();
            show_status_after_navigation();
            break;

        case C_LIST_RT_DELETE:
            g_PatternStore.DeleteCurrentRealTimeList();
            g_PatternStore.SetFocus();
            g_PatternStore.SetStatus();
            show_status_after_navigation();
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

        case C_EXIT :
            bResult = false;
            break;

        case C_NONE :
            iTemp = stoi(tokens[0]) - 1;
            // Function returns a format string. Too obfuscated?
            set_status(STAT_POS_2, g_PatternStore.SetNewPatternOrJump(iTemp).c_str(), iTemp + 1);
            break;
        default :
            break;
        }
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
        set_status(STAT_POS_2, "%s", s.c_str());
    }

    // Keeping track of all the places I might need to do this
    // is nearly impossible, so might as well just do it every
    // time.

    update_edit_panels(true);
    commandString = "";
    return bResult;
}

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
