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

#include "maListBuilder.h"
#include "maPattern.h"

#include "MidiFile.h"


#include <cstring>
#include <unordered_map>

#include "maUtility.h"

using namespace std;

void Pattern::Step(Cluster & cluster, TrigRepeater & repeater,
    double & stepValueMultiplier, double phase, double stepValue, double globalBeat)
{
    // Add in step based events, if any, and step position.

    if ( ! m_StepListSet.empty() )
    {
        int loopCheck = 0;

        if ( m_TrigList.Empty() )
        {
            if ( m_Pos >= m_StepListSet.size() )
                m_Pos = 0;

            m_LastRequestedPos = m_Pos;

            Cluster * result = m_StepListSet[m_Pos++].Step();

            if ( result != NULL )
            {
                cluster.SetStepsTillNextNote(result->StepsTillNextNote());
                cluster += *result;
            }
        }
        else while ( loopCheck < m_TrigList.Size() ) // Loop only if skipping entries.
        {
            TrigListItem * trigItem = m_TrigList.Step();

            if ( trigItem->Skip() )
            {
                loopCheck += 1;
                continue;
            }

            if ( trigItem->Mute() )
                break;

            stepValueMultiplier = trigItem->Multiplier();

            repeater = trigItem->Repeater();

            for ( vector<int>::iterator it = trigItem->Trigs().begin(); it < trigItem->Trigs().end(); it++ )
            {
                // Just ignore list number if list doesn't exist.

                if ( *it < m_StepListSet.size() )
                {
                    m_LastRequestedPos = *it;

                    Cluster * result = m_StepListSet.at(*it).Step();

                    if ( result != NULL )
                    {
                        cluster.SetStepsTillNextNote(result->StepsTillNextNote());
                        cluster += *result;
                    }
                }
            }

            break;
        }
    }

    // All notes collected so far are from step lists, so they won't have any
    // phase start times set. Fix that now ...

    cluster.SetPhaseAllNotes(phase);

    // Collect any real time events.

    if ( ! m_RealTimeSet.empty() )
    {
        for ( vector<RealTimeList>::iterator it = m_RealTimeSet.begin(); it < m_RealTimeSet.end(); it++ )
            it->Step2(cluster, phase, stepValue, m_RealTimeBeat);

        if ( lround(1000.0 * m_RealTimeBeat) > 0 )
        {
            m_RealTimeComplete = true;
            for ( auto it = m_RealTimeSet.begin(); it != m_RealTimeSet.end(); it++ )
                m_RealTimeComplete &= it->PhazeIsZero();
            if ( m_RealTimeComplete )
                m_RealTimeBeat = 0;
        }
    }

    m_LastRealTimeBeat = m_RealTimeBeat;
    m_RealTimeBeat += 4.0 / stepValue;
}

bool Pattern::AllListsComplete()
{
    bool bResult = true;

    if ( ! m_RealTimeComplete && ! m_RealTimeSet.empty() )
        return false;

    for ( auto it = m_StepListSet.begin(); it != m_StepListSet.end(); it++ )
        bResult &= it->Complete();


    return bResult;
}

string Pattern::TrigsToStringForDisplay()
{
    return m_TrigList.ToStringForDisplay();
}

enum pat_element_names_t
{
    pat_heading,
    pat_name_step_value,
    pat_name_gate,
    pat_name_gate_hold,
    pat_name_velocity,
    pat_name_label,
    num_pat_element_names
};


unordered_map<pat_element_names_t, const char *> pat_element_names = {
    {pat_heading, "Pattern"},
    {pat_name_step_value, "Step Value"},
    {pat_name_gate, "Gate"},
    {pat_name_gate_hold, "Hold"},
    {pat_name_label, "Label"},
    {pat_name_velocity, "Vel"}
};



