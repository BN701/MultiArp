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

#include <cmath>
#include <cstring>
#include <numeric>
//#include <unordered_map>

#ifdef MA_BLUE
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#else
#include "MidiFile.h"
#endif // MA_BLUE

#include "maListBuilder.h"  // Listbuilder is used for midi file import.
#include "maPattern.h"
#include "maState.h"
#include "maUtility.h"

using namespace std;

int Pattern::m_PatternCount = 0;
unordered_set<std::string> Pattern::m_ShortLabels;

Pattern::Pattern():
    m_MenuList(this, &m_Visible)
{
//    m_TestString = "Set from Pattern Constructor.";

    m_PopUpMenuID = C_MENU_ID_PATTERN;
    m_DisplayObjectType = BaseUI::dot_pattern;
    m_MenuList.Add(this);
    m_MenuListIndent = 0;
    m_MenuList.m_DisplayObjectType = BaseUI::dot_pattern_menu_list;
}

Pattern::~Pattern()
{
    for ( auto it = m_ListGroups.begin(); it != m_ListGroups.end(); it++ )
        delete *it;

    ItemMenu::m_MenuListPtr = NULL; // Prevent ItemMenu destructor from attempting
                                    // to remove ourselves from the menu list that
                                    // will already have been cleared.
}

//Pattern::Pattern(const Pattern & p):
//    ItemMenu(p),
////    m_Pos(p.m_Pos),
//    m_Label(p.m_Label),
//    m_ShortLabel(p.m_ShortLabel),
//    m_ShortLabelHash(p.m_ShortLabelHash),
//    m_StepValue(p.m_StepValue),
//    m_Gate(p.m_Gate),
//    m_GateHold(p.m_GateHold),
//    m_Velocity(p.m_Velocity),
//    m_MenuList(this, &m_Visible)    // Initialized, not copied
//{
//    m_Visible = false;
//    m_MenuList.m_DisplayObjectType = p.m_MenuList.m_DisplayObjectType;
//
//    m_MenuList.Add(this);
//
//    // This is a copy constructor, so we have to copy each List Group
//
//    CopyContent(p);
//
//}

Pattern::Pattern(const Pattern & p):
    m_MenuList(this, &m_Visible)    // Initialized, not copied
{
    m_MenuList.m_DisplayObjectType = p.m_MenuList.m_DisplayObjectType;
    m_MenuList.Add(this);

    *this = p;
}

//Pattern & Pattern::operator = (const Pattern & p)
Pattern & Pattern::operator = (const ItemMenu & m)
{
    ItemMenu::operator = (m);

#if defined(MA_BLUE)
    const Pattern & p = *static_cast<const Pattern*>(&m);
#else
    const Pattern & p = *dynamic_cast<const Pattern*>(&m);
#endif
    m_Visible = p.m_Visible;
    m_Label = p.m_Label;
    m_ShortLabel = p.m_ShortLabel;      // If genuine copy, need to call SetShortLabel() again to generate unique ID.
    m_ShortLabelHash = p.m_ShortLabelHash;  // Not sure about the validity of doing this, given the above.
    m_StepValue = p.m_StepValue;
    m_Gate = p.m_Gate;
    m_GateHold = p.m_GateHold;
    m_Velocity = p.m_Velocity;

    CopyContent(p);

    return *this;
}

//Pattern & Pattern::ExplicitCopy(const Pattern & p)
//{
//    ItemMenu::ExplicitCopy(p);
//
////    m_Pos = p.m_Pos;
//    m_Label = p.m_Label;
//    m_ShortLabel = p.m_ShortLabel;      // If genuine copy, need to call SetShortLabel() again to generate unique ID.
//    m_ShortLabelHash = p.m_ShortLabelHash;  // Not sure about the validity of doing this, given the above.
//    m_StepValue = p.m_StepValue;
//    m_Gate = p.m_Gate;
//    m_GateHold = p.m_GateHold;
//    m_Velocity = p.m_Velocity;
//
//    CopyContent(p);
//
//    return *this;
//}

void Pattern::CopyContent(const Pattern & p)
{
    for ( auto it = p.m_ListGroups.begin(); it != p.m_ListGroups.end(); it++ )
    {
        switch( (*it)->Type() )
        {
            case ListGroup::lgtype_step:
                m_ListGroups.emplace_back(new StepListGroup(this));
                static_cast<StepListGroup*>(m_ListGroups.back())->ExplicitCopy(*static_cast<StepListGroup*>(*it));
                break;
            case ListGroup::lgtype_realtime:
                m_ListGroups.emplace_back(new RTListGroup(this));
                static_cast<RTListGroup*>(m_ListGroups.back())->ExplicitCopy(*static_cast<RTListGroup*>(*it));
                break;
        }

        ListGroup * lgNew = m_ListGroups.back();
        if ( lgNew != NULL )
        {
//            lgNew->SetParent(this);
            lgNew->SetVisible(m_Visible);
            lgNew->AddToMenuList(m_MenuList);
        }
    }

    // Copy the menu list cursor position, too.

    if ( !m_MenuList.m_Items.empty() )
    {
        m_MenuList.m_Cursor = m_MenuList.m_Items.begin();
        for ( auto it = p.m_MenuList.m_Items.begin(); it != p.m_MenuList.m_Cursor; it++)
            m_MenuList.m_Cursor++;
    }
}


