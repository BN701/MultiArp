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

#include "maCursorKeys.h"


class TrigListItem : public CursorKeys
{
    public:
        TrigListItem();
        virtual ~TrigListItem();

        double GetMultiplier() { return m_Multiplier; }
        bool GetSkip() { return m_Skip; }
        bool GetMute() { return m_Mute; }
        int GetRepeats() { return m_Repeats; }
        double GetRepeatTime() { return m_RepeatTime; }

    protected:

    private:
        std::vector<int> m_Trigs;
        double m_Multiplier;
        bool m_Skip;
        bool m_Mute;
        int m_Repeats;
        double m_RepeatTime;
};

#endif // MATRIGLISTITEM_H