string Pattern::ToString(const char * prefix)
{
    string result;

    if ( prefix != NULL )
    {
        result += prefix;
        result += ' ';
    }

    if ( !m_Label.empty() )
    {
        result += pat_element_names.at(pat_name_label);
        result += " \"";
        result += m_Label;
        result += "\" ";
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

    result += m_TranslateTable.ToString(prefix);
    result += "\n";

    result += m_FeelMap.ToString(prefix);
    result += "\n";

    result += m_TrigList.ToString(prefix);
    result += "\n";

    int index = 1;
    for ( vector<StepList>::iterator i = m_StepListSet.begin(); i != m_StepListSet.end(); i++, index++ )
    {
        char buffer[20];
        sprintf(buffer, "Step List %i ", index);
        result += buffer;
        result += (*i).ToString();
        result += "\n";
    }

    index = 1;
    for ( vector<RealTimeList>::iterator i = m_RealTimeSet.begin(); i != m_RealTimeSet.end(); i++, index++ )
    {
        char buffer[20];
        sprintf(buffer, "Real Time List %i ", index);
        result += buffer;
        result += (*i).ToString();
        result += "\n";
    }

    return result;
}

void Pattern::SetFieldsFromString(string s)
{
    for ( pat_element_names_t e = static_cast<pat_element_names_t>(1);
          e < num_pat_element_names;
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
        // This is now a klunky hack to allow for both 'Default' and 'Pattern' tags,
        // when initializing from a string.
        //
        // Fortunately, both tags are the same length so we can check for 'Scale',
        // 'Trigs' and 'Feel' at position 7 and route accordingly. We don't do this
        // for 'List' because the default pattern won't have any so all lists are
        // routed to the current member of the pattern list. The final else
        // clause catches everything else.

        if ( s.find("List ") == 0 )
        {
            // Old tag, should remove this after a while.
            int index = stoi(s.substr(5)) - 1;
            size_t pos = s.find(' ', 5);
            if ( pos == string::npos )
                throw string("Pattern::FromString(), Step parameter list is empty.");
            AddListFromString(index, s.substr(pos));
            updates += 1;
            return true;
        }
        else if ( s.find("Step List ") == 0 )
        {
            int index = stoi(s.substr(10)) - 1;
            size_t pos = s.find(' ', 10);
            if ( pos == string::npos )
                throw string("Pattern::FromString(), Step parameter list is empty.");
            AddListFromString(index, s.substr(pos));
            updates += 1;
            return true;
        }
        else if ( s.find("Real Time List ") == 0 )
        {
            int index = stoi(s.substr(15)) - 1;
            size_t pos = s.find(' ', 15);
            if ( pos == string::npos )
                throw string("Pattern::FromString(), Real Time parameter list is empty.");
            AddRealTimeListFromString(index, s.substr(pos));
            updates += 1;
            return true;
        }
        else if ( s.find(" Trigs ") == 7 )
        {
            m_TrigList.FromString(s.substr(14));
            updates += 1;
            return true;
        }
        else if ( s.find(" Scale ") == 7 )
        {
            m_TranslateTable.FromString(s);
            updates += 1;
            return true;
        }
        else if ( s.find(" Feel ") == 7 )
        {
            m_FeelMap.FromString(s);
            updates += 1;
            return true;
        }
        else if ( s.find("Pattern ") == 0 || s.find("Default ") == 0 )
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

void Pattern::AddListFromString(vector<StepList>::size_type index, string s)
{
    if ( index < 0 )
        throw string("Pattern::AddListFromString(), invalid list index.");

    if ( index >= m_StepListSet.size() )
        m_StepListSet.resize(index + 1);

//    m_StepListSet.at(index).Clear();
    m_StepListSet.at(index).FromString(s);
}

void Pattern::AddRealTimeList(std::map<double,Note> realTimeList, double quantum)
{
    m_RealTimeSet.emplace_back(realTimeList, quantum);
}

void Pattern::AddRealTimeListFromString(vector<RealTimeList>::size_type index, string s)
{
    if ( index < 0 )
        throw string("Pattern::AddListFromString(), invalid list index.");

    if ( index >= m_RealTimeSet.size() )
        m_RealTimeSet.resize(index + 1);

    m_RealTimeSet.at(index).FromString(s);
}

void Pattern::AddRealTimeListFromMidiFile(string s)
{
    MidiFile midiFile;

    // Extract file name and add default path and extension if
    // not already present.

    s = s.substr(s.find(' ') + 1);
    if ( s.find('/') == string::npos )
        s.insert(0, "Midi Files/");
    if ( s.find(".mid") == string::npos )
        s += ".mid";

    if ( midiFile.read(s.c_str()) == 0 )
        throw string("Couldn't open the file.");

    int importedTracks = 0;
    int tracks = midiFile.getTrackCount();
    int ppq = midiFile.getTicksPerQuarterNote();

    double captureQuantum = 0;

    for ( int track = 0; track < tracks; track++ )
    {
        ListBuilder builder;
        builder.SetMidiInputMode(MIDI_INPUT_FILE);

        for (int event = 0; event < midiFile[track].size(); event++ )
        {
            double beat = static_cast<double>(midiFile[track][event].tick)/ppq;
            snd_seq_event_t ev = {0};

            if ( midiFile[track][event].isNoteOn() )
                ev.type = SND_SEQ_EVENT_NOTEON;
            else if ( midiFile[track][event].isNoteOff() )
                ev.type = SND_SEQ_EVENT_NOTEOFF;
            else
                continue;

            ev.data.note.note = midiFile[track][event][1];
            ev.data.note.velocity = midiFile[track][event][2];

            builder.HandleMidi(&ev, beat);

            if ( beat > captureQuantum )
            {
                captureQuantum = ceil(beat);
            }
        }

        if ( ! builder.RealTimeList().empty() )
        {
            m_RealTimeSet.emplace_back(builder.RealTimeList());
            m_PosRealTimeEdit = m_RealTimeSet.size() - 1;
            importedTracks += 1;
        }
    }

    // TODO: This is fine if we're loading a pattern from empty, but is this
    //       OK if there are already lists in the pattern?

    for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList ++ )
        rtList->RaiseQuantumAtCapture(captureQuantum);

    if ( importedTracks == 0 )
        throw string("Something went wrong, nothing imported.");
}

void Pattern::SetRealTimeMultipliers(vector<string>::iterator token, vector<string>::iterator end)
{
    if ( m_RealTimeSet.empty() )
        throw string("Pattern::SetRealTimeMultipliers() - No RealTime lists present.");

    double rate = 1.0;
    double increment = 0.0;

    try
    {
        // Token should point to rate.

        rate = stod(token->c_str());

        // Now look for optional step increment.

        if ( ++token != end )
            increment = stod(token->c_str());
    }
    catch (...)
    {
        throw string("Pattern::SetRealTimeMultipliers() - There's something wrong with the parameter list.");
    }

    for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList++, rate += increment)
        rtList->SetMultiplier(rate);
}

void Pattern::SetRealTimeQuantum(string & token)
{
    if ( m_RealTimeSet.empty() )
        throw string("Pattern::SetRealTimeQuantum() - No RealTime lists present.");

    double quantum;

    try
    {
        quantum = stod(token.c_str());
    }
    catch (...)
    {
        throw string("Pattern::SetRealTimeQuantum() - There's something wrong with the parameter list.");
    }

    for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList++)
        rtList->SetQuantum(quantum);
}


