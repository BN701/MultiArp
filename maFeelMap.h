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

#ifndef FEELMAP_H
#define FEELMAP_H

#include <vector>

#include "maCursorKeys.h"

class FeelMap : public CursorKeys
{
    public:
        FeelMap();
        virtual ~FeelMap();

        double Feel(double beat);

        virtual void SetFocus()
        {
            CursorKeys::SetFocus();
            SetStatus();
        }

    protected:
        void SetStatus();

        virtual bool HandleKey(key_type_t k);
        int m_EditPoint;

    private:

        int m_Points;
        std::vector<double> m_StretchPoints;
};

#endif // FEELMAP_H
