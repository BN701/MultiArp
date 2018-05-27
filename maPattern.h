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
#include <unordered_set>
#include <vector>

#include "maCommand.h"
#include "maFeelMap.h"
#include "maItemMenu.h"
#include "maListGroup.h"
#include "maNotes.h"
#include "maRealTimeList.h"
#include "maStepList.h"
#include "maTranslateTable.h"
#include "maTrigList.h"

struct Pattern : public ItemMenu
{
    // Statics

    static Pattern EmptyPattern;
    static int m_PatternCount;
    static std::unordered_set<std::string> m_ShortLabels;

    // Variables

    std::vector<StepList>::size_type m_Pos = 0;
//    std::vector<StepList>::size_type m_PosEdit;
    std::vector<RealTimeList>::size_type m_PosRealTimeEdit = 0;
    std::vector<StepList>::size_type m_LastRequestedPos = 0;

    std::string m_Label = "Empty Pattern";
    std::string m_ShortLabel;

    double m_StepValue = 16;
    double m_Gate = 0.5;
    bool m_GateHold = false;
    unsigned char m_Velocity = 64;

    unsigned m_DisplayStartStep = 0;
    unsigned m_DisplayStartRealTime = 0;

    double m_RealTimeBeat = 0;  // Tracks beat for real time lists. Can be reset.
    double m_LastRealTimeBeat = 0;
    double m_RealTimeBeatStart = 0;

    bool m_RealTimeComplete = false;

    // Containers

    std::vector<ListGroup *> m_ListGroups;

//    std::vector<int> m_Trigs;

//    std::vector<StepList> m_StepListSet;
//    std::vector<RealTimeList> m_RealTimeSet;

//    std::list<ItemMenu *>::iterator m_PosCursor;

    // Embedded objects

    MenuList m_MenuList;

    FeelMap m_FeelMap;
    TranslateTable m_TranslateTable;
//    TrigList m_TrigList;

    // Functions

    Pattern();
    Pattern(const Pattern & p);
    ~Pattern();

    void ResetMenuList();

    menu_list_cursor_t CursorPos() { return m_MenuList.m_Cursor; }
    double LastRealTimeBeat() { return m_LastRealTimeBeat; }

    void StoreStepValue( double val );
    void StoreGate( double gate);
    void StoreGateHold( bool val );
    void StoreVelocity( unsigned char vel );
    void StoreTranslateTable( TranslateTable & table );

    double StepValue() { return m_StepValue; }
    double Gate() { return m_Gate; }
    bool GateHold() { return m_GateHold; }
    unsigned char Velocity() { return m_Velocity; }

//    ListGroup & ListGroupForEdit() { return *m_ListGroups.at(m_PosEdit); }
    TranslateTable & PatternTranslateTable() { return m_TranslateTable; }
    FeelMap & PatternFeelMap() { return m_FeelMap; }
//    TrigList & PatternTrigList() { return m_TrigList; }

    void SetStatus();

    void Clear();

    void SetLabel(const char * label);
    void SetShortLabel(const char * label = NULL);

    int SetPatternID()
    {
        m_PatternID = ++m_PatternCount;
        return m_PatternID;
    }
    int PatternID() { return m_PatternID; }

    virtual void SetRedraw();
    virtual void SetVisible(bool val);

    command_menu_id_t PopUpMenuID()
    {
        // If there's a legitimate reason for coming here
        // when the display list is empty we'll add a
        // check. Otherwise, let it crash and solve it
        // in the debugger.

        return (*m_MenuList.m_Cursor)->PopUpMenuID();
    }

    std::string StepListManager(command_t command);

    void SetStepValue( double val ) { m_StepValue = val; }
//    int StepListCount() { return m_StepListSet.size(); }
    int ListGroupCount() { return m_ListGroups.size(); }
//    int RealTimeListCount() { return m_RealTimeSet.size(); }
//    int TrigListCount() { return m_TrigList.Size(); }
    void ResetPosition();

    void Step(Cluster & cluster, TrigRepeater & repeater, double & stepValueMultiplier, double phase, double stepValue, double globalBeat);

    bool LabelEmpty()
    {
        return m_Label.empty();
    }

//    size_t TrigPlayPosition() { return m_TrigList.PlayPosition(); }
    std::string Label() { return m_Label; }
    std::string Label(size_t width);
    const char * ShortLabel() { return m_ShortLabel.c_str(); }
//    std::string TrigsToStringForDisplay();
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
    void CopyCurrentListGroup();
    void RunCurrentListGroup();
    void StopCurrentListGroup();
    void RunAllListGroups(double startBeat);
    void StopAllListGroups();
    void StopAllListGroups(double lastBeat);
    void DeleteCurrentListGroup();

    void DeleteCurrentList();
    void DeleteCurrentRealTimeList();

    bool AllListsComplete();

    protected:
        int m_PatternID = -1;
        virtual bool HandleKey(BaseUI::key_command_t k);

        void UpCursorPos()
        {
            m_MenuList.UpCursorPos();
        }

        void DownCursorPos()
        {
            m_MenuList.DownCursorPos();
        }

};



#endif // MAPATTERN_H_INCLUDED
