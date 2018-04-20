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

        void ResetPosition();
        void SetStatus();
        bool HandleKey(BaseUI::key_command_t k);

        void NewListGroup();

    protected:
        enum list_group_t
        {
            lt_step,
            lt_realtime
        };

        list_group_t m_Type;
        uint8_t m_MidiChannel = 0;
        double m_Step = 4;
};

class StepListGroup : public ListGroup
{
    public:
//        std::vector<StepListPtr> m_StepListSet;
        std::vector<StepList> m_StepListSet;

        StepListGroup()
        {
            m_DisplayObjectType = BaseUI::dot_step_list_group;
            m_Type = lt_step;
            m_PopUpMenuID = C_MENU_ID_STEPGROUP;
            m_MenuListIndent = 0;

        }

//        StepListPtr NewStepList();
        StepList * NewStepList();


};

class RTListGroup : public ListGroup
{
    public:
        std::vector<RealTimeList> m_RealTimeSet;

        RTListGroup()
        {
            m_DisplayObjectType = BaseUI::dot_rt_list_group;
            m_Type = lt_realtime;
            m_PopUpMenuID = C_MENU_ID_RTGROUP;
            m_MenuListIndent = 2;
        }
};

#endif // MALISTGROUP_H_INCLUDED
