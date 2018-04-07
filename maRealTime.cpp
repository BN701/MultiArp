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

#include "maNotes.h"

#include <algorithm>
#include <cmath>
#include <unordered_map>

#include "maListGroup.h"
#include "maRealTimeList.h"
#include "maStepList.h"

#if defined(MA_BLUE)
#include <cstdio>
#endif

#include "maUtility.h"

using namespace std;


//
// RealTimeList
//
//////////////////////////////////////////////////////////////////////////

#if defined(MA_BLUE)
unordered_map<int, const char *> rtp_window_mode_names =
#else
unordered_map<RealTimeListParams::rtp_window_mode_t, const char *> rtp_window_mode_names =
#endif
{
    {RealTimeListParams::normal, "centered"},
    {RealTimeListParams::small, "centered-small"},
    {RealTimeListParams::look_ahead, "ahead"}
};

RealTimeListParams::rtp_window_mode_t rtp_window_mode_lookup(string s)
{
    static unordered_map<string, RealTimeListParams::rtp_window_mode_t> map;

    // Initialize on first use.

    if ( map.size() == 0 )
        for ( RealTimeListParams::rtp_window_mode_t m = static_cast<RealTimeListParams::rtp_window_mode_t>(0);
              m < RealTimeListParams::num_rtp_window_modes;
              m = static_cast<RealTimeListParams::rtp_window_mode_t>(static_cast<int>(m) + 1) )
        {
            map.insert(std::make_pair(rtp_window_mode_names.at(m), m));
        }

    return map.at(s);
}

void RealTimeListParams::NextWindowMode( int dir )
{
    // Move up and down list but don't wrap.

    int t = static_cast<int>(m_WindowMode) + dir;

    if ( t >= 0 && t < static_cast<int>(num_rtp_window_modes))
    {
        m_WindowMode = static_cast<rtp_window_mode_t>(t);
    }
}


void RealTimeListParams::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    snprintf(buff, 200, "[RT List %i] ", m_ItemID);
    m_Status = buff;

    m_Status += " Loop - ";

    m_Status += "S: ";
    pos = m_Status.size();
    snprintf(buff, 200, "%.2f", m_LoopStart);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Q: ";
    pos = m_Status.size();
    snprintf(buff, 200, "%.2f", m_Quantum);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Mul: ";
    pos = m_Status.size();
    snprintf(buff, 200, "%.2f", m_Multiplier);
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Status += " Window Mode: ";
    pos = m_Status.size();
    snprintf(buff, 200, "%s", rtp_window_mode_names.at(m_WindowMode));
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    m_Highlights.push_back(m_FieldPositions.at(m_RTParamsFocus));
}

