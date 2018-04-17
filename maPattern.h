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

#ifndef MAPATTERN_H_INCLUDED
#define MAPATTERN_H_INCLUDED

#include <list>
#include <map>
#include <string>
#include <vector>

#include "maItemMenu.h"
#include "maFeelMap.h"
#include "maNotes.h"
#include "maListGroup.h"
#include "maRealTimeList.h"
#include "maStepList.h"
#include "maTranslateTable.h"
#include "maTrigList.h"

struct Pattern : public ItemMenu
{
    static Pattern EmptyPattern;
    std::vector<StepList>::size_type m_Pos;
//    std::vector<StepList>::size_type m_PosEdit;
    std::vector<RealTimeList>::size_type m_PosRealTimeEdit;
    std::vector<StepList>::size_type m_LastRequestedPos;

//    std::vector<int> m_Trigs;

//    std::vector<StepList> m_StepListSet;
//    std::vector<RealTimeList> m_RealTimeSet;

    std::vector<ListGroup *> m_ListGroups;
    MenuList m_MenuList;
//    std::list<ItemMenu *>::iterator m_PosCursor;
    // int m_PosCursor = 0;

    menu_list_cursor_t CursorPos() { return m_MenuList.m_Cursor; }

    std::string m_Label;

    double m_StepValue;
    double m_Gate;
    bool m_GateHold;
    unsigned char m_Velocity;

    FeelMap m_FeelMap;
    TranslateTable m_TranslateTable;
    TrigList m_TrigList;

    unsigned m_DisplayStartStep = 0;
    unsigned m_DisplayStartRealTime = 0;

    double m_RealTimeBeat = 0;  // Tracks beat for real time lists. Can be reset.
    double m_LastRealTimeBeat = 0;
    double m_RealTimeBeatStart = 0;

    double LastRealTimeBeat() { return m_LastRealTimeBeat; }

    bool m_RealTimeComplete = false;

    void StoreStepValue( double val );
    void StoreGate( double gate);
    void StoreGateHold( bool val );
    void StoreVelocity( unsigned char vel );
    void StoreTranslateTable( TranslateTable & table );

    double StepValue() { return m_StepValue; }
    double Gate() { return m_Gate; }
    bool GateHold() { return m_GateHold; }
    unsigned char Velocity() { return m_Velocity; }

//    StepList & StepListForEdit() { return m_StepListSet.at(m_PosEdit); }
//    RealTimeList & RTListForEdit() { return m_RealTimeSet.at(m_PosRealTimeEdit); }
//    ListGroup & ListGroupForEdit() { return *m_ListGroups.at(m_PosEdit); }
    TranslateTable & PatternTranslateTable() { return m_TranslateTable; }
    FeelMap & PatternFeelMap() { return m_FeelMap; }
    TrigList & PatternTrigList() { return m_TrigList; }

    void SetStatus();

    Pattern():
        m_Pos(0),
//        m_PosEdit(0),
        m_PosRealTimeEdit(0),
        m_LastRequestedPos(0),
        m_StepValue(16),
        m_Gate(0.5),
        m_GateHold(false),
        m_Velocity(64),
        m_MenuList(this, &m_Visible)
    {
        m_PopUpMenuID = C_MENU_ID_PATTERN;
        m_MenuList.Add(this);
        m_MenuList.Select(m_MenuPos);
        m_MenuListIndent = 0;
        m_DisplayObjectType = BaseUI::dot_pattern;
        m_MenuList.m_DisplayObjectType = BaseUI::dot_pattern_menu_list;
    }

    Pattern(const Pattern & p):
        m_Pos(p.m_Pos),
        m_StepValue(p.m_StepValue),
        m_Gate(p.m_Gate),
        m_GateHold(p.m_GateHold),
        m_Velocity(p.m_Velocity),
        m_Label(p.m_Label),
        m_MenuList(this, &m_Visible)
    {
        m_Visible = false;
        m_PopUpMenuID = p.m_PopUpMenuID;
        m_MenuList.Add(this);
        m_MenuList.Select(m_MenuPos);
        m_MenuListIndent = p.m_MenuListIndent;
        m_DisplayObjectType = p.m_DisplayObjectType;
        m_MenuList.m_DisplayObjectType = p.m_MenuList.m_DisplayObjectType;

        for ( auto lg = p.m_ListGroups.begin(); lg != p.m_ListGroups.end(); lg++ )
        {
            m_ListGroups.push_back(*lg);
            m_MenuList.m_Items.push_back(m_ListGroups.back());
        }
    }