//void Pattern::ResetMenuList()
//{
////    ItemMenu::m_MenuListPtr = & m_MenuList;
////    *(m_MenuList.m_Items.begin()) = this;
////    m_MenuList.m_Cursor = m_MenuList.m_Items.begin();
//
//    m_MenuList.m_Items.clear();
//
//    m_MenuList.Add(this);
//
//    for ( auto it = m_ListGroups.begin(); it != m_ListGroups.end(); it++ )
//        (*it)->AddToMenuList(m_MenuList);
//
//    // Copy the menu list cursor position, too.
//
//    if ( m_MenuList.m_LastCursorPos >= 0 )
//    {
//        m_MenuList.m_Cursor = m_MenuList.m_Items.begin();
//        for ( int pos = 0; pos < m_MenuList.m_LastCursorPos;  pos++)
//            m_MenuList.m_Cursor++;
//    }
//    else
//        m_MenuList.m_Cursor = m_MenuList.m_Items.end();
//}

int Pattern::SetPatternID()
{
    // Note that this is *not* a unique ID.
    m_PatternID = ++m_PatternCount;
    return m_PatternID;
}

void Pattern::Clear()
{
    m_Label.clear();
    while ( ! m_ListGroups.empty() )
    {
        delete m_ListGroups.back();
        m_ListGroups.pop_back();
    }
    ResetPosition();
    m_TranslateTable.Reset();
//    m_TrigList.Clear();
    m_StepValue = 4.0;
    m_Gate = 0.5;
    m_GateHold = false;
    m_Velocity = 64;
}


void Pattern::SetLabel(string label)
{
    static const char * numbers[] =
    {
        "Zero",
        "One",
        "Two",
        "Three",
        "Four",
        "Five",
        "Six",
        "Seven",
        "Eight",
        "Nine"
    };

    if ( label.empty() )
    {
        // Automatic label name.
        int n = m_PatternID;
        while ( n > 0 )
        {
            label.insert(0, numbers[n % 10]);
            n /= 10;
        }
    }
    m_Label = label;
    SetRedraw();
}

std::hash<std::string> str_hash;

void Pattern::SetShortLabel()
{
    if ( m_ShortLabel.empty() )
    {
        char buff[10];
        snprintf(buff, 10, "P%02i", SetPatternID());
        m_ShortLabel = buff;
    }
    else
    {
        // Add (and increment) an alphabetic suffix.
        char c = m_ShortLabel.back();
        if ( isdigit(c) )
            c = 'a';
        else
            m_ShortLabel.pop_back();
        while ( true )
        {
            if ( c < 'z' )
            {
                m_ShortLabel += ++c;
                auto result = m_ShortLabels.insert(m_ShortLabel);
                if ( result.second )
                    break;
                else
                    m_ShortLabel.pop_back();
            }
            else
            {
                // Give up and generate a fresh short label.
                m_ShortLabel.clear();
                SetShortLabel();
                break;
            }
        }
    }

    m_ShortLabelHash = str_hash(m_ShortLabel);
}

void Pattern::SetRedraw() noexcept
{
    if ( m_Visible )
    {
        ItemMenu::SetRedraw();
        for ( auto it = m_MenuList.m_Items.begin(); it != m_MenuList.m_Items.end(); it++ )
            if ( *it != this )  // Avoid recursive call back to Pattern::SetRedraw()!
            {
//                Pattern & debugPattern = *dynamic_cast<Pattern*>(*it);
                (*it)->SetRedraw();
            }
    }
}

void Pattern::SetVisible(bool val)
{
    ItemMenu::SetVisible(val);
    for ( auto it = m_ListGroups.begin(); it != m_ListGroups.end(); it++ )
        (*it)->SetVisible(val);
}

void Pattern::ResetPosition()
{
//    m_Pos = 0;
//    m_TrigList.ResetPosition();

    for ( auto group = m_ListGroups.begin(); group != m_ListGroups.end(); group++ )
        (*group)->ResetPosition();

    m_RealTimeBeat = m_RealTimeBeatStart;
    m_RealTimeComplete = false;
//        for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList++ )
//            rtList->ResetPosition();
}


//void Pattern::Step(/*Cluster & cluster, TrigRepeater & repeater,*/
//    double & stepValueMultiplier, double phase, double stepValue, double globalBeat)
//{
//    // Add in step based events, if any, and step position.
//
//// TODO:LG
////    if ( ! m_StepListSet.empty() )
////    {
////        unsigned loopCheck = 0;
////
////        if ( m_TrigList.Empty() )
////        {
////            if ( m_Pos >= m_StepListSet.size() )
////                m_Pos = 0;
////
////            m_LastRequestedPos = m_Pos;
////
////            Cluster * result = m_StepListSet[m_Pos++].Step();
////
////            if ( result != NULL )
////            {
////                cluster.SetStepsTillNextNote(result->StepsTillNextNote());
////                cluster += *result;
////            }
////        }
////        else while ( loopCheck < m_TrigList.Size() ) // Loop only if skipping entries.
////        {
////            TrigListItem * trigItem = m_TrigList.Step();
////
////            if ( trigItem->Skip() )
////            {
////                loopCheck += 1;
////                continue;
////            }
////
////            if ( trigItem->Mute() )
////                break;
////
////            stepValueMultiplier = trigItem->Multiplier();
////
////            repeater = trigItem->Repeater();
////
////            for ( vector<int>::iterator it = trigItem->Trigs().begin(); it < trigItem->Trigs().end(); it++ )
////            {
////                // Just ignore list number if list doesn't exist.
////
////                if ( static_cast<unsigned>(*it) < m_StepListSet.size() )
////                {
////                    m_LastRequestedPos = *it;
////
////                    Cluster * result = m_StepListSet.at(*it).Step();
////
////                    if ( result != NULL )
////                    {
////                        cluster.SetStepsTillNextNote(result->StepsTillNextNote());
////                        cluster += *result;
////                    }
////                }
////            }
////
////            break;
////        }
////    }
////
////    // All notes collected so far are from step lists, so they won't have any
////    // phase start times set. Fix that now ...
////
////    cluster.SetPhaseAllNotes(phase);
////
////    // Collect any real time events.
////
////    if ( ! m_RealTimeSet.empty() )
////    {
////        for ( vector<RealTimeList>::iterator it = m_RealTimeSet.begin(); it < m_RealTimeSet.end(); it++ )
////            it->Step2(cluster, phase, stepValue, m_RealTimeBeat);
////
////        if ( lround(1000.0 * m_RealTimeBeat) > 0 )
////        {
////            m_RealTimeComplete = true;
////            for ( auto it = m_RealTimeSet.begin(); it != m_RealTimeSet.end(); it++ )
////                m_RealTimeComplete &= it->PhazeIsZero();
////            if ( m_RealTimeComplete )
////                m_RealTimeBeat = 0;
////        }
////    }
////
////    m_LastRealTimeBeat = m_RealTimeBeat;
////    m_RealTimeBeat += 4.0 / stepValue;
//}