bool RealTimeListParams::HandleKey(BaseUI::key_command_t k)
{
    int temp;
    double inc = 0.1;
    switch ( k )
    {
    case BaseUI::key_return:
    case BaseUI::key_backspace:
    case BaseUI::key_escape:
        ReturnFocus();
        break;

    case BaseUI::key_left:
        temp = static_cast<int>(m_RTParamsFocus) - 1;
        if ( temp >= 0 && temp < num_rt_params_menu_focus_modes )
            m_RTParamsFocus = static_cast<rt_params_menu_focus_t>(temp);
        break;

    case BaseUI::key_right:
        temp = static_cast<int>(m_RTParamsFocus) + 1;
        if ( temp >= 0 && temp < num_rt_params_menu_focus_modes )
            m_RTParamsFocus = static_cast<rt_params_menu_focus_t>(temp);
        break;

    case BaseUI::key_delete:
        switch ( m_RTParamsFocus )
        {
        case rtp_loop_start:
            m_LoopStart = 0;
            break;
        case rtp_local_quantum:
            m_Quantum = m_QuantumAtCapture;
            break;
        case rtp_multiplier:
            m_Multiplier = 1.0;
            break;
        default:
            break;
        }
        break;

    case BaseUI::key_shift_up:
        inc = 0.01;
    case BaseUI::key_up:
        switch ( m_RTParamsFocus )
        {
        case rtp_loop_start:
            m_LoopStart = tidy_3(m_LoopStart + inc);
            break;
        case rtp_local_quantum:
            m_Quantum = tidy_3(m_Quantum + inc);
            break;
        case rtp_multiplier:
            m_Multiplier = tidy_3(m_Multiplier + inc);
            break;
        case rtp_window_adjust:
            NextWindowMode(-1);
            break;
        default:
            break;
        }
        break;

    case BaseUI::key_shift_down:
        inc = 0.01;
    case BaseUI::key_down:
        switch ( m_RTParamsFocus )
        {
        case rtp_loop_start:
            m_LoopStart = tidy_3(m_LoopStart - inc);
            break;
        case rtp_local_quantum:
            m_Quantum = tidy_3(m_Quantum - inc);
            if ( m_Quantum < 0.01 )
                m_Quantum = 0.01;
            break;
        case rtp_multiplier:
            m_Multiplier = tidy_3(m_Multiplier - inc);
            break;
        case rtp_window_adjust:
            NextWindowMode(1);
            break;
        default:
            break;
        }
        break;

    default:
        return false;
    }

    m_FirstField = m_RTParamsFocus == 0;

//    SetStatus();

    return true;
}

map<double,Note>::iterator RealTimeList::MoveNote(map<double, Note>::iterator it, double newPhase)
{
    // Avoid clashes.

    while ( true )
    {
        if ( m_RealTimeList.find(newPhase) == m_RealTimeList.end() )
            break;
        newPhase += 0.001;
    }

    // Make a copy of the note and erase the original.

    auto ret = m_RealTimeList.insert(make_pair(newPhase, it->second));
    m_RealTimeList.erase(it);

    // Increment phase.

    ret.first->second.SetPhase(newPhase);

    return ret.first;
}

map<double,Note>::iterator RealTimeList::CopyNote(map<double, Note>::iterator it)
{
    double newPhase = it->second.m_Phase + 0.001;

    while ( true )
    {
        if ( m_RealTimeList.find(newPhase) == m_RealTimeList.end() )
            break;
        newPhase += 0.001;
    }

    auto ret = m_RealTimeList.insert(make_pair(newPhase, it->second));
    ret.first->second.SetPhase(newPhase);
    return ret.first;
}


void RealTimeList::SetStatus()
{
    int pos = 0;
    char buff[200];

    m_FieldPositions.clear();
    m_Highlights.clear();

    snprintf(buff, 200, "[RT List %i] ", m_ItemID);
    m_Status = buff;

    pos = m_Status.size();
    m_Status += "Params";
    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

    map<double, Note>::iterator it;

    for ( it = m_RealTimeList.begin(); it != m_RealTimeList.end(); it++ )
    {
        pos = m_Status.size() + 1; // "+ 1" because there's a space before the phase value.
        snprintf(buff, 200, " %.2f:", it->first);
        m_Status += buff;
        m_Status += it->second.ToString(false);
        m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));
    }

    m_Highlights.push_back(m_FieldPositions.at(m_RTListFocus));

}

