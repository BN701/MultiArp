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

#ifndef PATTERN_STORE_H_INCLUDED
#define PATTERN_STORE_H_INCLUDED

#include <vector>
#include <string>

#include "maCursorKeys.h"
#include "maFeelMap.h"
#include "maTranslateTable.h"

#define PLAY_DATA_STEP 0x01
#define PLAY_DATA_GATE 0x02
#define PLAY_DATA_VELO 0x04
#define PLAY_DATA_SCALE 0x08
#define PLAY_DATA_ALL 0x0F

struct PosInfo
{
    int offset;
    int length;

    PosInfo(int ofs = 0, int len = 0):
        offset(ofs),
        length(len)
    {};
};

struct Note
{

    int m_NoteNumber;        // -1 indicates 'empty' or 'rest'.
    int m_NoteVelocity;      // -1 indicates not set (so use value from elsewhere)

    Note(int n = -1, int v = -1):
        m_NoteNumber(n),
        m_NoteVelocity(v)
    {}

    std::string ToString();
    void FromString(std::string);

    static std::string NoteName(int n); // Expose the notes/numbers look up table.
    static int NoteNumberFromString(std::string note);


};

struct Chord
{

    std::vector<Note> m_Notes;
    int m_StepLength;           // This will be filled in at some point to indicate how many rests follow.

    Chord():
        m_StepLength(1)
    { }

    void Add(int n, int v = -1) { m_Notes.push_back(Note(n, v)); }

    void Clear() { m_Notes.clear(); }
    bool Empty() { return m_Notes.empty(); }
    bool IsRest();

    int StepsTillNextNote() { return m_StepLength; }

    std::string ToString();
    void FromString(std::string s);
};

struct NoteList
{

    std::vector<Chord>::size_type m_Pos;                 // Points to the next position to be retrieved.
    std::vector<Chord>::size_type m_LastRequestedPos;    // Last position for which note info was requested.
    std::vector<Chord> m_Chords;

    std::vector<PosInfo> m_PosInfo; // Store string element offsets and lengths for highlighting.

    // std::string m_Label;

    bool m_Complete;

    NoteList():
        m_Pos(0),
        m_LastRequestedPos(0),
        m_Complete(false)
    {
    }

    void Clear ()
    {
        m_Chords.clear();
        ResetPosition();
    }

    bool Empty()
    {
        return m_Chords.empty();
    }

    void Add(int n = -1, int v = -1)
    {
        Chord chord;
        chord.Add(n, v);
        Add(chord);
    }

    void Add(Chord & chord)
    {
        m_Chords.push_back(chord);
    }

    void DeleteLast()
    {
        if ( !m_Chords.empty() )
            m_Chords.pop_back();
    }

    // True if mpos is zero after completing a cycle.
    // False if any other position, or immediately after a reset.

    bool Complete()
    {
        return m_Complete;
    }

    Chord * Step();

    void ResetPosition()
    {
        m_Pos = 0;
        m_Complete = false;
    }

    std::string ToString();
    void FromString(std::string s);

    bool PlayPositionInfo(int & offset,  int & length);
};


struct Pattern
{
    std::vector<int>::size_type m_Pos;
    std::vector<int>::size_type m_PosEdit;
    std::vector<int>::size_type m_LastRequestedPos;

    std::vector<int> m_Trigs;

    std::vector<NoteList> m_ListSet;

    std::string m_Label;

    double m_StepValue;
    double m_Gate;
    bool m_GateHold;
    unsigned char m_Velocity;
    TranslateTable m_TranslateTable;
    FeelMap m_FeelMap;

    void StoreStepValue( double val );
    void StoreGate( double gate);
    void StoreGateHold( bool val );
    void StoreVelocity( unsigned char vel );
    void StoreTranslateTable( TranslateTable & table );

    double StepValue() { return m_StepValue; }
    double Gate() { return m_Gate; }
    bool GateHold() { return m_GateHold; }
    unsigned char Velocity() { return m_Velocity; }
    TranslateTable & PatternTranslateTable() { return m_TranslateTable; }
    FeelMap & PatternFeelMap() { return m_FeelMap; }

    Pattern():
        m_Pos(0),
        m_PosEdit(0),
        m_LastRequestedPos(0),
        m_StepValue(4),
        m_Gate(0.5),
        m_GateHold(false),
        m_Velocity(64)
    {
        NewList();
    }

    void Clear ()
    {
        m_Label.clear();
        m_Trigs.clear();
        m_ListSet.clear();
        ResetPosition();
        m_TranslateTable.Reset();
        m_StepValue = 4.0;
        m_Gate = 0.5;
        m_GateHold = false;
        m_Velocity = 64;
    }

    void SetEditPos( std::vector<int>::size_type p )
    {
        if ( p >= 0 && p < m_ListSet.size() )
            m_PosEdit = p;
    }

    void SetStepValue( double val )
    {
        m_StepValue = val;
    }

    int ListCount()
    {
        return m_ListSet.size();
    }

    void ResetPosition()
    {
        m_Pos = 0;
        for ( std::vector<NoteList>::iterator i = m_ListSet.begin(); i != m_ListSet.end(); i++ )
            (*i).ResetPosition();
    }

