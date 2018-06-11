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

#ifndef CHAINLINK_H
#define CHAINLINK_H

//#include <string>

#include "maItemMenu.h"

class PatternChain;
class PatternStore;

class ChainLink : public ItemMenu
{
    friend PatternChain;

    public:
        ChainLink(PatternStore * store = NULL);
        virtual ~ChainLink();

        std::string & PatternLabel() { return m_PatternLabel; }
        size_t PatternHash() { return m_PatternHash; }
        void SetPattern(std::string label, int hash);

        int Repeats() { return m_Repeats; }
        void ClearRemaining() { m_Remaining = -1; }
        int Remaining();
        bool RemainingIsOne();
        void SetRepeats(int val) { m_Repeats = val; }
        int Jump() { return m_Jump; }
        void SetJump(int val) { m_Jump = val; }

        std::string ToString();
        std::string ToStringForDisplay(bool forMenu = false, unsigned width = 12);
        void FromString(std::string & s);

        virtual void SetStatus();

    protected:
//        virtual bool HandleKey(key_type_t k);
        virtual bool HandleKey(BaseUI::key_command_t k);

    private:
//        int m_PatternID = 0;
        std::string m_PatternLabel = " - "; // A pattern's 'short label'
        size_t m_PatternHash = -1;

        int m_Repeats = 0;              // Zero means no repeat. Set to -1 for indefinite repeat.
        int m_Remaining = -1;           // -1 means initialize in RepeatsRemaining().
        int m_Jump = -1;

        int m_PosEdit = 0;

        PatternStore * m_PatternStore = NULL;
};

#endif // CHAINLINK_H