bool RealTimeList::HandleKey(BaseUI::key_command_t k)
{
    double inc = 0.1;

    map<double, Note>::iterator it = m_RealTimeList.begin();
    for ( int i = 0; i < m_RTListFocus - m_SubMenus; i++ )
        it++;   // There's no + operator on iterators for maps, apparently, so we move step-by-step.

    switch ( k )
    {
    case BaseUI::key_return:
        if ( m_RTListFocus < m_SubMenus )
        {
            m_Params.SetItemID(m_ItemID);
            m_Params.SetFocus();
            m_Params.SetStatus();
            m_Params.SetReturnFocus(this);
        }
        else
        {
            Note & n = it->second;
            n.SetFocus();
            n.SetStatus();
            n.SetReturnFocus(this);
        }
        break;

    case BaseUI::key_backspace:
    case BaseUI::key_escape:
        ReturnFocus();
        break;

    case BaseUI::key_left:
        if ( m_RTListFocus > 0 )
            m_RTListFocus -= 1;
        break;

    case BaseUI::key_right:
        if ( static_cast<unsigned>(m_RTListFocus) < m_FieldPositions.size() - 1 )
            m_RTListFocus += 1;
        break;

    case BaseUI::key_shift_up:
    case BaseUI::key_shift_down:
        inc = 0.01;
    case BaseUI::key_up:
    case BaseUI::key_down:
        if ( m_RTListFocus < m_SubMenus && m_ReturnFocus != NULL )
        {
            m_ReturnFocus->HandleKey(k);
            m_ReturnFocus->HandleKey(BaseUI::key_return);
            return true;
        }
        else if ( m_RTListFocus >= m_SubMenus )   // Update note start time (phase).
        {
            if ( k == BaseUI::key_down || k == BaseUI::key_shift_down )
                inc *= -1;

            auto result = MoveNote(it, it->second.Phase() + inc);

            // We get back an iterator to the new entry. Set menu
            // focus by counting backwards to start of map.

            m_RTListFocus = m_SubMenus;
            while ( m_RealTimeList.begin() != result-- )
                m_RTListFocus += 1;
        }
        break;

    case BaseUI::key_shift_delete:
        if ( m_RTListFocus >= m_SubMenus )
        {
            m_UndoList.push_back(it->second);
            m_RealTimeList.erase(it);
            if ( m_RTListFocus - m_SubMenus >= static_cast<int>(m_RealTimeList.size()) )
                m_RTListFocus -= 1;
        }
        break;

    case BaseUI::key_ctrl_delete:
        if ( !m_UndoList.empty() )
        {
            // Re-insert and reset our cursor position.

            Note & n = m_UndoList.back();

            pair<map<double,Note>::iterator, bool> result = m_RealTimeList.insert(make_pair(n.Phase(), n));

            if ( result.second )
            {
                m_RTListFocus = m_SubMenus;
                while ( m_RealTimeList.begin() != result.first-- )
                    m_RTListFocus += 1;
            }

            m_UndoList.pop_back();
        }
        break;

    default:
        return false;
    }


    m_FirstField = m_RTListFocus == 0;

//    SetStatus();

    return true;
}

enum rtl_element_names_t
{
    rtl_name_loop,
    rtl_name_quantum,
    rtl_name_multiplier,
    rtl_name_window_adjust,
    num_rtl_element_names
};

#if defined(MA_BLUE)
unordered_map<int, const char *> rtl_element_names =
#else
unordered_map<rtl_element_names_t, const char *> rtl_element_names =
#endif
{
    {rtl_name_loop, "Loop"},
    {rtl_name_quantum, "Quantum"},
    {rtl_name_multiplier, "Multiplier"},
    {rtl_name_window_adjust, "Window"},
    {num_rtl_element_names, ""}
};

