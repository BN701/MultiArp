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

#include "maPatternStore.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <unordered_map>

#include "maUtility.h"
#include "maState.h"

using namespace std;

void PatternStore::SetStatus()
{
    size_t pos = 0;
    m_Status.clear();
    m_Highlights.clear();
    m_FieldPositions.clear();

    if ( m_Patterns.empty() )
        return;

    char buff[100];

    m_Status = "Pattern ";
    pos = m_Status.size();
    sprintf(buff, "%i/%i", m_PosEdit + 1, m_Patterns.size());
    m_Status += buff;
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Status += ", Step List ";
    pos = m_Status.size();
    if ( !m_Patterns.at(m_PosEdit).m_StepListSet.empty() )
    {
        sprintf(buff, "%i/%i",
            m_Patterns.at(m_PosEdit).m_PosEdit + 1,
            m_Patterns.at(m_PosEdit).m_StepListSet.size());
        m_Status += buff;
    }
    else
    {
        m_Status += "-/-";
    }
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Status += ", Real Time List ";
    pos = m_Status.size();
    if ( !m_Patterns.at(m_PosEdit).m_RealTimeSet.empty() )
    {
        sprintf(buff, "%i/%i",
            m_Patterns.at(m_PosEdit).m_PosRealTimeEdit + 1,
            m_Patterns.at(m_PosEdit).m_RealTimeSet.size());
        m_Status += buff;
    }
    else
    {
        m_Status += "-/-";
    }
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Status += ", ";
    pos = m_Status.size();
    m_Status += "Trig List";
    m_FieldPositions.emplace_back(pos, m_Status.size() - pos);

    m_Highlights.push_back(m_FieldPositions.at(m_PatternStoreFocus));
}

bool PatternStore::HandleKey(key_type_t k)
{
    int temp;

    switch ( k )
    {
    case enter:
        switch ( m_PatternStoreFocus )
        {
        case psf_pattern:
            // TODO: Set current pattern (not Pattern Chain mode).
            break;

        case psf_list:
            if ( !m_Patterns.empty() && m_Patterns.at(m_PosEdit).StepListCount() > 0 )
            {
                StepList & s = m_Patterns.at(m_PosEdit).StepListForEdit();
                s.SetItemID(m_Patterns.at(m_PosEdit).m_PosEdit + 1);
                s.SetFocus();
                s.SetStatus();
                s.SetReturnFocus(this);
            }
            break;

        case psf_rt_list:
            if ( !m_Patterns.empty() && m_Patterns.at(m_PosEdit).RealTimeListCount() > 0 )
            {
                RealTimeList & r = m_Patterns.at(m_PosEdit).RTListForEdit();
                r.SetItemID(m_Patterns.at(m_PosEdit).m_PosRealTimeEdit + 1);
                r.SetFocus();
                r.SetStatus();
                r.SetReturnFocus(this);
            }
            break;

        case psf_trig_list:
            if ( !m_Patterns.empty() )
            {
                TrigList & t = m_Patterns.at(m_PosEdit).PatternTrigList();
                t.SetItemID(m_Patterns.at(m_PosEdit).m_PosRealTimeEdit + 1);
                t.SetFocus();
                t.SetStatus();
                t.SetReturnFocus(this);
            }
            break;

        default:
            break;
        }
        break;

    case left:
        temp = static_cast<int>(m_PatternStoreFocus) - 1;
        if ( temp >= 0 && temp < num_psf_menu_focus_modes )
            m_PatternStoreFocus = static_cast<pattern_store_menu_focus_t>(temp);
        break;

    case right:
        temp = static_cast<int>(m_PatternStoreFocus) + 1;
        if ( temp >= 0 && temp < num_psf_menu_focus_modes )
            m_PatternStoreFocus = static_cast<pattern_store_menu_focus_t>(temp);
        break;

    case up:
        switch ( m_PatternStoreFocus )
        {
        case psf_pattern:
            DownEditPos();
            break;
        case psf_list:
            DownListPos();
            break;
        case psf_rt_list:
            DownRTListPos();
            break;
        default:
            break;
        }
        break;

    case down:
        switch ( m_PatternStoreFocus )
        {
        case psf_pattern:
            UpEditPos();
            break;
        case psf_list:
            UpListPos();
            break;
        case psf_rt_list:
            UpRTListPos();
            break;
        default:
            break;
        }
        break;

    default:
        return false;
    }

    SetStatus();

    return true;
}

