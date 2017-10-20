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


#include <cstring>
#include <unordered_map>

#include "maUtility.h"

using namespace std;

Chord * Pattern::Step()
{
    if ( m_ListSet.empty() )
        return NULL;

    if ( m_Pos >= m_ListSet.size() )
    {
        m_Pos = 0;
    }

    m_LastRequestedPos = m_Pos;

    return m_ListSet[m_Pos++].Step();
}

bool Pattern::AllListsComplete()
{
    bool bResult = true;

    for ( std::vector<NoteList>::iterator it = m_ListSet.begin(); it < m_ListSet.end(); it++ )
    {
        bResult &= (*it).Complete();
    }

    return bResult;
}

bool Pattern::PlayPositionInfo(int & listIndex, int & offset, int & length)
{
    if ( m_ListSet.empty() )
        return false;

    listIndex = m_LastRequestedPos;

    return m_ListSet.at(m_LastRequestedPos).PlayPositionInfo(offset, length);
}

string Pattern::ListToString(vector<int>::size_type n)
{
    if ( n >= m_ListSet.size() )
        return "";

    return m_ListSet.at(n).ToString();
}

enum pat_element_names_t {
    pat_heading,
    pat_name_step_value,
    pat_name_gate,
    pat_name_gate_hold,
    pat_name_velocity,
    pat_name_label,
    number_pat_element_names
};


unordered_map<pat_element_names_t, const char *> pat_element_names = {
    {pat_heading, "Pattern"},
    {pat_name_step_value, "Step Value"},
    {pat_name_gate, "Gate"},
    {pat_name_gate_hold, "Hold"},
    {pat_name_label, "Label"},
    {pat_name_velocity, "Vel"}
};



string Pattern::ToString()
{
    string result = "Pattern ";

    if ( !m_Label.empty() )
    {
        result += pat_element_names.at(pat_name_label);
        result += " \"";
        result += m_Label;
        result += "\"\n";
    }

    char buffer[200];

    sprintf(buffer, "%s %.2f", pat_element_names.at(pat_name_step_value), m_StepValue);
    result += buffer;

    if ( !result.empty() )
        result += ' ';
    sprintf(buffer, "%s %.2f %s %s",
            pat_element_names.at(pat_name_gate), m_Gate,
            pat_element_names.at(pat_name_gate_hold), m_GateHold ? "ON" : "OFF");
    result += buffer;

    if ( !result.empty() )
        result += ' ';
    sprintf(buffer, "%s %i", pat_element_names.at(pat_name_velocity), m_Velocity);
    result += buffer;

    result += "\n";
    result += m_TranslateTable.ToString("Pattern");

    int index = 1;
    for ( vector<NoteList>::iterator i = m_ListSet.begin(); i != m_ListSet.end(); i++, index++ )
    {
        char buffer[12];
        sprintf(buffer, "List %i ", index);
        result += buffer;
        result += NoteList(*i).ToString();
        result += "\n";
    }
    return result;
}

void Pattern::SetFieldsFromString(string s)
{
    for ( pat_element_names_t e = static_cast<pat_element_names_t>(1);
          e < number_pat_element_names;
          e = static_cast<pat_element_names_t>(static_cast<int>(e) + 1) )
    {
        string token = find_token(s, pat_element_names.at(e));

        if ( token.empty() )
            continue;

        try
        {
            switch (e)
            {
            case pat_name_step_value:
                m_StepValue = stod(token);
                break;
            case pat_name_gate:
                m_Gate = stod(token);
                break;
            case pat_name_gate_hold:
                m_GateHold = token == "ON";
                break;
            case pat_name_velocity:
                m_Velocity = stoi(token);
                break;
            case pat_name_label:
                m_Label = token;
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


bool Pattern::FromString(string s, int & updates)
{
    try
    {
        if ( s.find("List ") == 0 )
        {
            int index = stoi(s.substr(5)) - 1;
            size_t pos = s.find(' ', 5);
            if ( pos == string::npos )
                throw string("Pattern::FromString(), parameter list is empty.");
            AddListFromString(index, s.substr(pos));
            updates += 1;
            return true;
        }
        else if ( s.find("Trigs ") == 0 )
        {
            updates += 1;
            return true;
        }
        else if ( s.find("Pattern Scale ") == 0 )
        {
            m_TranslateTable.FromString(s);
            updates += 1;
            return true;
        }
        else if ( s.find("Pattern ") == 0 )
        {
            SetFieldsFromString(s);
            updates += 1;
            return true;
        }
        else
            return false;
    }
    catch (invalid_argument ex)
    {
        throw string("Pattern::FromString(), failed to read a number somewhere in the parameter list.");
    }
}

void Pattern::AddListFromString(vector<int>::size_type index, string s)
{
    if ( index < 0 )
        throw string("Pattern::AddListFromString(), invalid list index.");

    if ( index >= m_ListSet.size() )
        m_ListSet.resize(index + 1);

    m_ListSet.at(index).Clear();
    m_ListSet.at(index).FromString(s);
}


int Pattern::NewList()
{
    m_ListSet.emplace_back();
    m_PosEdit = m_ListSet.size() - 1;
    return m_PosEdit;
}

void Pattern::ReplaceList(NoteList & noteList)
{
    if ( m_PosEdit >= m_ListSet.size() )
        m_ListSet.resize(m_PosEdit + 1);

    m_ListSet.at(m_PosEdit) = noteList;
}


string Pattern::Label(size_t width)
{
    char buf[100];
    char format[20];

    if ( width > 80 )                           // Make sure we don't overflow buffer.
        width = 80;

    if ( m_Label.size() > width - 2 )                    // Allow for quotes.
        sprintf(format, "\'%%.%is...\'", width - 5);    // Allow for quotes and ellipsis.
    else
        strcpy(format, "\'%s\'");

    sprintf(buf, format, m_Label.c_str());

    return buf;
}


void Pattern::DeleteCurrentList()
{
    if ( m_ListSet.empty() )
        return;

    m_ListSet.erase(m_ListSet.begin() + m_PosEdit);

    if ( m_ListSet.empty() )
    {
        m_PosEdit = 0;
        m_Pos = 0;
        return;
    }

    // If the play pointer is above the pattern that was deleted,
    // move it down to keep it with the pattern it points at.
    //
    // Or, if the play pointer was pointing at the last pattern in
    // the list and that was deleted, it needs to point to the item
    // that's now at the end of the list.
    //
    // (If it was pointing at the pattern that was deleted, it now
    // points to the one that took its place.)

    if ( m_Pos > m_PosEdit || m_Pos == m_ListSet.size() )
        m_Pos -= 1;

    // The edit pointer stays in place and now points to next in
    // list (unless it was already at the end of the list).

    if ( m_PosEdit == m_ListSet.size() )
        m_PosEdit -= 1;

}

void Pattern::StoreStepValue( double val )
{
    m_StepValue = val;
}

void Pattern::StoreGate( double gate )
{
    m_Gate = gate;
}

void Pattern::StoreGateHold( bool val )
{
    m_GateHold = val;
}

void Pattern::StoreVelocity( unsigned char vel )
{
    m_Velocity = vel;
}

void Pattern::StoreTranslateTable( TranslateTable & table )
{
    m_TranslateTable = table;
}