void RealTimeList::FromString(string s)
{
    vector<string> tokens = split(s.c_str(), ',', true);

    if ( tokens.size() == 0 )
#ifdef MA_BLUE
        return;
#else
        throw string("Note List parse error: nothing found.");
#endif

    // Expect field list in first token ...

    for ( rtl_element_names_t e = static_cast<rtl_element_names_t>(0);
          e < num_rtl_element_names;
          e = static_cast<rtl_element_names_t>(static_cast<int>(e) + 1) )
    {
        string token = find_token(tokens.at(0), rtl_element_names.at(e));

        if ( token.empty() )
            continue;

#ifndef MA_BLUE
        try
        {
#endif
            switch (e)
            {
            case rtl_name_loop:
                m_Params.m_LoopStart = strtod(token.c_str(), NULL);
                break;
            case rtl_name_quantum:
                m_Params.m_Quantum = strtod(token.c_str(), NULL);
                break;
            case rtl_name_multiplier:
                m_Params.m_Multiplier = strtod(token.c_str(), NULL);
                break;
            case rtl_name_window_adjust:
                m_Params.m_WindowMode = rtp_window_mode_lookup(token);
                break;
            default:
                break;
            }
#ifndef MA_BLUE
        }
        catch(invalid_argument ex)
        {

        }
        catch(out_of_range ex)
        {
        }
#endif
    }

    // Anything after that is a note.

    if ( tokens.size() == 1 )
        return;  // Leave note list intact.

    m_RealTimeList.clear();

    for ( vector<string>::iterator it = tokens.begin() + 1; it != tokens.end(); it++ )
    {
        Note note;
        if ( !note.NoteFromString(*it) )
            continue;

        // Check for existing entries and adjust start time if found.

        while ( true )
        {
//            map<double,Note>::iterator it2 = m_RealTimeList.find(note.Phase());
            if ( m_RealTimeList.find(note.Phase()) == m_RealTimeList.end() )
                break;
            note.SetPhase(note.Phase() + 0.0001);
        }

        m_RealTimeList.insert(make_pair(note.Phase(), note));
    }
}

string RealTimeList::ToString()
{
    string result;

    char buff[200];
    snprintf(buff, 200, " %s %.3f %s %.3f %s %.3f %s '%s'",
            rtl_element_names.at(rtl_name_loop), m_Params.m_LoopStart,
            rtl_element_names.at(rtl_name_quantum), m_Params.m_Quantum,
            rtl_element_names.at(rtl_name_multiplier), m_Params.m_Multiplier,
            rtl_element_names.at(rtl_name_window_adjust), rtp_window_mode_names.at(m_Params.m_WindowMode)
            );

    result += buff;

    int i = 0;
    for (map<double,Note>::iterator it = m_RealTimeList.begin(); it != m_RealTimeList.end(); it++)
    {
        result += ", ";
        if ( i++ % 4 == 0 )
            result += "\\\n     ";
        result += it->second.ToString();
    }

    return result;
}

// Less efficient (probably) but easier to read (possibly) ...

string RealTimeList::ToStringForDisplay(int & offset, int & length, unsigned width)
{
    offset = 0;
    length = 0;

    char buff[100];
    snprintf(buff, 100, "%05.2f ", m_LastRequestedPhase);
//    snprintf(buff, 100, "%05.2f ", m_InternalBeat);

    string result = buff;

    long topLimit = lround(1000 * m_Params.m_QuantumAtCapture);
    long midLimit = lround(1000 * m_LastRequestedPhase);

    double windowPos = m_LastRequestedPhase;
    double windowStep = 4.0/m_LastRequestedStepValue;

    Cluster notes;

    while ( lround(1000 * windowPos) < topLimit )
    {
        double windowStart = windowPos - windowStep/2;
        double windowEnd = windowPos + windowStep/2;

        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                        it != m_RealTimeList.upper_bound(windowEnd); it++ )
            notes.Add(it->second);

        if ( !notes.Empty() )
        {
            result += notes.ToString(false);
            notes.Clear();
        }
        else
            result += '-';

        windowPos += windowStep;
    }

    windowPos = 0;

    while ( lround(1000 * windowPos) < midLimit )
    {
        double windowStart = windowPos - windowStep/2;
        double windowEnd = windowPos + windowStep/2;

        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                        it != m_RealTimeList.upper_bound(windowEnd); it++ )
            notes.Add(it->second);

        if ( windowPos == 0 )
            offset = result.size();

        if ( !notes.Empty() )
        {
            result += notes.ToString(false);
            notes.Clear();
        }
        else
            result += '-';

        if ( windowPos == 0 )
            length = result.size() - offset;

        windowPos += windowStep;
    }

    if ( result.size() > width )
        result.resize(width);

