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

#ifndef PATTERNCHAIN_H
#define PATTERNCHAIN_H

#include <string>
#include <vector>

#include "maChainLink.h"
#include "maCursorKeys.h"


class PatternChain : public CursorKeys
{
    public:
        PatternChain();
        virtual ~PatternChain();

        ChainLink & at(std::vector<ChainLink>::size_type pos) { return m_Chain.at(pos); }
        bool empty() { return m_Chain.empty(); }
        std::vector<ChainLink>::size_type size() { return m_Chain.size(); }


        std::string ToStringForDisplay(int firstRow, int rows);
        std::string ToString();
        void FromString(std::string s);
        void FromSimpleString(std::string s);

        void SetStatus();
        virtual void SetFocus()
        {
            CursorKeys::SetFocus();
            // SetStatus();
        }

    protected:
        virtual bool HandleKey(key_type_t k);

        std::vector<ChainLink>::size_type m_PosEdit = 0;

    private:

        std::vector<ChainLink> m_Chain;

};

#endif // PATTERNCHAIN_H