StepList & PatternStore::CurrentEditStepList()
{
    if ( m_Patterns.empty() )
        throw string("Pattern Store is Empty.");

    if ( m_Patterns.at(m_PosEdit).m_StepListSet.empty() )
        throw string("Current pattern has no lists.");

    size_t pos = m_Patterns.at(m_PosEdit).m_PosEdit;
    return m_Patterns.at(m_PosEdit).m_StepListSet.at(pos);
}

RealTimeList & PatternStore::CurrentEditRealTimeList()
{
    if ( m_Patterns.empty() )
        throw string("Pattern Store is Empty.");

    if ( m_Patterns.at(m_PosEdit).m_RealTimeSet.empty() )
        throw string("Current pattern has no real time lists.");

    size_t pos = m_Patterns.at(m_PosEdit).m_PosRealTimeEdit;
    return m_Patterns.at(m_PosEdit).m_RealTimeSet.at(pos);
}

Pattern & PatternStore::CurrentPlayPattern()
{
    if ( m_Patterns.empty() )
        throw string("Pattern Store is Empty.");

    return m_Patterns.at(m_PosPlay);
}

Pattern & PatternStore::CurrentEditPattern()
{
    if ( m_Patterns.empty() )
        throw string("Pattern Store is Empty.");

    return m_Patterns.at(m_PosEdit);
}

void PatternStore::UpListPos()
{
    if ( !m_Patterns.empty() )
        m_Patterns[m_PosEdit].UpEditPos();
}

void PatternStore::DownListPos()
{
    if ( !m_Patterns.empty() )
        m_Patterns[m_PosEdit].DownEditPos();
}

void PatternStore::UpRTListPos()
{
    if ( !m_Patterns.empty() )
        m_Patterns[m_PosEdit].UpRTEditPos();
}

void PatternStore::DownRTListPos()
{
    if ( !m_Patterns.empty() )
        m_Patterns[m_PosEdit].DownRTEditPos();
}

string PatternStore::PatternChainToStringForDisplay(int firstRow, int rows)
{
    if ( m_Patterns.empty() )
        return "No patterns loaded.\n";

    return m_PatternChain.ToStringForDisplay(firstRow, rows);

}

string PatternStore::PatternStatus()
{
    string result;

    if ( m_Patterns.empty() )
        return "No patterns loaded.\n";

    char buf[80];

    sprintf(buf, "Play: %i", m_PosPlay + 1);
    result += buf;

    switch ( m_PatternChain.Mode() )
    {
        case PatternChain::off :
            strcpy(buf, ", Chain: Off");
            break;

        case PatternChain::natural :
            sprintf(buf, ", Chain: N [%i/%i]", m_PatternChain.PosPlay() + 1, m_PatternChain.size());
            break;

        case PatternChain::quantum :
            sprintf(buf, ", Chain: Q, [%i/%i]", m_PatternChain.PosPlay() + 1, m_PatternChain.size());
            break;
    }

    result += buf;


    sprintf(buf, "\nEdit: %i", m_PosEdit + 1);
    result += buf;

    if (  ! m_Patterns.at(m_PosEdit).m_StepListSet.empty() )
    {
        sprintf(buf, ", List %i", m_Patterns.at(m_PosEdit).m_PosEdit + 1);
        result += buf;

        int listCount = m_Patterns.at(m_PosEdit).m_StepListSet.size();
        if ( listCount > 1 )
        {
            sprintf(buf, " of %i", listCount);
            result += buf;
        }
    }

    return result;
}


string PatternStore::PatternOverview()
{
    char buff[200];

    sprintf(buff, "Patterns: %i, Edit %s, Global Vel/Gate: %i/%.0f%% (Hold %s)",
               PatternCount(),
               EditFocusFollowsPlay() ? "locked" : "unlocked",
               m_DefaultPattern.Velocity(),
               m_DefaultPattern.Gate() * 100,
               m_DefaultPattern.GateHold() ? "on" : "off");

    return buff;
}

