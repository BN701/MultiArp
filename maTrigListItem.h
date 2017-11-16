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


#ifndef MATRIGLISTITEM_H
#define MATRIGLISTITEM_H

#include <string>

#include "maCursorKeys.h"


class TrigListItem : public CursorKeys
{
    public:
        TrigListItem();
        virtual ~TrigListItem();

        std::vector<int> & Trigs() { return m_Trigs; }
        double Multiplier() { return m_Multiplier; }
        bool Skip() { return m_Skip; }
        bool Mute() { return m_Mute; }
        int Repeats() { return m_Repeats; }
        double RepeatTime() { return m_RepeatTime; }

        std::string ToString();
        void FromString(std::string s);
        std::string TrigMaskToString();
        std::string MenuString(int width = 0);

    virtual void SetStatus();
    protected:
        virtual bool HandleKey(key_type_t k);
        enum trig_list_item_focus_t
        {
            tlif_trigs,
            tlif_multiplier,
            tlif_skip,
            tlif_mute,
            tlif_repeats,
            tlif_repeat_time,
            number_tlif_types
        };

        trig_list_item_focus_t m_TrigListItemFocus = tlif_trigs;

    private:
        std::vector<int> m_Trigs = {0}; // Default for individual item is to trigger the first list.
        unsigned int m_TrigMask = 1;    // Cheat for scrolling values in menu. (Default matches m_Trigs default.)
        double m_Multiplier = 1.0;
        bool m_Skip = false;
        bool m_Mute = false;
        int m_Repeats = 0;
        double m_RepeatTime = 0.0; // Less than means split evenly across step. (We don't use zero as that's hard to test for.)
};

#endif // MATRIGLISTITEM_H
