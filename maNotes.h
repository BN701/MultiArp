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

#ifndef MANOTES_H_INCLUDED
#define MANOTES_H_INCLUDED

#include <vector>
#include <string>
#include <map>

#include "maItemMenu.h"

#define PLAY_DATA_STEP 0x01
#define PLAY_DATA_GATE 0x02
#define PLAY_DATA_VELO 0x04
#define PLAY_DATA_SCALE 0x08
#define PLAY_DATA_ALL 0x0F

struct PosInfo
{
    int offset = 0;
    int length = 0;

    PosInfo(int ofs, int len):
        offset(ofs),
        length(len)
    {}
};

struct PosInfo2 : public PosInfo
{
    int row = 0;

    PosInfo2(int row, int ofs, int len):
        PosInfo(ofs,len),
        row(row)
    {}
};

struct Note : public ItemMenu
{
    friend class StepList;
    friend class Cluster;

    int m_NoteNumber;        // -1 indicates 'empty' or 'rest'.
    int m_NoteVelocity;      // -1 indicates not set (so use value from elsewhere)

    double m_Length;         // Length in beats (or fraction of a beat).
    double m_Phase;

    // Self-modifying list things.

    double m_Inc = 0;
    double m_Moved = 0;
    double m_Target = 0;
    int m_Interval = 0;

    bool IncrementAndCheckTarget();

    Note(int n = -1, int v = -1, double length = 0, double phase = 0 ):
        m_NoteNumber(n),
        m_NoteVelocity(v),
        m_Length(length),
        m_Phase(phase)
    {
        m_DisplayObjectType = BaseUI::dot_note;
    }

    void Set(Note & n)
    {
        m_NoteNumber = n.m_NoteNumber;
        m_NoteVelocity = n.m_NoteVelocity;
        m_Length = n.m_Length;
        m_Phase = n.m_Phase;
    }

    std::string ToString(bool fullFormat = true);
    bool NoteFromString(std::string);

    static const char * NoteNameOnly(int n); // Expose the notes/numbers look up table.
    static std::string NoteString(int n); // Expose the notes/numbers look up table.
    static int NoteNumberFromString(std::string note);

    void SetPhase( double val ) { m_Phase = val; }
    void AdjustPhase ( double multiplier, double phase, double globalPhase, double base );
    double Phase() { return m_Phase; }

    void SetLength( double val ) { m_Length = val; }
    double Length() { return m_Length; }

    virtual void Update(Cluster & chord);
    virtual void SetStatus();

    protected:
        enum note_edit_menu_focus_t
        {
            nef_note_number,
            nef_velocity,
            nef_length,
            num_nef_types
        };

        virtual bool HandleKey(BaseUI::key_command_t k);
        note_edit_menu_focus_t m_NoteEditFocus = nef_note_number;

};

struct Cluster : public ItemMenu
{
    friend class StepList;

    std::vector<Note> m_Notes;
    int m_StepLength = 0;           // This will be filled in at some point to indicate how many rests follow.

    Cluster():
        m_StepLength(1)
    {
        m_DisplayObjectType = BaseUI::dot_cluster;
        m_PopUpMenuID = C_MENU_ID_CLUSTER;
        m_Help = "S-Left/Right: insert note, C-Left/Right: copy note, S-Del: delete";
        // Todo: Should an empty cluster, which behaves as a rest, contain an actual rest? No ...
        // m_Notes.emplace_back();
    }

    Cluster & operator+=(const Cluster & rhs)
    {
        // Copy note data, not the whole object with its menu data.
        // m_Notes.insert(m_Notes.end(), rhs.m_Notes.begin(), rhs.m_Notes.end());
        for ( auto it = rhs.m_Notes.begin(); it != rhs.m_Notes.end(); it++ )
            m_Notes.emplace_back(it->m_NoteNumber, it->m_NoteVelocity, it->m_Length, it->m_Phase);
        return *this; // return the result by reference
    }

    Note & Add(int n, int v = -1)
    {
//        m_Notes.push_back(Note(n, v));
//        return m_Notes.back();
        Note note(n, v);
        return Add(note);
    }

    Note & Add(Note & note);
//    {
//        m_Notes.push_back(note);
//        return m_Notes.back();
//    }

    void InsertLeft();
    void InsertRight();
    void CopyLeft();
    void CopyRight();
    void DeleteNote();

    virtual void Update(Cluster & chord);

    void Clear() { m_Notes.clear(); }
    bool Empty() { return m_Notes.empty(); }
    bool IsRest();
    void SetPhaseAllNotes(double phase);

    void SetStepsTillNextNote( int val )
    {
        // Use val if step length hasn't been set, or
        // if less than current value.

        if ( m_StepLength == 0 || val < m_StepLength )
            m_StepLength = val;
    }

    int StepsTillNextNote() { return m_StepLength; }

    std::string ToString(bool fullFormat = true);
    bool ClusterFromString(std::string s);

    virtual void SetStatus();
    protected:
        virtual bool HandleKey(BaseUI::key_command_t k);
        std::vector<Note>::size_type m_PosEdit = 0;

};

#endif // MANOTES_H_INCLUDED