void Pattern::SetRealTimeStartPhase(string & token)
{
    if ( m_RealTimeSet.empty() )
        throw string("Pattern::SetRealTimeStartPhase() - No RealTime lists present.");

    double phase;

    try
    {
        phase = stod(token.c_str())/100;
    }
    catch (...)
    {
        throw string("Pattern::SetRealTimePhase() - There's something wrong with the parameter list.");
    }

    // Calculate an overall pattern phase length using current list phaselengths.

    vector<int64_t> phaseLengths;

    for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList++)
        phaseLengths.push_back(rtList->PhaseLength());

    int64_t patternPhaseLength = std::accumulate(phaseLengths.begin(), phaseLengths.end(), 1, lcm);

    // Convert to a start beat value and reset all positions. Subsequent resets will also use this value.

    m_RealTimeBeatStart = phase * static_cast<double>(patternPhaseLength)/1000;
    ResetPosition();
}

enum rt_echo_parameter_t
{
    rte_increment,
    rte_interval,
    rte_target,
    rte_num_parameters
};

unordered_map<string, rt_echo_parameter_t> rt_echo_parameter_lookup =
{
    {"increment", rte_increment},
    {"inc", rte_increment},
    {"interval", rte_interval},
    {"int", rte_interval},
    {"target", rte_target},
    {"t", rte_target}
};


void Pattern::StartRealTimeEcho(vector<string>::iterator token, vector<string>::iterator last)
{
    if ( m_RealTimeSet.empty() )
        throw string("Pattern::StartRealTimeEcho() - No RealTime lists present.");

    double inc = 0;
    double target = 0;
    int interval = 0;

    try
    {
        for(; token != last; token++ )
        {
            switch(rt_echo_parameter_lookup.at(token->c_str()))
            {
            case rte_increment:
                inc = stod(*(++token));
                break;
            case rte_target:
                target = stod(*(++token));
                break;
            case rte_interval:
                interval = stoi(*(++token));
                break;
            default:
                break;
            }
        }
    }
    catch (...)
    {
        throw string("Pattern::StartRealTimeEcho() - There's something wrong with the parameter list.");
    }

    if ( equals_3(inc, 0) )
        throw string("Increment must be set for other parameters to have and effect.");

    auto rtList = m_RealTimeSet.begin() + m_PosEdit;

    if ( rtList != m_RealTimeSet.end() )
        rtList->BeginEcho(inc, target, interval);
}

int Pattern::NewList()
{
    m_StepListSet.emplace_back();
    m_PosEdit = m_StepListSet.size() - 1;
    return m_PosEdit;
}

void Pattern::ReplaceList(StepList & noteList)
{
    if ( m_PosEdit >= m_StepListSet.size() )
        m_StepListSet.resize(m_PosEdit + 1);

    m_StepListSet.at(m_PosEdit) = noteList;
}


string Pattern::Label(size_t width)
{
    char buf[100];
    char format[20];

    if ( width > 80 )                           // Make sure we don't overflow buffer.
        width = 80;

    if ( m_Label.size() > width - 2 )                    // Allow for quotes.
        sprintf(format, "%%.%lus...", width - 5);    // Allow for quotes and ellipsis.
    else
        strcpy(format, "%s");

    sprintf(buf, format, m_Label.c_str());

    return buf;
}