void PatternStore::Step(Cluster & cluster, TrigRepeater & repeater, double phase, double stepValue)
{
    /*
        As long as PatternChanged() is called for every step, we
        don't need to clear the flag here.
     */

    if ( m_Patterns.empty() )
        return;

    if ( m_PatternChain.Mode() != PatternChain::off && ! m_PatternChain.empty() )
    {
        bool changePattern = false;
        switch ( m_PatternChain.Mode() )
        {
            case PatternChain::natural :
                if ( m_Patterns.at(m_PosPlay).AllListsComplete() )
                    changePattern = true;
                break;

            case PatternChain::quantum :
                if ( m_PhaseIsZero )
                    changePattern = true;
                break;
        }

        while ( changePattern ) // Not a loop, just something we can 'break' out of.
        {
            int next = m_PatternChain.JumpOverride();

            // TODO: Can probably do this internally to PatternChain at some point.

            if ( next < 0 && m_PatternChain.at(m_PatternChain.PosPlay()).Remaining() > 0 )
                break;

            if ( next < 0 )
                next = m_PatternChain.at(m_PatternChain.PosPlay()).Jump();

            if ( next < 0 )
                next = m_PatternChain.PosPlay() + 1;

            if ( next >= m_PatternChain.size() )
                next = 0;

            m_PatternChain.SetPosPlay(next);

            // Don't change play pointer if new pattern out of range.

            int pos = m_PatternChain.at(m_PatternChain.PosPlay()).Pattern();

            if ( pos < m_Patterns.size() )
            {
                m_PosPlay = pos;
                if ( m_EditPosFollowsPlay )
                    m_PosEdit = m_PosPlay;
            }

            m_PatternChanged = true;

            if ( m_ResetOnPatternChange )
                m_Patterns.at(m_PosPlay).ResetPosition();

            break;
        }
    }

    m_PhaseIsZero = false;

    m_Patterns.at(m_PosPlay).Step(cluster, repeater, m_StepValueMultiplier, phase, stepValue);
}

void PatternStore::UpdatePatternChainFromSimpleString(string s)
{
    m_PatternChain.FromSimpleString(s);
    m_PatternChain.ResetPosPlay();
}

void PatternStore::UpdatePatternChainFromString(string s)
{
    m_PatternChain.FromString(s);
    m_PatternChain.ResetPosPlay();
}

string PatternStore::PatternChainToString()
{
    return m_PatternChain.ToString();
}

string PatternStore::EditPatternToString()
{
    if ( m_Patterns.empty() )
        return "";
    return m_Patterns.at(m_PosEdit).ToString("Pattern");
}

string PatternStore::PatternSelectionList(int start, int rows)
{
    string result;

    if ( m_Patterns.empty() )
        return result;

    for ( size_t pos = start; pos < start + rows && pos < m_Patterns.size(); pos++ )
    {
        char buff[50];
        sprintf(buff, " %02i ", pos + 1);
        result += buff;
        result += m_Patterns.at(pos).Label(15);
        result += '\n';
    }

    return result;
}

void PatternStore::UpdatePattern(StepList & noteList)
{
    if ( m_Patterns.empty() )
        m_Patterns.emplace_back();

    m_Patterns.at(m_PosEdit).ReplaceList(noteList);
}

void PatternStore::UpdatePattern(std::map<double,Note> & realTimeList, double quantum)
{
    if ( m_Patterns.empty() )
        m_Patterns.emplace_back();

    m_Patterns.at(m_PosEdit).AddRealTimeList(realTimeList, quantum);
}

enum ps_element_names_t
{
    ps_heading,
    ps_name_pattern_chain_mode,
    ps_name_reset_on_pattern_change,
    ps_name_edit_focus_follows_play,
    ps_name_use_pattern_play_data,
    num_ps_element_names
};


unordered_map<ps_element_names_t, const char *> ps_element_names = {
    {ps_heading, "Pattern Store"},
    {ps_name_pattern_chain_mode, "Pattern Chain Mode"},
    {ps_name_reset_on_pattern_change, "Reset on pattern change"},
    {ps_name_edit_focus_follows_play, "Edit focus follows play"},
    {ps_name_use_pattern_play_data, "Use Pattern Play Data"}
};


