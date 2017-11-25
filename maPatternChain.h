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
        enum pattern_chain_mode_t
        {
            off,
            natural,
            quantum,
            num_pattern_chain_modes
        };

        PatternChain();
        virtual ~PatternChain();

        ChainLink & at(std::vector<ChainLink>::size_type pos) { return m_Chain.at(pos); }
        bool empty() { return m_Chain.empty(); }
        std::vector<ChainLink>::size_type size() { return m_Chain.size(); }
        void SetMode(pattern_chain_mode_t val) { m_PatternChainMode = val; }
        pattern_chain_mode_t Mode() { return m_PatternChainMode; }

        std::vector<ChainLink>::size_type PosPlay() { return m_PosPlay; }
        void SetPosPlay( int val ) { m_PosPlay = static_cast<std::vector<ChainLink>::size_type>(val); }
        void ResetPosPlay() { m_PosPlay = 0; }

        std::string ToStringForDisplay(int firstRow, int rows);
        std::string ToString();
        void FromString(std::string s);
        void FromSimpleString(std::string s);

        void SetJumpOverride( int val )
        {
            m_JumpOverride = val;
        }

        int JumpOverride()
        {
            int t = m_JumpOverride;
            m_JumpOverride = -1;
            return t;
        }

        void Clear()
        {
            m_Chain.clear();
            m_PosEdit = 0;
            SetStatus();
        }

        void New()
        {
            m_Chain.emplace_back();
            m_PosEdit = m_Chain.size() - 1;
            SetStatus();
        }

        void Delete()
        {
            m_Chain.pop_back();
            if ( m_PosEdit >= m_Chain.size() )
            {
                m_PosEdit = m_Chain.size() - 1;
                // Menu focus will also be at max, so reset it.
                m_MenuFocus = static_cast<pattern_chain_menu_focus_t>(m_MenuFocus - 1);
            }
            if ( m_PosPlay >= m_Chain.size() )
                m_PosPlay = m_Chain.size() - 1;
            SetStatus();
        }

        virtual void SetStatus();
        virtual void SetFocus()
        {
            CursorKeys::SetFocus();
            // SetStatus();
        }

    protected:
        enum pattern_chain_menu_focus_t
        {
            mode,
            num_pc_menu_focus_modes
        };

        virtual bool HandleKey(key_type_t k);
        void NextPatternChainMode( int dir );

        pattern_chain_menu_focus_t m_MenuFocus = mode;
        std::vector<ChainLink>::size_type m_PosEdit = 0;
        std::vector<ChainLink>::size_type m_PosPlay = 0;
        std::vector<ChainLink>::size_type m_JumpOverride = -1;

    private:

        std::vector<ChainLink> m_Chain;
        pattern_chain_mode_t m_PatternChainMode = off;

};

#endif // PATTERNCHAIN_H