    Chord * Step();

    bool LabelEmpty()
    {
        return m_Label.empty();
    }

    std::string Label(size_t width);
    std::string ListToString(std::vector<int>::size_type n);
    std::string ToString();
    bool FromString(std::string s, int & updates);
    void AddListFromString(std::vector<int>::size_type index, std::string s);
    void SetFieldsFromString(std::string s);
    bool PlayPositionInfo(int & listIndex, int & offset, int & length);

    void ReplaceList(NoteList & noteList);
    int NewList();
    void DeleteCurrentList();

    bool AllListsComplete();

    void UpEditPos() { SetEditPos( m_PosEdit + 1); }
    void DownEditPos() { SetEditPos( m_PosEdit - 1); }

};

// Patternchain modes

#define PC_MODE_NONE 0
#define PC_MODE_NATURAL 1
#define PC_MODE_FORCED 2

// class State;

struct PatternStore : public CursorKeys
{
    std::vector<int>::size_type m_PosPlay;
    std::vector<int>::size_type m_PosEdit;
    std::vector<int>::size_type m_PosPatternChain;

    int m_PatternChainMode;
    bool m_ResetOnPatternChange;
    bool m_PhaseIsZero;
    bool m_EditFocusFollowsPlay;
    bool m_NewPatternPending;
    int m_NewPattern;
    bool m_PatternChanged;
    bool m_UsePatternPlayData;

    std::vector<int> m_PatternChain;

    std::vector<Pattern> m_Patterns;
    std::vector<Pattern> m_Deleted;

    Pattern m_DefaultPattern; // Somewhere to keep 'global' data when not using a specific pattern.

    PatternStore(/*TranslateTable & table*/):
        m_PosPlay(0),
        m_PosEdit(0),
        m_PosPatternChain(0),
        m_PatternChainMode(PC_MODE_NONE),
        m_ResetOnPatternChange(true),
        m_PhaseIsZero(false),
        m_EditFocusFollowsPlay(true),
        m_NewPatternPending(false),
        m_NewPattern(-1),
        m_PatternChanged(false),
        m_UsePatternPlayData(false),
        m_PatternStoreFocus(psf_pattern)
    {
        // m_PatternChain = {0, 0, 0, 1};
    };

    std::string ToString();
    void FromString(const char * s);

    std::string PatternStatus();
    std::string PatternOverview();

    int PatternCount()
    {
        return m_Patterns.size();
    }

    void SetNewPatternPending( int val )
    {
        m_NewPattern = val;
        m_NewPatternPending = true;
    }

    bool NewPatternPending()
    {
        if ( m_NewPatternPending )
        {
            SetPlayPos(m_NewPattern);
            m_NewPatternPending = false;
            return true;
        }
        else
            return false;
    }

    void SetEditFocusFollowsPlay(bool bVal)
    {
        m_EditFocusFollowsPlay = bVal;
        if ( m_EditFocusFollowsPlay )
            m_PosEdit = m_PosPlay;
    }

    bool EditFocusFollowsPlay()
    {
        return m_EditFocusFollowsPlay;
    }

    void SetResetOnPatternChange(bool bVal)
    {
        m_ResetOnPatternChange = bVal;
    }

    void SetPatternChainMode(int bVal)
    {
        m_PatternChainMode = bVal;
    }

    bool EditPatternIsPlayPattern()
    {
        return m_PosEdit == m_PosPlay;
    }

    void SetStepValCurrentEditPattern(int i)
    {
        if ( !m_Patterns.empty() )
            m_Patterns.at(m_PosEdit).SetStepValue(i);
    }

    double StepValueCurrentPlayPattern()
    {
        if ( m_Patterns.empty() )
            return 0;
        else
            return m_Patterns.at(m_PosPlay).StepValue();
    }

    void SetPhaseIsZero()
    {
        m_PhaseIsZero = true;
    }

    Chord * Step();

    std::string EditPatternToString();
    std::string PlayPatternToString();

    int PlayPatternListCount();
    std::string PlayPatternListToString(int n);
    bool PlayPositionInfo(int & listIndex, int & offset, int & length);

    std::string PatternChainToString();
    void UpdatePattern(NoteList & noteList);
    void SetFieldsFromString(std::string s);
    bool LoadFromString(std::string s, int & created, int & updates);
    void UpdatePatternChainFromString(std::string s);

    NoteList & CurrentEditNoteList();
    Pattern & CurrentPlayPattern();
    Pattern & CurrentEditPattern();

    int CurrentEditPatternID() { return m_PosEdit + 1; }
    int CurrentPlayPatternID() { return m_PosPlay + 1; }

    int AddEmptyPattern()
    {
        m_Patterns.push_back(m_DefaultPattern);
        if ( m_EditFocusFollowsPlay )
            return m_Patterns.size() - 1;
        else
            return m_PosEdit = m_Patterns.size() - 1;
    }

    int CopyCurrentPattern()
    {
        m_Patterns.push_back(m_Patterns.at(m_PosEdit));
        m_Patterns.back().ResetPosition();
        if ( m_EditFocusFollowsPlay )
            return m_Patterns.size() - 1;
        else
            return m_PosEdit = m_Patterns.size() - 1;
    }