string PatternStore::ToString()
{
    char buff[100];
    string result;

    result += "<< Pattern Store >>\n\n";
    result += PatternChainToString();
    result += "\n\n";

    sprintf(buff, "Store %s %i\n", ps_element_names.at(ps_name_pattern_chain_mode), static_cast<int>(m_PatternChain.Mode()));
    result += buff;
    sprintf(buff, "Store %s %s\n", ps_element_names.at(ps_name_reset_on_pattern_change), m_ResetOnPatternChange ? "On" : "Off");
    result += buff;
    sprintf(buff, "Store %s %s\n", ps_element_names.at(ps_name_edit_focus_follows_play), m_EditPosFollowsPlay ? "On" : "Off");
    result += buff;
    sprintf(buff, "Store %s %s\n", ps_element_names.at(ps_name_use_pattern_play_data), m_UsePatternPlayData ? "On" : "Off");
    result += buff;
    result += '\n';

    result += "<< Pattern Default >>\n\n";
    result += m_DefaultPattern.ToString("Default");
    result += "\n\n";

    for ( int i = 0; i < m_Patterns.size(); i++ )
    {
        sprintf(buff, "<< Pattern %i >>\n\n", i + 1);
        result += buff;
        result += m_Patterns.at(i).ToString("Pattern");
        result += "\n";
    }

    return result;
}

void PatternStore::SetFieldsFromString(string s)
{
    for ( ps_element_names_t e = static_cast<ps_element_names_t>(1);
          e < num_ps_element_names;
          e = static_cast<ps_element_names_t>(static_cast<int>(e) + 1) )
    {
        string token = find_token(s, ps_element_names.at(e));

        if ( token.empty() )
            continue;

        transform(token.begin(), token.end(), token.begin(), ::tolower);

        try
        {
            switch (e)
            {
            case ps_name_pattern_chain_mode:
                m_PatternChain.SetMode(static_cast<PatternChain::pattern_chain_mode_t>(stoi(token)));
                break;
            case ps_name_reset_on_pattern_change:
                m_ResetOnPatternChange = token == "on";
                break;
            case ps_name_edit_focus_follows_play:
                m_EditPosFollowsPlay = token == "on";
                break;
            case ps_name_use_pattern_play_data:
                m_UsePatternPlayData = token == "on";
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


bool PatternStore::FromString(string s, int & created, int & updates)
{
    try
    {
        if ( s.find("<< Pattern Store >>") == 0 )
        {
            // Do nothing, it's just a section heading for readability.
            return true;
        }
        else if ( s.find("<< Pattern Default >>") == 0 ) // Add pattern to end of list.
        {
            // Do nothing, it's just a section heading for readability.
            return true;
        }
        else if ( s.find("<< Pattern >>") == 0 ) // Add pattern to end of list.
        {
            m_Patterns.emplace_back();
            m_PosEdit = m_Patterns.size() - 1;
            created += 1;
            return true;
        }
        else if ( s.find("<< Pattern ") == 0 ) // Create pattern with specific ID
        {
            size_t index = stoi(s.substr(11)) - 1;
            if ( m_Patterns.size() < index + 1)
                m_Patterns.resize(index + 1);
            m_PosEdit = index;
            created = m_Patterns.size();
            return true;
        }
        else if ( s.find("Chain ") == 0 )
        {
            UpdatePatternChainFromString(s);
            updates += 1;
            return true;
        }
        else if ( s.find("Store ") == 0 )
        {
            SetFieldsFromString(s);
            return true;
        }
        else if ( s.find("Default ") == 0 )
        {
            m_DefaultPattern.FromString(s, updates);
            return true;
        }
        else
        {
            if ( m_Patterns.empty() )
            {
                m_Patterns.emplace_back();
                created += 1;
            }
            return m_Patterns.at(m_PosEdit).FromString(s, updates);
        }
    }
    catch (invalid_argument e)
    {
        throw string("PatternStore::LoadFromString(), invalid argument.");
    }
}

void PatternStore::DeleteCurrentRealTimeList()
{
    if ( m_Patterns.empty() )
        throw string("Pattern Store is Empty.");

    if ( m_Patterns.at(m_PosEdit).m_RealTimeSet.empty() )
        throw string("Current pattern has no real time lists.");

    m_Patterns.at(m_PosEdit).DeleteCurrentRealTimeList();

    SetStatus();
}

string PatternStore::ListManager(string commandString, vector<string> & tokens)
{
    int index;

    // We're guaranteed to have at least a second token.
    // (The calling routine doesn't call if there's only one.)

    // Check for commands not specific to any list.

    if ( tokens[1] == "new")
    {
        if ( m_Patterns.empty() )
            m_Patterns.emplace_back();

        m_Patterns.at(m_PosEdit).NewList();
        return "Empty list created.";
    }
    else if ( tokens[1] == "delete")
    {
        if ( m_Patterns.empty() || m_Patterns.at(m_PosEdit).m_StepListSet.empty() )
            return "Nothing to delete.";
        m_Patterns.at(m_PosEdit).DeleteCurrentList();
        return "Current list deleted.";
    }


    // For everything else, we need a list index and that should
    // be the next token.

    try
    {
        index = stoi(tokens.at(1)) - 1;

        if ( index < 0 )
            return "List cannot be less than zero.";

    }
    catch ( invalid_argument e )
    {
        return "List number not valid.";
    }

    // If we just have a list index, display list and set list pointer
    // to this index, then return.

    if ( tokens.size() == 2 )
    {
        m_Patterns.at(m_PosEdit).SetEditPos(index);
        string temp = "L " + tokens.at(1) + " -" + m_Patterns.at(m_PosEdit).m_StepListSet.at(index).ToString();
        if ( temp.size() > 60 )
        {
            temp.resize(57);
            temp += "...";
        }
        return temp;
    }

    // At this point we have at least three tokens and we
    // know the list index is good.

    if ( tokens.at(2) == "clear" )
    {
        m_Patterns.at(m_PosEdit).m_StepListSet.at(index).Clear();
        return "List cleared.";
    }

    // If we get to here, expect an actual note list.

    if ( m_Patterns.empty() )
        m_Patterns.emplace_back();

    int pos = tokens.at(0).size() + tokens.at(1).size() + 1;
    m_Patterns.at(m_PosEdit).AddListFromString(index, commandString.substr(pos));

    return "List updated.";
}


TranslateTable & PatternStore::TranslateTableForPlay()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() /* && m_Patterns.at(m_PosEdit).TranslateTableValid() */ )
        return m_Patterns.at(m_PosPlay).PatternTranslateTable();
    else
        return m_DefaultPattern.PatternTranslateTable();
}


TranslateTable & PatternStore::TranslateTableForEdit(bool setFocus)
{
    TranslateTable * pTable = NULL;

    if ( m_UsePatternPlayData && !m_Patterns.empty() /* && m_Patterns.at(m_PosEdit).TranslateTableValid() */ )
        pTable = & m_Patterns.at(m_PosEdit).PatternTranslateTable();
    else
        pTable = & m_DefaultPattern.PatternTranslateTable();

    if ( setFocus )
        pTable->SetFocus();

    return *pTable;
}

FeelMap & PatternStore::FeelMapForPlay()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() /* && m_Patterns.at(m_PosEdit).TranslateTableValid() */ )
        return m_Patterns.at(m_PosPlay).PatternFeelMap();
    else
        return m_DefaultPattern.PatternFeelMap();
}