    void Clear()
    {
        m_Label.clear();
//        m_StepListSet.clear();
//        m_RealTimeSet.clear();
        while ( ! m_ListGroups.empty() )
        {
            delete m_ListGroups.back();
            m_ListGroups.pop_back();
        }
        ResetPosition();
        m_TranslateTable.Reset();
        m_TrigList.Clear();
        m_StepValue = 4.0;
        m_Gate = 0.5;
        m_GateHold = false;
        m_Velocity = 64;
    }

    void SetLabel(const char * label)
    {
        m_Label = label;
        SetRedraw();
    }

    virtual void SetRedraw()
    {
        if ( m_Visible )
            for ( auto it = m_MenuList.m_Items.begin(); it != m_MenuList.m_Items.end(); it++ )
                m_RedrawList.push_back(*it);
    }

//    void SetCursorPos( int p )
//    {
////        if ( p >= 0 && p < m_StepListSet.size() )
////            m_PosEdit = p;
//        if ( p >= 0 && p < m_MenuList.size() )
//            m_PosCursor = p;
//    }

    command_menu_id_t PopUpMenuID()
    {
        // If there's a legitimate reason for coming here
        // when the display list is empty we'll add a
        // check. Otherwise, let it crash and solve it
        // in the debugger.

        return (*m_MenuList.m_Cursor)->PopUpMenuID();
    }

    std::string StepListManager(command_t command);

//    void SetRealTimeEditPos( std::vector<int>::size_type p )
//    {
//        if ( p >= 0 && p < m_RealTimeSet.size() )
//            m_PosRealTimeEdit = p;
//    }

    void SetStepValue( double val ) { m_StepValue = val; }
//    int StepListCount() { return m_StepListSet.size(); }
    int ListGroupCount() { return m_ListGroups.size(); }
//    int RealTimeListCount() { return m_RealTimeSet.size(); }
    int TrigListCount() { return m_TrigList.Size(); }
    void ResetPosition()
    {
        m_Pos = 0;
        m_TrigList.ResetPosition();

        for ( auto group = m_ListGroups.begin(); group != m_ListGroups.end(); group++ )
            (*group)->ResetPosition();

        m_RealTimeBeat = m_RealTimeBeatStart;
        m_RealTimeComplete = false;
//        for ( auto rtList = m_RealTimeSet.begin(); rtList != m_RealTimeSet.end(); rtList++ )
//            rtList->ResetPosition();
    }

    void Step(Cluster & cluster, TrigRepeater & repeater, double & stepValueMultiplier, double phase, double stepValue, double globalBeat);

    bool LabelEmpty()
    {
        return m_Label.empty();
    }

    size_t TrigPlayPosition() { return m_TrigList.PlayPosition(); }
    std::string Label() { return m_Label; }
    std::string Label(size_t width);
    std::string TrigsToStringForDisplay();
    std::string ToString(const char * prefix);
    std::string Display(int mode, std::vector<PosInfo2> & highlights, int centre = 30, int width = 80, int rows = 11);

    bool FromString(std::string s, int & updates);
    void AddRealTimeList(std::map<double,Note> realTimeList, double quantum);
    void AddRealTimeListFromString(std::vector<RealTimeList>::size_type index, std::string s);
#ifndef MA_BLUE
    void AddRealTimeListFromMidiFile(std::string s);
#endif
    void SetRealTimeMultipliers(std::vector<std::string>::iterator token, std::vector<std::string>::iterator last);
    void SetRealTimeQuantum(std::string & token);
    void SetRealTimeStartPhase(std::string & token);
    void StartRealTimeEcho(std::vector<std::string>::iterator token, std::vector<std::string>::iterator last);
    bool AddStepListFromString(std::vector<int>::size_type index, std::string s);
    void SetFieldsFromString(std::string s);

    void ReplaceList(StepList & noteList);
    void NewListGroup(ListGroup::list_group_type type);
    void DeleteCurrentList();
    void DeleteCurrentRealTimeList();

    bool AllListsComplete();

    protected:
        virtual bool HandleKey(BaseUI::key_command_t k);

//    void UpCursorPos() { SetCursorPos( m_PosCursor + 1); }
        void UpCursorPos()
        {
            m_MenuList.UpCursorPos();
//        if ( m_PosCursor != --m_MenuList.end())
//            m_PosCursor++;
        }

        void DownCursorPos()
        {
            m_MenuList.DownCursorPos();
//        if ( m_PosCursor != m_MenuList.begin() )
//            m_PosCursor--;
        }

//    void UpRTEditPos() { SetRealTimeEditPos( m_PosRealTimeEdit + 1); }
//    void DownRTEditPos() { SetRealTimeEditPos( m_PosRealTimeEdit - 1); }

};



#endif // MAPATTERN_H_INCLUDED
