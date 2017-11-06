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

#ifndef CHAINLINK_H
#define CHAINLINK_H

#include <string>

#include "maCursorKeys.h"

class ChainLink : public CursorKeys
{
    public:
        ChainLink();
        virtual ~ChainLink();

        int Pattern() { return m_Pattern; }
        void SetPattern(int val) { m_Pattern = val; }
        int Repeats() { return m_Repeats; }
        void ClearRemaining() { m_Remaining = -1; }
        int Remaining();
        void SetRepeats(int val) { m_Repeats = val; }
        int Jump() { return m_Jump; }
        void SetJump(int val) { m_Jump = val; }
        void SetID(int val) { m_ID = val; }

        std::string ToString();
        std::string ToStringForDisplay(bool forMenu = false, int width = 12);
        void FromString(std::string & s);

        virtual void SetStatus();
        virtual void SetFocus()
        {
            CursorKeys::SetFocus();
            // SetStatus();
        }

    protected:
        virtual bool HandleKey(key_type_t k);

    private:
        int m_Pattern = 0;
        int m_Repeats = 0;              // Zero means no repeat. Set to -1 for indefinite repeat.
        int m_Remaining = -1;           // -1 means initialize in RepeatsRemaining().
        int m_Jump = -1;

        int m_ID = -1;

        int m_PosEdit = 0;
};

#endif // CHAINLINK_H