#if 0
    snprintf(buff, 100, "\n    Multiplier %.2f, Loop Start %.2f, Loop Quantum %.2f, Window Adjust %s",
        m_Params.m_Multiplier,
        m_Params.m_LoopStart,
        m_Params.m_LocalQuantum,
        m_Params.m_AdjustWindowToStep ? "ON" : "OFF");
    result += buff;
#endif

    return result;
}


void RealTimeList::Step(Cluster & cluster, double phase, double stepValue)
{
    bool localLoop = lround(m_Params.m_Quantum) > 0;

    phase *= m_Params.m_Multiplier;

    // Wrap to start of local loop.

    if ( localLoop )
        while ( phase > m_Params.m_Quantum )
            phase -= m_Params.m_Quantum;

    phase += m_Params.m_LoopStart;

    // Wrap to start of capture loop loop.

    if ( localLoop )
        while ( phase > m_Params.m_QuantumAtCapture )
            phase -= m_Params.m_QuantumAtCapture;

    m_LastRequestedStepValue = stepValue;
    m_LastRequestedPhase = phase;

    double window = 4.0 * m_Params.m_Multiplier/stepValue;
    double windowStart, windowEnd;

    switch ( m_Params.m_WindowMode )
    {
    case RealTimeListParams::normal:
    case RealTimeListParams::small:
        if ( m_Params.m_WindowMode == RealTimeListParams::small && abs(m_Params.m_Multiplier) < 1.0 )
            window *= m_Params.m_Multiplier;

        windowStart = phase - window/2;
        windowEnd = phase + window/2;
        break;

    case RealTimeListParams::look_ahead:
        windowStart = phase;
        windowEnd = phase + window;
        break;

    default:
        break;
    }

    for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
                    it != m_RealTimeList.upper_bound(windowEnd); it++ )
    {
        cluster.Add(it->second);
    }

    // When phase is zero, window start will be negative, so we also need to
    // look for notes at the top of the loop that would normally be quantized
    // up to next beat zero.

//    if ( windowStart < 0 )
//    {
//        windowStart += m_LinkQuantum;
//        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(windowStart);
//                    it != m_RealTimeList.upper_bound(m_LinkQuantum); it++ )
//            m_Captured.Add(it->second);
//    }

//    return result;

}


void RealTimeList::BeginEcho(double inc, double target, int interval)
{
    // Iterator here is a map pair.

    // Make a set up references to current map members before duplicating
    // them. Updating the map while we're traversing it leads to confusion!

    vector<map<double,Note>::iterator> notes;

    for ( auto p = m_RealTimeList.begin(); p != m_RealTimeList.end(); p++ )
        notes.push_back(p);

    for ( auto p = notes.begin(); p != notes.end(); p++ )
    {
        double newPhase = (*p)->second.m_Phase + inc;
        pair<map<double,Note>::iterator,bool> ret = m_RealTimeList.insert(make_pair(newPhase, (*p)->second));

        if ( ret.second )
        {
            Note & n = ret.first->second;

            n.SetPhase(newPhase);
            n.m_Inc = inc;
            n.m_Moved = inc;        // We already added increment to start phase, above.
            n.m_Target = target;
            n.m_Interval = interval;
            n.m_NoteNumber += interval;
        }
    }
}

enum rte_target_action_mode_t
{
    rteam_delete,
    rteam_freeze,
    rteam_echo_all,
    rteam_echo_all_delete,
    rteam_echo_all_freeze,
    rteam_copy,
    rteam_copy_freeze,
    rteam_copy_freeze_target_multiplier,
    num_rte_target_action_modes
};