string Pattern::StepListManager(command_t command)
{
    switch ( command )
    {
        case C_LIST_NEW:
        {
            StepListGroup * pGroup = NULL;

            // Get the current group.

            ItemMenu * pItem = m_MenuList.CurrentItem();
            if ( pItem->CheckType(BaseUI::dot_step_list) )
            {
                menu_list_cursor_t pos = m_MenuList.ReverseFind(BaseUI::dot_step_list_group);
                if ( pos != m_MenuList.m_Items.end() )
#if defined(MA_BLUE)
                    pGroup = static_cast<StepListGroup*>(*pos);
#else
                    pGroup = dynamic_cast<StepListGroup*>(*pos);
#endif
            }
            else
#if defined(MA_BLUE)
                pGroup = static_cast<StepListGroup*>(pItem);
#else
                pGroup = dynamic_cast<StepListGroup*>(pItem);
#endif
            if ( pGroup == NULL )
                return "Pattern Step List Manager: Not a step list or step list group.";

            StepList *pStepList = pGroup->NewStepList();
            pStepList->SetVisible(m_Visible);
            menu_list_cursor_t pos = m_MenuList.FindFirstNonMatching(BaseUI::dot_step_list);
            m_MenuList.Insert(pos, pStepList, true);  // Automatically select.
            break;
        }

        case C_LIST_MOVE_UP:
        case C_LIST_MOVE_DOWN:
        case C_LIST_COPY:
        case C_LIST_DELETE:
        {
            // Make sure we have a step list pointer.

#if defined(MA_BLUE)
            if ( ! m_MenuList.CurrentItem()->CheckType(BaseUI::dot_step_list) )
                return "Pattern Step List Manager: Not a step list.";

            StepList * pStepList = static_cast<StepList*>(m_MenuList.CurrentItem());

            StepListGroup * pGroup = NULL;

            menu_list_cursor_t pos = m_MenuList.ReverseFind(BaseUI::dot_step_list_group);
            if ( pos == m_MenuList.m_Items.end() || ! (*pos)->CheckType(BaseUI::dot_step_list_group))
                return "Pattern Step List Manager: Can't find containing step list group.";

            pGroup = static_cast<StepListGroup*>(*pos);
#else
            StepList * pStepList = dynamic_cast<StepList*>(m_MenuList.CurrentItem());
            if ( pStepList == NULL )
                return "Pattern Step List Manager: Not a step list.";
            // Get the current group.

            StepListGroup * pGroup = NULL;

            menu_list_cursor_t pos = m_MenuList.ReverseFind(BaseUI::dot_step_list_group);
            if ( pos != m_MenuList.m_Items.end() )
                pGroup = dynamic_cast<StepListGroup*>(*pos);

            if ( pGroup == NULL )
                return "Pattern Step List Manager: Can't find containing step list group.";
#endif

            switch ( command )
            {
                case C_LIST_MOVE_UP:
                    pGroup->MoveListUp(pStepList, m_MenuList);
                    break;
                case C_LIST_MOVE_DOWN:
                    pGroup->MoveListDown(pStepList, m_MenuList);
                    break;
                case C_LIST_COPY:
                    pGroup->CopyList(pStepList, m_MenuList);
                    break;
                case C_LIST_DELETE:
//                    m_MenuList.Select(pGroup->MenuPos());
                    pGroup->DeleteList(pStepList, m_MenuList);
                    break;
                default:
                    break;
            }
        }

        case C_STEP_INSERT_LEFT:
        case C_STEP_INSERT_RIGHT:
        case C_STEP_COPY_LEFT:
        case C_STEP_COPY_RIGHT:
        case C_STEP_DELETE:
        {
#if defined(MA_BLUE)
            if ( !m_MenuList.CurrentItem()->CheckType(BaseUI::dot_step_list) )
                return "Pattern Step List Manager: Not a step list.";
            StepList * pStepList = static_cast<StepList*>(m_MenuList.CurrentItem());
#else
            StepList * pStepList = dynamic_cast<StepList*>(m_MenuList.CurrentItem());
            if ( pStepList == NULL )
                return "Pattern Step List Manager: Not a step list.";
#endif
            switch (command)
            {
                case C_STEP_INSERT_LEFT:
                    pStepList->InsertLeft();
                    break;
                case C_STEP_INSERT_RIGHT:
                    pStepList->InsertRight();
                    break;
                case C_STEP_COPY_LEFT:
                    pStepList->CopyLeft();
                    break;
                case C_STEP_COPY_RIGHT:
                    pStepList->CopyRight();
                    break;
                case C_STEP_DELETE:
                    pStepList->DeleteStep();
                    break;
                default:
                    break;
            }
            pStepList->SetRedraw();
            break;
        }
        case C_CLUSTER_INSERT_LEFT:
        case C_CLUSTER_INSERT_RIGHT:
        case C_CLUSTER_COPY_LEFT:
        case C_CLUSTER_COPY_RIGHT:
        case C_CLUSTER_DELETE:
        {
#if defined(MA_BLUE)
            if ( !m_MenuList.CurrentItem()->CheckType(BaseUI::dot_cluster) )
                return "Pattern Step List Manager: Not a step list step (Cluster).";
            Cluster * pCluster = static_cast<Cluster*>(m_MenuList.CurrentItem());
#else
            Cluster * pCluster = dynamic_cast<Cluster*>(m_MenuList.CurrentItem());
            if ( pCluster == NULL )
                return "Pattern Step List Manager: Not a step list step (Cluster).";
#endif
            switch ( command )
            {
                case C_CLUSTER_INSERT_LEFT:
                    pCluster->InsertLeft();
                    break;
                case C_CLUSTER_INSERT_RIGHT:
                    pCluster->InsertRight();
                    break;
                case C_CLUSTER_COPY_LEFT:
                    pCluster->CopyLeft();
                    break;
                case C_CLUSTER_COPY_RIGHT:
                    pCluster->CopyRight();
                    break;
                case C_CLUSTER_DELETE:
                    pCluster->DeleteNote();
                    break;
                default:
                    break;
            }
            pCluster->SetRedraw();
            break;
        }

        default:
            return "Pattern Step List Manager: Doesn't handle this command.";
    }

    return "";

}


