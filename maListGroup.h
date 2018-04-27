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

#include "maItemMenu.h"
#include "maRealTimeList.h"
#include "maStepList.h"

class ListGroup : public ItemMenu
{
    public:
        enum list_group_type
        {
            lgtype_step = 0,
            lgtype_realtime
        };

        ListGroup(list_group_type type);
        ListGroup(ListGroup & lg);
        ~ListGroup();

        list_group_type Type() { return m_Type; }
        void ResetPosition();
        void SetStatus();
        bool HandleKey(BaseUI::key_command_t k);

        static bool Step(int ListGroupID, int queueId);

        virtual void Step(int queueId);

        virtual void AddToMenuList(MenuList & m) = 0;

//        void NewListGroup();

    protected:
        enum listgroup_params_menu_focus_t
        {
            lgp_midi_channel,
            lgp_step_value,
            lgp_quantum,
            num_listgroup_params_menu_focus_modes
        };

        listgroup_params_menu_focus_t m_ListGroupMenuFocus = lgp_midi_channel;

        static int m_ListGroupCounter;
        static std::map<int, ListGroup*> m_ListGroupsLookup;
        int m_ListGroupID = m_ListGroupCounter++;


        list_group_type m_Type;
        uint8_t m_MidiChannel = 0;
//        double m_Step = 4;

        char m_Progress[20];
        double m_CurrentStepValue = 4;
        double m_LastUsedStepValue = 4;
        double m_Quantum = 4;
        double m_Beat = 0;
        double m_Phase = 0;


};

class StepListGroup : public ListGroup
{
    public:
//        std::vector<StepListPtr> m_StepListSet;
        std::vector<StepList> m_StepListSet;

        StepListGroup();
        StepListGroup(ListGroup * g);

//        StepListPtr NewStepList();
        StepList * NewStepList();

        virtual void Step(int queueId);

        virtual void AddToMenuList(MenuList & m);

};

class RTListGroup : public ListGroup
{
    public:
        std::vector<RealTimeList> m_RealTimeSet;

        RTListGroup();
        RTListGroup(ListGroup * g);

        virtual void Step(int queueId);
        virtual void AddToMenuList(MenuList & m);

};

#endif // MALISTGROUP_H_INCLUDED
