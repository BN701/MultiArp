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

        ListGroup(list_group_type type):
            m_Type(type)
        {}

        list_group_type Type() { return m_Type; }
        void ResetPosition();
        void SetStatus();
        bool HandleKey(BaseUI::key_command_t k);

        virtual void AddToMenuList(MenuList & m) = 0;

//        void NewListGroup();

    protected:
        list_group_type m_Type;
        uint8_t m_MidiChannel = 0;
        double m_Step = 4;
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

        virtual void AddToMenuList(MenuList & m);

};

class RTListGroup : public ListGroup
{
    public:
        std::vector<RealTimeList> m_RealTimeSet;

        RTListGroup();
        RTListGroup(ListGroup * g);

        virtual void AddToMenuList(MenuList & m);

};

#endif // MALISTGROUP_H_INCLUDED