bool Pattern::AllListsComplete()
{
    bool bResult = true;
// TODO:LG
//    if ( ! m_RealTimeComplete && ! m_RealTimeSet.empty() )
//        return false;

//    for ( auto it = m_StepListSet.begin(); it != m_StepListSet.end(); it++ )
//        bResult &= it->Complete();


    return bResult;
}

//string Pattern::TrigsToStringForDisplay()
//{
//    return m_TrigList.ToStringForDisplay();
//}

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

#if defined(MA_BLUE)
unordered_map<int, const char *> pat_element_names = {
#else
unordered_map<pat_element_names_t, const char *> pat_element_names = {
#endif
    {pat_heading, "Pattern"},
    {pat_name_step_value, "Step Value"},
    {pat_name_gate, "Gate"},
    {pat_name_gate_hold, "Hold"},
    {pat_name_label, "Label"},
    {pat_name_velocity, "Vel"}
};

void Pattern::SetStatus()
{
//    int pos = 0;
    char buff[50];

    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( m_GotFocus )
        snprintf(buff, 50, "[Pattern %s]", m_Label.c_str());
    else
        snprintf(buff, 50, " Pattern %s ", m_Label.c_str());

    InitStatus();
    m_Status += buff;

//    m_Status += " Midi: ";
//    pos = m_Status.size();
//    snprintf(buff, 50, "%02i", m_MidiChannel);
//    m_Status += buff;
//    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

//    m_Status += " Step: ";
//    pos = m_Status.size();
//    snprintf(buff, 50, "%05.2f", m_Step);
//    m_Status += buff;
//    m_FieldPositions.emplace_back(pos, static_cast<int>(m_Status.size() - pos));

//    if ( !m_FieldPositions.empty() )
//        m_Highlights.push_back(m_FieldPositions.at(m_PosEdit));
}

bool Pattern::HandleKey(BaseUI::key_command_t k)
{
    switch ( k )
    {
    case BaseUI::key_cmd_enter:
//        m_MenuList.OpenCurrentItem(this);
        break;

    case BaseUI::key_cmd_back:
        ReturnFocus();
        break;

    case BaseUI::key_cmd_left:
        break;

    case BaseUI::key_cmd_right:
        break;

    case BaseUI::key_cmd_up:
        m_MenuList.DownCursorPos();
        return true;

    case BaseUI::key_cmd_down:
        m_MenuList.UpCursorPos();
        return true;

//    case BaseUI::key_up:
//        DownCursorPos();
//        break;
//
//    case BaseUI::key_down:
//        UpCursorPos();
//        break;

//    case BaseUI::key_up:
//    case BaseUI::key_ctrl_up:
//        DownCursorPos();
//        m_MenuList.OpenCurrentItem();

////        if ( m_MenuList.m_Cursor == m_MenuPos )
////            m_MenuList.OpenCurrentItem(m_ReturnFocus);
////        else
////            m_MenuList.OpenCurrentItem(this);
        break;

//    case BaseUI::key_down:
//    case BaseUI::key_ctrl_down:
//        UpCursorPos();
//        m_MenuList.OpenCurrentItem();

////        if ( m_MenuList.m_Cursor == m_MenuPos )
////            m_MenuList.OpenCurrentItem(m_ReturnFocus);
////        else
////            m_MenuList.OpenCurrentItem(this);
        break;

    default:
        return false;
    }

//    SetStatus();

    return true;
}


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

    snprintf(buffer, 200, "%s %.2f", pat_element_names.at(pat_name_step_value), m_StepValue);
    result += buffer;

    if ( !result.empty() )
        result += ' ';
    snprintf(buffer, 200, "%s %.2f %s %s",
            pat_element_names.at(pat_name_gate), m_Gate,
            pat_element_names.at(pat_name_gate_hold), m_GateHold ? "ON" : "OFF");
    result += buffer;

    if ( !result.empty() )
        result += ' ';
    snprintf(buffer, 200, "%s %i", pat_element_names.at(pat_name_velocity), m_Velocity);
    result += buffer;
    result += "\n";

    result += m_TranslateTable.ToString(prefix);
    result += "\n";

    result += m_FeelMap.ToString(prefix);
    result += "\n";

//    result += m_TrigList.ToString(prefix);
//    result += "\n";

// TODO:LG
//    int index = 1;
//    for ( vector<StepList>::iterator i = m_StepListSet.begin(); i != m_StepListSet.end(); i++, index++ )
//    {
//        char buffer[20];
//        snprintf(buffer, 200, "Step List %i ", index);
//        result += buffer;
//        result += (*i).ToString();
//        result += "\n";
//    }
//
//    index = 1;
//    for ( vector<RealTimeList>::iterator i = m_RealTimeSet.begin(); i != m_RealTimeSet.end(); i++, index++ )
//    {
//        char buffer[20];
//        snprintf(buffer, 200, "Real Time List %i ", index);
//        result += buffer;
//        result += (*i).ToString();
//        result += "\n";
//    }

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

#ifndef MA_BLUE
        try
        {
#endif
            switch (e)
            {
            case pat_name_step_value:
                m_StepValue = strtod(token.c_str(), NULL);
                break;
            case pat_name_gate:
                m_Gate = strtod(token.c_str(), NULL);
                break;
            case pat_name_gate_hold:
                m_GateHold = token == "ON";
                break;
            case pat_name_velocity:
                m_Velocity = strtol(token.c_str(), NULL, 0);
                break;
            case pat_name_label:
                m_Label = token;
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
}


bool Pattern::FromString(string s, int & updates)
{
#ifndef MA_BLUE
    try
    {
#endif
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
            int index = strtol(s.substr(5).c_str(), NULL, 0) - 1;
            size_t pos = s.find(' ', 5);
            if ( pos == string::npos )
#ifdef MA_BLUE
                return false;
#else
                throw string("Pattern::FromString(), Step parameter list is empty.");
#endif
            AddStepListFromString(index, s.substr(pos));
            updates += 1;
            return true;
        }
        else if ( s.find("Step List ") == 0 )
        {
            int index = strtol(s.substr(10).c_str(), NULL, 0) - 1;
            size_t pos = s.find(' ', 10);
            if ( pos == string::npos )
#ifdef MA_BLUE
                return false;
#else
                throw string("Pattern::FromString(), Step parameter list is empty.");
#endif
            AddStepListFromString(index, s.substr(pos));
            updates += 1;
            return true;
        }
        else if ( s.find("Real Time List ") == 0 )
        {
            int index = strtol(s.substr(15).c_str(), NULL, 0) - 1;
            size_t pos = s.find(' ', 15);
            if ( pos == string::npos )
#ifdef MA_BLUE
                return false;
#else
                throw string("Pattern::FromString(), Real Time parameter list is empty.");
#endif
            AddRealTimeListFromString(index, s.substr(pos));
            updates += 1;
            return true;
        }
//        else if ( s.find(" Trigs ") == 7 )
//        {
//            m_TrigList.FromString(s.substr(14));
//            updates += 1;
//            return true;
//        }
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
#ifndef MA_BLUE
    }
    catch (invalid_argument ex)
    {
        throw string("Pattern::FromString(), failed to read a number somewhere in the parameter list.");
    }
#endif
}

bool Pattern::AddStepListFromString(vector<StepList>::size_type index, string s)
{
    if ( index < 0 )
#ifdef MA_BLUE
        return false;
#else
        throw string("Pattern::AddListFromString(), invalid list index.");
#endif
// TODO:LG
//    auto prevListSize = m_StepListSet.size();
//
//    if ( index >= m_StepListSet.size() )
//        m_StepListSet.resize(index + 1);
//
//    if ( m_StepListSet.at(index).StepListFromString(s) )
//        return true;
//
//    // If initialization fails and we end up with an empty list,
//    // set the list back to its previous size. (Destructors
//    // are called by this process.)
//
//    m_StepListSet.resize(prevListSize);
    return false;
}

void Pattern::AddRealTimeList(std::map<double,Note> realTimeList, double quantum)
{
// TODO:LG
//    m_RealTimeSet.emplace_back(realTimeList, quantum);
}

void Pattern::AddRealTimeListFromString(vector<RealTimeList>::size_type index, string s)
{
    if ( index < 0 )
#ifdef MA_BLUE
        return;
#else
        throw string("Pattern::AddListFromString(), invalid list index.");
#endif

// TODO:LG
//    if ( index >= m_RealTimeSet.size() )
//        m_RealTimeSet.resize(index + 1);
//
//    m_RealTimeSet.at(index).FromString(s);
}

#ifndef MA_BLUE
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
// TODO:LG
//            m_RealTimeSet.emplace_back(builder.RealTimeList());
//            m_PosRealTimeEdit = m_RealTimeSet.size() - 1;
            importedTracks += 1;
        }
    }

    // TODO: This is fine if we're loading a pattern from empty, but is this
    //       OK if there are already lists in the pattern?