FeelMap & PatternStore::FeelMapForEdit(bool setFocus)
{
    FeelMap * pMap = NULL;

    if ( m_UsePatternPlayData && !m_Patterns.empty() /* && m_Patterns.at(m_PosEdit).TranslateTableValid() */ )
        pMap = & m_Patterns.at(m_PosEdit).PatternFeelMap();
    else
        pMap = & m_DefaultPattern.PatternFeelMap();

    if ( setFocus )
        pMap->SetFocus();

    return *pMap;
}


void PatternStore::SetUsePatternPlayData( bool val )
{
    if ( val && m_Patterns.empty() )
        throw string("Cannot use pattern play data until a pattern is loaded.");

    m_UsePatternPlayData = val;
}


void PatternStore::SetStepValue(double step)
{
    // TO TEST:
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        CurrentEditPattern().StoreStepValue(step);
    else
        m_DefaultPattern.StoreStepValue(step);

    // SetNewPatternPending(m_PosPlay); // Force reload
}

void PatternStore::SetGateLength(double gate)
{
    // TO TEST:
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        CurrentEditPattern().StoreGate(gate);
    else
        m_DefaultPattern.StoreGate(gate);
}

void PatternStore::SetGateHold(bool val)
{
    // TO TEST:
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        CurrentEditPattern().StoreGateHold(val);
    else
        m_DefaultPattern.StoreGateHold(val);
}

void PatternStore::SetNoteVelocity(unsigned char vel)
{
    // TO TEST:
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        CurrentEditPattern().StoreVelocity(vel);
    else
        m_DefaultPattern.StoreVelocity(vel);
}

double PatternStore::StepValue()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        return CurrentPlayPattern().StepValue();
    else
        return m_DefaultPattern.StepValue();
}

double PatternStore::GateLength()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        return CurrentPlayPattern().Gate();
    else
        return m_DefaultPattern.Gate();
}

