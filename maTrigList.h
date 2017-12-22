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


#ifndef TRIGLIST_H
#define TRIGLIST_H

#include "maCursorKeys.h"
#include "maTrigListItem.h"


class TrigList : public CursorKeys
{
    public:
        TrigList();
        virtual ~TrigList();

        bool Empty() { return m_TrigItems.empty(); }
        size_t Size() { return m_TrigItems.size(); }
        size_t PlayPosition() { return m_Pos; }
        void ResetPosition() { m_Pos = 0; }

        TrigListItem * Step();

        void Clear()
        {
            m_TrigItems.clear();
            m_Pos = 0;
            m_PosEdit = 0;
        }

        std::string ToString(const char * prefix);
        void FromString(std::string s);
        void FromSimpleString(std::string s);

        void AddArpeggio(std::string s);

        std::string ToStringForDisplay();
        std::string ToStringForDisplay2(int & offset, int & length, int width = 80);

        virtual void SetStatus();

    protected:
        virtual bool HandleKey(key_type_t k);

    private:
        std::vector<TrigListItem> m_TrigItems;
        std::vector<TrigListItem>::size_type m_Pos = 0;
        std::vector<TrigListItem>::size_type m_PosEdit = 0;
};

#endif // TRIGLIST_H