// TODO:LG
//    for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList ++ )
//        rtList->RaiseQuantumAtCapture(captureQuantum);

    if ( importedTracks == 0 )
        throw string("Something went wrong, nothing imported.");
}
#endif

void Pattern::SetRealTimeMultipliers(vector<string>::iterator token, vector<string>::iterator end)
{
// TODO:LG
//    if ( m_RealTimeSet.empty() )
//#ifdef MA_BLUE
//        return;
//#else
//        throw string("Pattern::SetRealTimeMultipliers() - No RealTime lists present.");
//#endif
//
//    double rate = 1.0;
//    double increment = 0.0;
//
//#ifndef MA_BLUE
//    try
//    {
//#endif
//        // Token should point to rate.
//
//        rate = strtod(token->c_str(), NULL);
//
//        // Now look for optional step increment.
//
//        if ( ++token != end )
//            increment = strtod(token->c_str(), NULL);
//#ifndef MA_BLUE
//    }
//    catch (...)
//    {
//        throw string("Pattern::SetRealTimeMultipliers() - There's something wrong with the parameter list.");
//    }
//#endif
//
//    for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList++, rate += increment)
//        rtList->SetMultiplier(rate);
}

void Pattern::SetRealTimeQuantum(string & token)
{
// TODO:LG
//    if ( m_RealTimeSet.empty() )
//#ifdef MA_BLUE
//        return;
//#else
//        throw string("Pattern::SetRealTimeQuantum() - No RealTime lists present.");
//#endif
//
//    double quantum;
//
//#ifdef MA_BLUE
//    quantum = strtod(token.c_str(), NULL);
//    if ( errno != 0 )
//        return;
//#else
//    try
//    {
//        quantum = stod(token.c_str());
//    }
//    catch (...)
//    {
//        throw string("Pattern::SetRealTimeQuantum() - There's something wrong with the parameter list.");
//    }
//#endif
//
//    for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList++)
//        rtList->SetQuantum(quantum);
}


