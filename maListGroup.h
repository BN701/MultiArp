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

#ifndef MALISTGROUP_H_INCLUDED
#define MALISTGROUP_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <queue>

#include "maItemMenu.h"
//#include "maPattern.h"
#include "maRealTimeList.h"
#include "maStepList.h"
#include "maTrigList.h"

class Pattern;

class ListGroup : public ItemMenu
{
    public:
        enum list_group_type
        {
            lgtype_step = 0,
            lgtype_realtime
        };

        ListGroup(Pattern & p, list_group_type type);
        ListGroup(ListGroup & lg);
        ~ListGroup();

        list_group_type Type() { return m_Type; }
        void ResetPosition();
        void SetStatus();
        bool HandleKey(BaseUI::key_command_t k);

        static bool Step(int ListGroupID, int queueId);

//        void SetQueueID(int queueID) { m_QueueID = queueID; }
        virtual bool Step(int queueId);

        virtual void AddToMenuList(MenuList & m) = 0;
        virtual void InsertListsIntoMenu(menu_list_cursor_t before) = 0;
        virtual void RemoveListsFromMenu() = 0;

        virtual void Run(int queueId);

        void Stop();

//        void NewListGroup();

    protected:

        Pattern & m_Parent;

        enum listgroup_params_menu_focus_t
        {
            lgp_run_stop,
            lgp_midi_channel,
            lgp_step_value,
            lgp_quantum,
            num_listgroup_params_menu_focus_modes
        };

        listgroup_params_menu_focus_t m_ListGroupMenuFocus = lgp_run_stop;

        static int m_ListGroupCounter;
        static std::map<int, ListGroup*> m_ListGroupsLookup;
        int m_ListGroupID = m_ListGroupCounter++;

        std::queue<ItemMenu*> m_DeferredRedrawList;


        list_group_type m_Type;
        uint8_t m_MidiChannel = 0;
//        double m_Step = 4;

        char m_Progress[20] = "";
        double m_CurrentStepValue = 4;
        double m_LastUsedStepValue = 4;
        double m_Quantum = 4;
        double m_Beat = 0;
        double m_Phase = 0;

        double m_StepEdit = 4;
        double m_StepPending = 4;

        double m_QuantumEdit = 4;
        double m_QuantumPending = 4;

        // Step() working data used by derived functions.
        uint64_t m_QueueTimeUsec;
        double m_NextBeatSwung;
        double m_Tempo;

        bool m_Running = false;
//        int m_QueueID = -1;

};

typedef std::vector<StepList>::size_type size_steplist;

struct update_pair
{
    size_steplist list_id;
    std::vector<Cluster>::size_type list_pos;
};

class StepListGroup : public ListGroup
{
    public:
        TrigList m_TrigList;

        std::vector<StepList> m_StepListSet;
        size_steplist m_Pos = 0;

        std::vector<update_pair> m_DeferredUpdates;

        StepListGroup(Pattern & p);
        StepListGroup(StepListGroup * g);

        StepList * NewStepList();
        void MoveListUp(StepList * pItem, MenuList & menu);
        void MoveListDown(StepList * pItem, MenuList & menu);
        void CopyList(StepList * pItem, MenuList & menu);
        void DeleteList(StepList * pItem, MenuList & menu);

        void StepTheLists( Cluster & cluster,
                        TrigRepeater & repeater,
                        double & stepValueMultiplier,
                        double phase,
                        double stepValue,
                        double globalBeat );
        virtual bool Step(int queueId);
        virtual void Run(int queueId);

        virtual void AddToMenuList(MenuList & m);
        virtual void InsertListsIntoMenu(menu_list_cursor_t before);
        virtual void RemoveListsFromMenu();

    private:
        void MoveList(StepList * pItem, MenuList & menu, bool up);

};

class RTListGroup : public ListGroup
{
    public:
        std::vector<RealTimeList> m_RealTimeSet;

        RTListGroup(Pattern & p);
        RTListGroup(RTListGroup * g);

        virtual bool Step(int queueId);
        virtual void AddToMenuList(MenuList & m);
        virtual void InsertListsIntoMenu(menu_list_cursor_t before);
        virtual void RemoveListsFromMenu();

};

#endif // MALISTGROUP_H_INCLUDED
