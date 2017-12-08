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

#ifndef MANOTES_H_INCLUDED
#define MANOTES_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "maCursorKeys.h"

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

struct Note : public CursorKeys
{

    int m_NoteNumber;        // -1 indicates 'empty' or 'rest'.
    int m_NoteVelocity;      // -1 indicates not set (so use value from elsewhere)

    double m_Phase;
    double m_Length;         // Length in beats (or fraction of a beat).

    Note(int n = -1, int v = -1):
        m_NoteNumber(n),
        m_NoteVelocity(v),
        m_Phase(0),
        m_Length(0)
    {}

    std::string ToString(bool fullFormat = true);
    void FromString(std::string);

    static const char * NoteNameOnly(int n); // Expose the notes/numbers look up table.
    static std::string NoteString(int n); // Expose the notes/numbers look up table.
    static int NoteNumberFromString(std::string note);

    void SetPhase( double val ) { m_Phase = val; }
    void AdjustPhase ( double multiplier, double phase, double globalPhase, double base );
    double Phase() { return m_Phase; }

    void SetLength( double val ) { m_Length = val; }
    double Length() { return m_Length; }

    virtual void SetStatus();
    protected:
        enum note_edit_menu_focus_t
        {
            nef_note_number,
            nef_velocity,
            nef_length,
            num_nef_types
        };

        virtual bool HandleKey(key_type_t k);
        note_edit_menu_focus_t m_NoteEditFocus = nef_note_number;

};

struct Cluster : public CursorKeys
{
    std::vector<Note> m_Notes;
    int m_StepLength = 0;           // This will be filled in at some point to indicate how many rests follow.

    Cluster():
        m_StepLength(1)
    {
        m_Help = "S-Left/Right: insert note, C-Left/Right: copy note, S-Del: delete";
    }

    Cluster & operator+=(const Cluster & rhs)
    {
        m_Notes.insert(m_Notes.end(), rhs.m_Notes.begin(), rhs.m_Notes.end());
        return *this; // return the result by reference
    }

    Note & Add(int n, int v = -1)
    {
        m_Notes.push_back(Note(n, v));
        return m_Notes.back();
    }

    Note & Add(Note & note)
    {
        m_Notes.push_back(note);
        return m_Notes.back();
    }

    void Clear() { m_Notes.clear(); }
    bool Empty() { return m_Notes.empty(); }
    bool IsRest();

    void SetStepsTillNextNote( int val )
    {
        // Use val if step length hasn't been set, or
        // if less than current value.

        if ( m_StepLength == 0 || val < m_StepLength )
            m_StepLength = val;
    }

    int StepsTillNextNote() { return m_StepLength; }

    std::string ToString(bool fullFormat = true);
    void FromString(std::string s);

    virtual void SetStatus();
    protected:
        virtual bool HandleKey(key_type_t k);
        std::vector<Note>::size_type m_PosEdit = 0;

};

struct StepList : public CursorKeys
{
    std::vector<Cluster>::size_type m_Pos;                  // Points to the next position to be retrieved.
    std::vector<Cluster>::size_type m_LastRequestedPos;     // Last position for which note info was requested.
    std::vector<Cluster> m_Clusters;

//    std::vector<PosInfo> m_PosInfo; // Store string element offsets and lengths for highlighting.

    // std::string m_Label;

    bool m_Complete;

    StepList():
        m_Pos(0),
        m_LastRequestedPos(0),
        m_Complete(false)
    {
        m_Help = "S-Left/Right: insert cluster, C-Left/Right: copy cluster, S-Del: delete";
    }

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

    void Add(Cluster & chord)
    {
        m_Clusters.push_back(chord);
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

    Cluster * Step();

    void ResetPosition()
    {
        m_Pos = 0;
        m_Complete = false;
    }

    std::string ToString(bool fullFormat = true);
    std::string ToStringForDisplay(int & offset, int & length);
    std::string ToStringForDisplay2(int & offset, int & length, int width = 80);
    void FromString(std::string s);

//    bool PlayPositionInfo(int & offset,  int & length);

    virtual void SetStatus();
    protected:
        virtual bool HandleKey(key_type_t k);
        std::vector<Cluster>::size_type m_PosEdit = 0;

};

struct RealTimeListParams : public CursorKeys
{
    enum rtp_window_mode_t
    {
        normal,
        small,
        look_ahead,
        num_rtp_window_modes
    };

    double m_LoopStart = 0.0;
    double m_QuantumAtCapture;
    double m_Quantum = 0.0;  // Loop length.
    double m_Multiplier = 1.0;
    rtp_window_mode_t m_WindowMode = normal;
//    bool m_AdjustWindowToStep = true;  // Probably just if multiplier less than 1.

    void NextWindowMode( int dir );

    virtual void SetStatus();
    protected:
        enum rt_params_menu_focus_t
        {
            rtp_loop_start,
            rtp_local_quantum,
            rtp_multiplier,
            rtp_window_adjust,
            num_rt_params_menu_focus_modes
        };

        virtual bool HandleKey(key_type_t k);
        rt_params_menu_focus_t m_RTParamsFocus = rtp_loop_start;

};


struct RealTimeList : public CursorKeys
{
    std::map<double,Note> m_RealTimeList;

//    bool m_Complete = false;
//    double m_InternalBeat = 0;

    double m_LastRequestedStepValue = 4.0;
    double m_LastRequestedPhase = 0.0;

    void Step(Cluster & cluster, double phase, double stepValue);
    void Step2(Cluster & cluster, double phase, double stepValue, double globalBeat);

    RealTimeList(std::map<double,Note> realTimeList = {}, double quantum = 4.0):
        m_RealTimeList(realTimeList)
    {
        m_Params.m_Quantum = quantum;
        m_Params.m_QuantumAtCapture = quantum;
        m_Help = "S-Del: delete note, C-Del: UNDO delete!, Up/Down: move note";
    }

    void RaiseQuantumAtCapture( double val )
    {
        if ( m_Params.m_QuantumAtCapture < val )
        {
            m_Params.m_QuantumAtCapture = val;
            m_Params.m_Quantum = val;
        }
    }

//    bool Complete() { return m_Complete; }
    bool PhazeIsZero();

    void FromString(std::string s);
    std::string ToString();
    std::string ToStringForDisplay(int & offset, int & length, int width = 75);

    void SetMultiplier( double val ) { m_Params.m_Multiplier = val; }
    double AdjustedQuantum() { return m_Params.m_Multiplier * m_Params.m_Quantum; }
    unsigned long PhaseLength();
    void SetQuantum( double val ) { m_Params.m_Quantum = val; }
    virtual void SetStatus();
    protected:

        virtual bool HandleKey(key_type_t k);
        int m_RTListFocus = 0;
        int m_SubMenus = 1; // How many submenu items will there be in the CursorKeys menu?

        RealTimeListParams m_Params;

        std::vector<Note> m_UndoList;
};



#endif // MANOTES_H_INCLUDED