void Pattern::SetRealTimeStartPhase(string & token)
{
// TODO:LG
//    if ( m_RealTimeSet.empty() )
//#ifdef MA_BLUE
//        return;
//#else
//        throw string("Pattern::SetRealTimeStartPhase() - No RealTime lists present.");
//#endif
//
//    double phase;
//
//#ifdef MA_BLUE
//    phase = strtod(token.c_str(), NULL)/100;
//    if ( errno != 0 )
//        return;
//#else
//    try
//    {
//        phase = stod(token.c_str())/100;
//    }
//    catch (...)
//    {
//        throw string("Pattern::SetRealTimePhase() - There's something wrong with the parameter list.");
//    }
//#endif
//
//    // Calculate an overall pattern phase length using current list phaselengths.
//
//    vector<int64_t> phaseLengths;
//
//    for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList++)
//        phaseLengths.push_back(rtList->PhaseLength());
//
//    int64_t patternPhaseLength = std::accumulate(phaseLengths.begin(), phaseLengths.end(), 1, lcm);
//
//    // Convert to a start beat value and reset all positions. Subsequent resets will also use this value.
//
//    m_RealTimeBeatStart = phase * static_cast<double>(patternPhaseLength)/1000;
//    ResetPosition();
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
// TODO:LG
//    if ( m_RealTimeSet.empty() )
//#ifdef MA_BLUE
//        return;
//#else
//        throw string("Pattern::StartRealTimeEcho() - No RealTime lists present.");
//#endif
//
//    double inc = 0;
//    double target = 0;
//    int interval = 0;
//
//#ifndef MA_BLUE
//    try
//    {
//#endif
//        for(; token != last; token++ )
//        {
//            switch(rt_echo_parameter_lookup.at(token->c_str()))
//            {
//            case rte_increment:
////                inc = stod(*(++token));
//                inc = strtod((++token)->c_str(), NULL);
//                break;
//            case rte_target:
////                target = stod(*(++token));
//                target = strtod((++token)->c_str(), NULL);
//                break;
//            case rte_interval:
////                interval = stoi(*(++token));
//                interval = strtol((++token)->c_str(), NULL, 0);
//                break;
//            default:
//                break;
//            }
//        }
//#ifndef MA_BLUE
//    }
//    catch (...)
//    {
//        throw string("Pattern::StartRealTimeEcho() - There's something wrong with the parameter list.");
//    }
//#endif
//
//    if ( equals_3(inc, 0) )
//#ifdef MA_BLUE
//        return;
//#else
//        throw string("Increment must be set for other parameters to have and effect.");
//#endif
//
//    auto rtList = m_RealTimeSet.begin() + m_PosEdit;
//
//    if ( rtList != m_RealTimeSet.end() )
//        rtList->BeginEcho(inc, target, interval);
}

void Pattern::NewListGroup(ListGroup::list_group_type type)
{
//    m_StepListSet.emplace_back();
//    m_PosEdit = m_StepListSet.size() - 1;

    switch (type)
    {
        case ListGroup::lgtype_step:
            m_ListGroups.push_back(new StepListGroup(this));
            break;
        case ListGroup::lgtype_realtime:
            m_ListGroups.push_back(new RTListGroup(this));
            break;
    }

    ListGroup * pNewGroup = m_ListGroups.back();
    pNewGroup->SetReturnFocus(m_ReturnFocus);
    pNewGroup->SetItemID(m_ListGroups.size() - 1);
    pNewGroup->SetVisible(m_Visible);
    m_MenuList.Add(pNewGroup, true);    // Autoselect new item.

}