void Pattern::DeleteCurrentList()
{
    if ( m_StepListSet.empty() )
        return;

    m_StepListSet.erase(m_StepListSet.begin() + m_PosEdit);

    if ( m_StepListSet.empty() )
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

    if ( m_Pos > m_PosEdit || m_Pos == m_StepListSet.size() )
        m_Pos -= 1;

    // The edit pointer stays in place and now points to next in
    // list (unless it was already at the end of the list).

    if ( m_PosEdit == m_StepListSet.size() )
        m_PosEdit -= 1;

}

void Pattern::DeleteCurrentRealTimeList()
{
    if ( m_RealTimeSet.empty() )
        return;

    m_RealTimeSet.erase(m_RealTimeSet.begin() + m_PosRealTimeEdit);

    if ( m_RealTimeSet.empty() )
    {
        m_PosRealTimeEdit = 0;
        return;
    }

    // The edit pointer stays in place and now points to next in
    // list (unless it was already at the end of the list).

    if ( m_PosRealTimeEdit == m_RealTimeSet.size() )
        m_PosRealTimeEdit -= 1;

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

string & Centre(string & line, int centre, int width, int & offset)
{
    size_t p = line.find('|');

    if ( p < string::npos )
        line.at(p) = ' ';

    if ( p < centre )
    {
        line.insert(0, centre - p, ' ');
        offset += centre - p;
    }
    else if ( p > centre )
    {
        line.erase(0, p - centre);
        offset -= p - centre;
    }

    if ( line.size() > width )
        line = line.substr(0, width);
    line += '\n';

    return line;
}

string Pattern::Display(int mode, vector<PosInfo2> & highlights, int centre, int width, int displayRows)
{
    int offset, length;
    int row = 1;

    string result;
    string line;

    centre -= 4;
    width -= 4;     // Allow for edit cursors in left column.

    // Trigs

    result = "\n    ";
    if ( ! m_TrigList.Empty() )
    {
        result += m_TrigList.ToStringForDisplay2(offset, length, width);
        highlights.push_back(PosInfo2(row, offset + 4, length));
        result += '\n';
    }
    else
        result += "Triggers: Auto\n";

    result += '\n';
    row += 2;

    // How many list rows can we show.

    displayRows -= row;

    int limitStep = m_StepListSet.size();
    int limitRealTime = m_RealTimeSet.size();

    if ( limitStep > 0 && limitRealTime > 0 )
        displayRows -= 1; // Allow for spacer row.

    while ( limitStep + limitRealTime > displayRows )
    {
        if ( limitRealTime >= limitStep )
            limitRealTime -= 1;
        else
            limitStep -= 1;
    }

    // Step Lists

    while ( m_DisplayStartStep > m_PosEdit )
        m_DisplayStartStep -= 1;

    while ( m_DisplayStartStep + limitStep <= m_PosEdit )
        m_DisplayStartStep += 1;

    for ( int i = m_DisplayStartStep; i < m_DisplayStartStep + limitStep; i++ )
    {
        if ( i == m_PosEdit )
        {
            result += " -> ";
            highlights.push_back(PosInfo2(row, 1, 2));
        }
        else
            result += "    ";
        switch(mode)
        {
        case 1:
            highlights.push_back(PosInfo2(row, centre, 4));
            line = m_StepListSet.at(i).ToStringForDisplay(offset, length);
            result += Centre(line, centre, width, offset);
            highlights.push_back(PosInfo2(row++, offset + 4, length));
            break;
        case 2:
            line = m_StepListSet.at(i).ToStringForDisplay2(offset, length, width);
            highlights.push_back(PosInfo2(row++, offset + 4, length));
            line += '\n';
            result += line;
            break;
        default:
            break;
        }
    }

    // Add spacer if both kinds of list are present.

    if ( limitStep > 0 && limitRealTime > 0 )
    {
        result += '\n';
        row += 1;
    }

    // Realtime Lists

    while ( m_DisplayStartRealTime > m_PosRealTimeEdit )
        m_DisplayStartRealTime -= 1;

    while ( m_DisplayStartRealTime + limitRealTime <= m_PosRealTimeEdit )
        m_DisplayStartRealTime += 1;

    for ( int i = m_DisplayStartRealTime; i < m_DisplayStartRealTime + limitRealTime; i++ )
    {
        if ( i == m_PosRealTimeEdit )
        {
            result += " -> ";
            highlights.push_back(PosInfo2(row, 1, 2));
        }
        else
            result += "    ";
        highlights.push_back(PosInfo2(row, 4, 5));
        result += m_RealTimeSet.at(i).ToStringForDisplay(offset, length);
        highlights.push_back(PosInfo2(row++, offset + 4, length));
        result += '\n';
    }

    return result;
}