bool PatternStore::GateHold()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        return CurrentPlayPattern().GateHold();
    else
        return m_DefaultPattern.GateHold();
}

unsigned char PatternStore::NoteVelocity()
{
    if ( m_UsePatternPlayData && !m_Patterns.empty() )
        return CurrentPlayPattern().Velocity();
    else
        return m_DefaultPattern.Velocity();
}

void PatternStore::StorePatternPlayData( /*State & state, TranslateTable & table,*/ unsigned char mask )
{
    if ( m_Patterns.empty() )
        throw string("Pattern Store is empty.");

    Pattern & p = m_Patterns.at(m_PosEdit);
    if ( mask & PLAY_DATA_STEP )
        p.StoreStepValue(m_DefaultPattern.StepValue());

    if ( mask & PLAY_DATA_GATE )
    {
        p.StoreGate(m_DefaultPattern.Gate());
        p.StoreGateHold(m_DefaultPattern.GateHold());
    }
    if ( mask & PLAY_DATA_VELO )
        p.StoreVelocity(m_DefaultPattern.Velocity());
    if ( mask & PLAY_DATA_SCALE )
        p.StoreTranslateTable(m_DefaultPattern.PatternTranslateTable());
}

string PatternStore::LoadPatternPlayData( /*State & state, TranslateTable & table,*/ unsigned char mask )
{
    if ( m_Patterns.empty() )
        throw string("Pattern Store is empty.");

    string result;

    Pattern & p = m_Patterns.at(m_PosEdit);

    if ( (mask & PLAY_DATA_STEP) /* && p.StepValueValid() */ )
    {
        m_DefaultPattern.SetStepValue(p.StepValue());
        result += "Step Value";
    }

    if ( (mask & PLAY_DATA_GATE) /* && p.GateValid() */ )
    {
        m_DefaultPattern.StoreGate(p.Gate());
        m_DefaultPattern.StoreGateHold(p.GateHold());
        if ( !result.empty() )
            result += ", ";
        result += "Gate Length";
    }

    if ( (mask & PLAY_DATA_VELO) /* && p.VelocityValid() */ )
    {
        m_DefaultPattern.StoreVelocity(p.Velocity());
        if ( !result.empty() )
            result += ", ";
        result += "Velocity";
    }

    if ( (mask & PLAY_DATA_SCALE) /* && p.TranslateTableValid() */)
    {
        m_DefaultPattern.StoreTranslateTable(p.PatternTranslateTable());
        if ( !result.empty() )
            result += ", ";
        result += "Translation Data";
    }

    if ( !result.empty() )
        result.insert(0, "Loading ");
    else
        result = "No play data has been stored here.";

    return result;
}

string PatternStore::ShowPatternPlayData()
{
    char buff[100];
    string result;

    bool usePatternPlayData = m_UsePatternPlayData && !m_Patterns.empty();
    Pattern & p = usePatternPlayData ? m_Patterns.at(m_PosEdit) : m_DefaultPattern;

    if ( usePatternPlayData )
    {
        sprintf(buff, "P %02i: ", m_PosEdit + 1);
        result += buff;
    }
    else
        result += "GLOB: ";

    sprintf(buff, "Step %.2f, ", p.StepValue());
    result += buff;

    sprintf(buff, "Vel/Gate %i/%.0f%% (Hold %s), ", p.Velocity(), p.Gate() * 100, p.GateHold() ? "on" : "off");
    result += buff;

    TranslateTable table = p.PatternTranslateTable();
    sprintf(buff, "C/T: %i/%i (%s), %s-%s",
            table.Transpose(),
            table.DegreeShift(),
            table.ShiftName(),
            table.RootName().c_str(),
            table.ScaleName());
    result += buff;

    return result;
}

string PatternStore::SetNewPatternOrJump( int val )
{
    if ( m_PatternChain.Mode() == PatternChain::off )
    {
        if ( val >= 0 && val < m_Patterns.size() )
        {
            SetNewPatternPending(val);
            return "Cueing pattern %i";
        }
        else
            throw string("Requested pattern doesn't exist!");
    }
    else
    {
        if ( val >= 0 && val < m_PatternChain.size() )
        {
            m_PatternChain.at(m_PatternChain.PosPlay()).ClearRemaining();
            m_PatternChain.SetJumpOverride(val);
            return "Jumping to chain step %i";
        }
        else
            throw string("Jump stage doesn't exist!");
    }
}