void Pattern::CopyCurrentListGroup()
{
    if ( !m_MenuList.CursorPosValid() )
        return;

    ItemMenu * pItem = *m_MenuList.m_Cursor;

    if ( ! pItem->CheckType(BaseUI::dot_step_list_group) && ! pItem->CheckType(BaseUI::dot_rt_list_group) )
        return;

    ListGroup * pGroup = static_cast<ListGroup*>(pItem);
    int pos = pGroup->ItemID() + 1;

    // Move focus elsewhere while we make copies.
    auto reselect = m_MenuList.ClearCursor();

    pGroup->RemoveListsFromMenu();
    auto menuInsertPos = m_MenuList.Remove(pGroup->MenuPos());

    ListGroup * pNewGroup = NULL;

    switch ( pGroup->Type() )
    {
        case ListGroup::lgtype_step:
            pNewGroup = *m_ListGroups.insert(m_ListGroups.begin() + pos, new StepListGroup(this));
            static_cast<StepListGroup*>(pNewGroup)->ExplicitCopy(*static_cast<StepListGroup*>(pGroup));
            break;
        case ListGroup::lgtype_realtime:
            pNewGroup = *m_ListGroups.insert(m_ListGroups.begin() + pos, new RTListGroup(this));
            static_cast<RTListGroup*>(pNewGroup)->ExplicitCopy(*static_cast<RTListGroup*>(pGroup));
            break;
    }

    for ( auto it = m_ListGroups.begin() + pos; it != m_ListGroups.end(); it++ )
        (*it)->SetItemID((*it)->ItemID() + 1);

    m_MenuList.Insert(menuInsertPos, pGroup);
    pGroup->InsertListsIntoMenu(menuInsertPos);
    m_MenuList.Insert(menuInsertPos, pNewGroup);
    pNewGroup->InsertListsIntoMenu(menuInsertPos);

    m_MenuList.Select(pNewGroup->MenuPos());

    SetRedraw();
}

void Pattern::DeleteCurrentListGroup()
{
    if ( !m_MenuList.CursorPosValid() )
        return;

    ItemMenu * pItem = *m_MenuList.m_Cursor;

    if ( ! pItem->CheckType(BaseUI::dot_step_list_group) && ! pItem->CheckType(BaseUI::dot_rt_list_group) )
        return;

    ListGroup * pGroup = static_cast<ListGroup*>(pItem);

    int pos = pGroup->ItemID();
    m_ListGroups.erase(m_ListGroups.begin() + pos);

    for ( auto it = m_ListGroups.begin() + pos; it != m_ListGroups.end(); it++ )
        (*it)->SetItemID((*it)->ItemID() - 1);

    delete pGroup;
    m_MenuList.Select(m_PosInMenuList);
    SetRedraw();
}

void Pattern::RunCurrentListGroup()
{
    if ( !m_MenuList.CursorPosValid() )
        return;

    ItemMenu * pItem = *m_MenuList.m_Cursor;

    if ( ! pItem->CheckType(BaseUI::dot_step_list_group) && ! pItem->CheckType(BaseUI::dot_rt_list_group) )
        return;

    ListGroup * pGroup = static_cast<ListGroup*>(pItem);
    pGroup->Run(g_State.Beat());
    pGroup->SetRedraw();
}

void Pattern::RunAllListGroups(double startBeat)
{
    for ( auto it = m_ListGroups.begin(); it != m_ListGroups.end(); it++ )
        (*it)->Run(startBeat);
    SetRedraw();
}

void Pattern::StopCurrentListGroup()
{
    if ( !m_MenuList.CursorPosValid() )
        return;

    ItemMenu * pItem = *m_MenuList.m_Cursor;

    if ( ! pItem->CheckType(BaseUI::dot_step_list_group) && ! pItem->CheckType(BaseUI::dot_rt_list_group) )
        return;

    ListGroup * pGroup = static_cast<ListGroup*>(pItem);

    pGroup->Stop();
    pGroup->SetRedraw();
}

void Pattern::StopAllListGroups()
{
    for ( auto it = m_ListGroups.begin(); it != m_ListGroups.end(); it++ )
        (*it)->Stop();

    SetRedraw();
}

void Pattern::StopAllListGroups(double lastBeat)
{
    for ( auto it = m_ListGroups.begin(); it != m_ListGroups.end(); it++ )
        (*it)->Stop(lastBeat);

    SetRedraw();
}

void Pattern::ClearStopAllListGroups()
{
    for ( auto it = m_ListGroups.begin(); it != m_ListGroups.end(); it++ )
        (*it)->ClearStop();

    SetRedraw();
}

void Pattern::ReplaceList(StepList & noteList)
{
// TODO:LG

//    if ( m_PosEdit >= m_StepListSet.size() )
//        m_StepListSet.resize(m_PosEdit + 1);
//
//    m_StepListSet.at(m_PosEdit) = noteList;
}


string Pattern::Label(size_t width)
{
    char buf[100];
    char format[20];

    if ( width > 80 )                           // Make sure we don't overflow buffer.
        width = 80;

    if ( m_Label.size() > width - 2 )                    // Allow for quotes.
    {
#if defined(MA_BLUE)
        const char * format2 = "%%.%us...";
#else
        const char * format2 = "%%.%lus...";
#endif
        snprintf(format, 20, format2, width - 5);    // Allow for quotes and ellipsis.
    }
    else
        strcpy(format, "%s");

    snprintf(buf, 100, format, m_Label.c_str());

    return buf;
}