bool RealTimeList::NoteTargetAction(map<double,Note>::iterator mapEntry, double newPhase)
{
    bool result = false;
    rte_target_action_mode_t mode = rteam_copy_freeze_target_multiplier;

    switch(mode)
    {
    case rteam_delete:
        m_RealTimeList.erase(mapEntry);
        return true;
    default:
        break;
    }

    Note * note = & mapEntry->second;

    // Echo tasks ...

    switch(mode)
    {
    case rteam_echo_all:
    case rteam_echo_all_delete:
    case rteam_echo_all_freeze:
        BeginEcho(note->m_Inc, note->m_Target, note->m_Interval);
        break;
    default:
        break;
    }

    // The current note is copied ...

    Note * newNote = NULL;

    switch(mode)
    {
    case rteam_copy:
    case rteam_copy_freeze:
    case rteam_copy_freeze_target_multiplier:
        newNote = & CopyNote(mapEntry)->second;
        break;
    default:
        break;
    }

    // Copied note simple initialisation, including interval transpose.

    switch(mode)
    {
    case rteam_copy:
    case rteam_copy_freeze:
    case rteam_copy_freeze_target_multiplier:
        newNote->m_Moved = 0;
        newNote->m_Phase = note->m_Phase + note->m_Inc;
        newNote->m_NoteNumber += newNote->m_Interval;
        break;
    default:
        break;
    }

    double targetMultiplier = 0.5;

    switch(mode)
    {
    case rteam_copy_freeze_target_multiplier:
        newNote->m_Target *= targetMultiplier;
        break;
    default:
        break;
    }
    // Delete or freeze the original note?

    switch(mode)
    {
    case rteam_echo_all_delete:
        m_RealTimeList.erase(mapEntry);
        result = true;
        break;
    case rteam_freeze:
    case rteam_copy_freeze:
    case rteam_copy_freeze_target_multiplier:
    case rteam_echo_all_freeze:
        note->m_Inc = 0;
        note->m_Moved = 0;
        break;
    default:
        break;
    }

    return result;
}

void RealTimeList::DoEchoes()
{
    // Build a list of iterators to notes to be changed. (We
    // shouldn't modify the map whilst traversing it!)

    vector<map<double,Note>::iterator> entries;

    for ( auto p = m_RealTimeList.begin(); p != m_RealTimeList.end(); p++ )
        if ( !equals_3(p->second.m_Inc, 0))
            entries.push_back(p);

    for ( auto p = entries.begin(); p != entries.end(); p++ )
    {
        Note & note = (*p)->second;

        double newPhase = note.m_Phase + note.m_Inc;

        if ( newPhase >= m_Params.m_Quantum )
        {
            if ( m_Params.m_EchoesDeleteAtQuantum )
            {
                m_RealTimeList.erase(*p);
                continue;
            }

            if ( m_Params.m_EchoesWrapAtQuantum )
                newPhase -= m_Params.m_Quantum;
        }

        // Have we reached the target?

        if ( note.IncrementAndCheckTarget() && NoteTargetAction(*p, newPhase) )
            continue;

        // MoveNote() inserts a copy at the new location. The previous
        // map entry and its note is deleted.

        /*Note & newNote =*/ MoveNote(*p, newPhase)->second;

        // Any further processing on new note? If so, do it here.


    }
}

unsigned long RealTimeList::PhaseLength()
{
    // Lets do this with integers, return the result in thousanths of a beat. (mBeat?)
    unsigned long adjustedQuantum = lround(1000.0 * m_Params.m_Multiplier * m_Params.m_Quantum);
    unsigned long quantum = lround(1000.0 * m_Params.m_Quantum);

    // Using lowest common multiple gives us the 'adjusted' result, which needs to be
    // adjusted back to pattern time.

    return lcm(quantum, adjustedQuantum)/adjustedQuantum * quantum;

    // This method is unreliable: fmod() seems to return
    // a whole value of m_Quantum for some values
    // of adjustedQuantum, which means we don't find the
    // lowest multiple. (Rounding errors, I guess.)

//    double t = adjustedQuantum;
//    int mod = 0;
//
//    do
//    {
//        t += adjustedQuantum;
//        mod = lround(1000.0 * fmod(t, m_Params.m_Quantum));
//    } while ( mod != 0 );

//    return 0.001 * static_cast<double>(t) / m_Params.m_Multiplier;
}