    void ClearCurrentPattern()
    {
        m_Deleted.push_back(m_Patterns.at(m_PosEdit));
        m_Patterns.at(m_PosEdit).Clear();
    }

    void DeleteCurrentPattern()
    {
        if ( m_Patterns.empty() )
            return;

        m_Deleted.push_back(m_Patterns.at(m_PosEdit));
        m_Patterns.erase(m_Patterns.begin() + m_PosEdit);

        if ( m_Patterns.empty() )
        {
            m_PosEdit = 0;
            m_PosPlay = 0;
            return;
        }

        // If the play pointer is above the pattern that was deleted,
        // move it down to keep it with the pattern it points at.
        //
        // Or, if the play pointer was pointing at the last pattern in
        // the list and that was deleted, it needs to point to the item
        // that's now at the end of the list.
        //
        // (If it was pointing at the pattern that was deleted, it now
        // points to the one that took its place.)

        if ( m_PosPlay > m_PosEdit || m_PosPlay == m_Patterns.size() )
            m_PosPlay -= 1;

        // The edit pointer stays in place and now points to next in
        // list (unless it was already at the end of the list).

        if ( m_PosEdit == m_Patterns.size() )
            m_PosEdit -= 1;

    }

    void DeleteAllPatterns()
    {
        while ( !m_Patterns.empty() )
        {
            m_Deleted.push_back(m_Patterns.back());
            m_Patterns.erase(m_Patterns.end());
        }

        m_PosEdit = 0;
        m_PosPlay = 0;
    }

    void PopDeletedPattern()
    {
        if ( m_Deleted.empty() )
            throw std::string("There are no deleted patterns to restore.");

        m_Patterns.push_back(m_Deleted.back());
        m_Deleted.pop_back();

        if ( !m_EditFocusFollowsPlay )
            m_PosEdit = m_Patterns.size() - 1;
    }

    bool PatternChanged(bool resetValue = false)
    {
        bool result = m_PatternChanged;
        if ( resetValue )
            m_PatternChanged = false;
        return result;
    }

    void SetPlayPos( std::vector<int>::size_type p )
    {
        if ( p >= 0 && p < m_Patterns.size() )
        {
            m_PosPlay = p;
            if ( m_EditFocusFollowsPlay && m_PatternChainMode == PC_MODE_NONE )
                m_PosEdit = m_PosPlay;
            m_PatternChanged = true; // Cleared again at the start of Step() ..
        }

        if ( m_ResetOnPatternChange )
            m_Patterns.at(m_PosPlay).ResetPosition();

    }

    void SetEditPos( std::vector<int>::size_type p )
    {
        if ( p >= 0 && p < m_Patterns.size() )
        {
            m_PosEdit = p;
            m_EditFocusFollowsPlay = false;
        }
    }

    void UpEditPos() { SetEditPos( m_PosEdit + 1); }
    void DownEditPos() { SetEditPos( m_PosEdit - 1); }
    void UpListPos();
    void DownListPos();

    bool ValidPosition( std::vector<int>::size_type p )
    {
        return p >= 0 && p < m_Patterns.size();
    }

    void ResetCurrentPattern()
    {
        if ( !m_Patterns.empty() )
            m_Patterns.at(m_PosEdit).ResetPosition();
    }

    void ResetAllPatterns()
    {
        for ( std::vector<Pattern>::iterator i = m_Patterns.begin(); i != m_Patterns.end(); i++ )
            (*i).ResetPosition();
        m_PosPatternChain = -1; // This seems odd, but it's incremented immediately on phase
    }

    void StorePatternPlayData( /*State & state, TranslateTable & table,*/ unsigned char mask = PLAY_DATA_ALL);
    std::string LoadPatternPlayData( /*State & state, TranslateTable & table,*/ unsigned char mask = PLAY_DATA_ALL );
    std::string ShowPatternPlayData();

    void SetStepValue(double step);
    void SetGateLength(double gate);
    void SetGateHold(bool val);
    void SetNoteVelocity(unsigned char vel);

    double StepValue();
    double GateLength();
    bool GateHold();
    unsigned char NoteVelocity();

    TranslateTable & CurrentTranslateTableForEdit(bool setFocus = true);
    TranslateTable & CurrentTranslateTableForPlay();

    FeelMap & CurrentFeelMapForEdit(bool setFocus = true);
    FeelMap & CurrentFeelMapForPlay();

    void SetUsePatternPlayData( bool val );
    bool UsePatternPlayData() { return m_UsePatternPlayData; }

    std::string ListManager(std::string commandString, std::vector<std::string> & tokens);

        virtual void SetFocus()
        {
            CursorKeys::SetFocus();
            SetStatus();
        }
    protected:
        enum pattern_store_focus_t {
            psf_pattern,
            psf_list,
            number_psf_focus_modes
        };

        void SetStatus();

        virtual bool HandleKey(key_type_t k);
        pattern_store_focus_t m_PatternStoreFocus;

};

#endif // PATTERN_STORE_H_INCLUDED