void Pattern::DeleteCurrentList()
{
// TODO:LG
//    if ( m_StepListSet.empty() )
//        return;
//
//    m_StepListSet.erase(m_StepListSet.begin() + m_PosEdit);
//
//    if ( m_StepListSet.empty() )
//    {
//        m_PosEdit = 0;
//        m_Pos = 0;
//        return;
//    }
//
//    // If the edit pointer is above the list that was deleted,
//    // move it down to keep it with the list it points at.
//    //
//    // Or, if the edit pointer was pointing at the last list in
//    // the list and that was deleted, it needs to point to the item
//    // that's now at the end of the list.
//    //
//    // (If it was pointing at the list that was deleted, it now
//    // points to the one that took its place.)
//
//    if ( m_Pos > m_PosEdit || m_Pos == m_StepListSet.size() )
//        m_Pos -= 1;
//
//    // The edit pointer stays in place and now points to next in
//    // list (unless it was already at the end of the list).
//
//    if ( m_PosEdit == m_StepListSet.size() )
//        m_PosEdit -= 1;
//
}

void Pattern::DeleteCurrentRealTimeList()
{
// TODO:LG
//    if ( m_RealTimeSet.empty() )
//        return;
//
//    m_RealTimeSet.erase(m_RealTimeSet.begin() + m_PosRealTimeEdit);
//
//    if ( m_RealTimeSet.empty() )
//    {
//        m_PosRealTimeEdit = 0;
//        return;
//    }
//
//    // The edit pointer stays in place and now points to next in
//    // list (unless it was already at the end of the list).
//
//    if ( m_PosRealTimeEdit == m_RealTimeSet.size() )
//        m_PosRealTimeEdit -= 1;
//
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

string & Centre(string & line, unsigned centre, unsigned width, int & offset)
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
//    int offset, length;
    int row = 1;

    string result;
    string line;

    centre -= 4;
    width -= 4;     // Allow for edit cursors in left column.

    // Trigs

//    result = "\n    ";
//    if ( ! m_TrigList.Empty() )
//    {
//        result += m_TrigList.ToStringForDisplay2(offset, length, width);
//        highlights.push_back(PosInfo2(row, offset + 4, length));
//        result += '\n';
//    }
//    else
//        result += "Triggers: Auto\n";

    result += '\n';
    row += 2;

    // How many list rows can we show.

    displayRows -= row;

// TODO:LG
//    int limitStep = m_StepListSet.size();
//    int limitRealTime = m_RealTimeSet.size();
//
//    if ( limitStep > 0 && limitRealTime > 0 )
//        displayRows -= 1; // Allow for spacer row.
//
//    while ( limitStep + limitRealTime > displayRows )
//    {
//        if ( limitRealTime >= limitStep )
//            limitRealTime -= 1;
//        else
//            limitStep -= 1;
//    }
//
//    // Step Lists
//
//    while ( m_DisplayStartStep > m_PosEdit )
//        m_DisplayStartStep -= 1;
//
//    while ( m_DisplayStartStep + limitStep <= m_PosEdit )
//        m_DisplayStartStep += 1;
//
//    for ( unsigned i = m_DisplayStartStep; i < m_DisplayStartStep + limitStep; i++ )
//    {
//        if ( i == m_PosEdit )
//        {
//            result += " -> ";
//            // highlights.push_back(PosInfo2(row, 1, 2));
//        }
//        else
//            result += "    ";
//        switch(mode)
//        {
//        case 1:
//            highlights.push_back(PosInfo2(row, centre, 4));
//            line = m_StepListSet.at(i).ToStringForDisplay(offset, length);
//            result += Centre(line, centre, width, offset);
//            if ( length > 0 )
//                highlights.push_back(PosInfo2(row++, offset + 4, length));
//            break;
//        case 2:
//            line = m_StepListSet.at(i).ToStringForDisplay2(offset, length, width);
//            if ( length > 0 )
//                highlights.push_back(PosInfo2(row++, offset + 4, length));
//            line += '\n';
//            result += line;
//            break;
//        default:
//            break;
//        }
//    }
//
//    // Add spacer if both kinds of list are present.
//
//    if ( limitStep > 0 && limitRealTime > 0 )
//    {
//        result += '\n';
//        row += 1;
//    }
//
//    // Realtime Lists
//
//    while ( m_DisplayStartRealTime > m_PosRealTimeEdit )
//        m_DisplayStartRealTime -= 1;
//
//    while ( m_DisplayStartRealTime + limitRealTime <= m_PosRealTimeEdit )
//        m_DisplayStartRealTime += 1;
//
//    for ( unsigned i = m_DisplayStartRealTime; i < m_DisplayStartRealTime + limitRealTime; i++ )
//    {
//        if ( i == m_PosRealTimeEdit )
//        {
//            result += " -> ";
//            // highlights.push_back(PosInfo2(row, 1, 2));
//        }
//        else
//            result += "    ";
//        // highlights.push_back(PosInfo2(row, 4, 5));
//        result += m_RealTimeSet.at(i).ToStringForDisplay(offset, length);
//        // highlights.push_back(PosInfo2(row++, offset + 4, length));
//        result += '\n';
//    }

    return result;
}