void RealTimeList::Step2(Cluster & cluster, double globalPhase, double stepValue, double patternBeat)
{
//    double altPhase = fmod(m_Params.m_Multiplier * patternBeat, m_Params.m_Quantum);

    long lTemp = lround(1000.0 * m_Params.m_Multiplier * patternBeat);
    long lPhase = lTemp % lround(1000.0 * m_Params.m_Quantum);
    double phase = static_cast<double>(lPhase)/1000;

    m_LastRequestedStepValue = stepValue;
    m_LastRequestedPhase = phase;
    m_LastStepPhaseZero = lPhase == 0;

    if ( m_LastStepPhaseZero )
        DoEchoes();

    // Window is defined in the list time scale, so use the multiplier.

    double window = 4.0 * m_Params.m_Multiplier/stepValue;
    double windowStart, windowEnd;

    switch ( m_Params.m_WindowMode )
    {
    case RealTimeListParams::normal:
    case RealTimeListParams::small:

        // This doesn't make sense: window becomes 4 * mul-squared/stepValue.
        if ( m_Params.m_WindowMode == RealTimeListParams::small && abs(m_Params.m_Multiplier) < 1.0 )
            window *= m_Params.m_Multiplier;

        windowStart = phase - window/2;
        windowEnd = phase + window/2;
        break;

    case RealTimeListParams::look_ahead:
        windowStart = phase;
        windowEnd = phase + window;
        break;

    default:
        windowStart = phase - window/2;
        windowEnd = phase + window/2;
        break;
    }

    // Swap round if negative multiplier (reverse play).

    if ( windowStart > windowEnd )
    {
        double t = windowStart;
        windowStart = windowEnd;
        windowEnd = t;
    }

    // Step window may overlap either end of the original capture range (m_Params.m_Quantum), or may even
    // be bigger than the capture window, in which case we need to repeat the captured data at new locations
    // within the step window for playback. Here, N represents the capture window as it slides over the step
    // window, starting with the value of N that contains windowStart, ending with the value of N that contains
    // windowEnd. For each position of N, we grab the relevant events and map them to their playback location in
    // the playback stream, marked by globalPhase.

    for ( int N = ceil(windowStart/m_Params.m_Quantum) - 1; N < ceil(windowEnd/m_Params.m_Quantum); N++ )
    {
        double rangeOffset = N * m_Params.m_Quantum;

        // Set bounds for the collection window applied to captured data.

        double lower = max(windowStart, rangeOffset) - rangeOffset;
        double upper = min(windowEnd, rangeOffset + m_Params.m_Quantum) - rangeOffset - 0.001;

        // (Taking off 0.001 makes sure we test against the element at the top of
        // the collection winow, as upper_bound() "Returns an iterator pointing to
        // the first element whose key is considered to go *after* k.")

        // Slide the collection window over the captured data. (This doesn't wrap if we slide
        // over the end of the capture range.)

        lower += m_Params.m_LoopStart;
        upper += m_Params.m_LoopStart;

//        lower = 0.001 * lround(lower * 1000);
//        upper = 0.001 * lround(upper * 1000);
        lower = tidy_3(lower);
        upper = tidy_3(upper);

        // Protect map from scans where lower > upper. This can happen due to rounding errors
        // and/or imprecise floating point data. (If lower > upper executing the loop below
        // seems to produce race conditions and/or stack corruption.)

        if ( lower >= upper )
            continue;

        for ( map<double,Note>::iterator it = m_RealTimeList.lower_bound(lower);
                        it != m_RealTimeList.upper_bound(upper); it++ )
            cluster.Add(it->second).AdjustPhase(m_Params.m_Multiplier, phase, globalPhase, rangeOffset);
    }


}
