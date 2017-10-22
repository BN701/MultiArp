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

        bool SetActive(bool val)
        {
            m_Active = val;
            SetStatus();
        }

        bool Active() { return m_Active; }

        void SetStatus();
        virtual void SetFocus()
        {
            CursorKeys::SetFocus();
            // SetStatus();
        }

        void New(std::vector<std::string> & tokens);
        void Add();
        void Remove();
        void Respace();

        std::string ToString(const char * prefix);
        void FromString(std::string s);


    protected:

        virtual bool HandleKey(key_type_t k);

    private:

        bool m_Active;
        int m_Points;       // Number of stretchpoints - 1.
        int m_EditPoint;
        std::vector<double> m_StretchPoints;
};

#endif // FEELMAP_H
