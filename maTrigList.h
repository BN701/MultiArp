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

    protected:

    private:
        std::vector<TrigListItem> m_TrigSteps;
        std::vector<TrigListItem>::size_type m_Pos;
        std::vector<TrigListItem>::size_type m_PosEdit;
};

#endif // TRIGLIST_H
