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

#ifndef MASTEPLIST_H_INCLUDED
#define MASTEPLIST_H_INCLUDED

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "maItemMenu.h"
struct StepListGroup;

struct StepList : public ItemMenu
{
    std::vector<Cluster> m_Clusters;
    std::vector<Cluster>::size_type m_PosSL = 0;                  // Points to the next position to be retrieved.
    bool m_Complete = false;

//    std::vector<PosInfo> m_PosInfo; // Store string element offsets and lengths for highlighting.

    static StepList EmptyList;

    StepList(StepListGroup * parent = NULL):
        m_Parent(parent)
    {
        m_DisplayObjectType = BaseUI::dot_step_list;
        m_PopUpMenuID = C_MENU_ID_STEPLIST;
        m_MenuListIndent = 4;
        m_Help = "S-Left/Right: insert cluster, C-Left/Right: copy cluster, S-Del: delete";
    }

    StepList(const StepList & other) = default;
    virtual StepList & operator = (const StepList & other) = default;
    StepList(StepList && other) noexcept = default;

    ~StepList()
    {
    }

    void Clear ()
    {
        m_Clusters.clear();
        ResetPosition();
    }

    bool Empty()
    {
        return m_Clusters.empty();
    }

    void Add(int n = -1, int v = -1)
    {
        Cluster chord;
        chord.Add(n, v);
        Add(chord);
    }

    void Insert(int pos, Cluster & cluster);
    void Add(Cluster & cluster)
    {
        m_Clusters.push_back(cluster);
    }

    bool DeleteLast()
    {
        if ( !m_Clusters.empty() )
        {
            m_Clusters.pop_back();
            return true;
        }
        else
            return false;
    }

    // True if mpos is zero after completing a cycle.
    // False if any other position, or immediately after a reset.

    bool Complete()
    {
        return m_Complete;
    }

    void SetNowPlayingPos( std::vector<Cluster>::size_type val )
    {
        if ( m_NowPlayingPos != val )
        {
            m_NowPlayingPos = val;
            SetRedraw();
        }
    }

    Cluster * Step();

    void InsertLeft();
    void InsertRight();
    void CopyLeft();
    void CopyRight();
    void DeleteStep();

    void MoveLeft();
    void MoveRight();
    void ShiftLeft();
    void ShiftRight();

    void ResetPosition()
    {
        m_PosSL = 0;
        m_Complete = false;
    }

    std::string ToString(bool fullFormat = true);
//    std::string ToStringForDisplay(int & offset, int & length);
//    std::string ToStringForDisplay2(int & offset, int & length, unsigned width = 80);
    bool StepListFromString(std::string s);

    virtual void SetStatus();

    protected:
        virtual bool HandleKey(BaseUI::key_command_t k);
        std::vector<Cluster>::size_type m_PosEdit = 0;

    private:
        std::vector<Cluster>::size_type m_NowPlayingPos = -1;        // Updated by parent group when events are played.

        StepListGroup * m_Parent;

};

//typedef std::shared_ptr<StepList> StepListPtr;

#endif // MANOTES_H_INCLUDED
