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
#include <map>
#include <memory>

#include "maChainLink.h"
#include "maCursorKeys.h"
#include "maFeelMap.h"
#include "maNotes.h"
#include "maPattern.h"
#include "maPatternChain.h"


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
    bool m_EditPosFollowsPlay;
    bool m_NewPatternPending;
    int m_NewPattern;
    bool m_PatternChanged;
    bool m_UsePatternPlayData;

    PatternChain m_PatternChain;

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
        m_EditPosFollowsPlay(true),
        m_NewPatternPending(false),
        m_NewPattern(-1),
        m_PatternChanged(false),
        m_UsePatternPlayData(false),
        m_PatternStoreFocus(psf_pattern)
    {
        // m_PatternChain = {0, 0, 0, 1};
    };

    bool Empty() { return m_Patterns.empty(); }
    bool PatternChainEmpty() { return m_PatternChain.empty(); }
    std::string ToString();
    void FromString(const char * s);

    std::string PatternStatus();
    std::string PatternOverview();

    int PatternCount()
    {
        return m_Patterns.size();
    }

    std::string SetNewPatternOrJump( int val );
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
        m_EditPosFollowsPlay = bVal;
        if ( m_EditPosFollowsPlay )
            m_PosEdit = m_PosPlay;
    }

    bool EditFocusFollowsPlay()
    {
        return m_EditPosFollowsPlay;
    }

    void SetResetOnPatternChange(bool bVal)
    {
        m_ResetOnPatternChange = bVal;
    }

    void SetPatternChainMode(int bVal)
    {
        m_PatternChainMode = bVal;
    }

    int PatternChainMode() { return m_PatternChainMode; }
    int CurrentPosPatternChain() { return m_PosPatternChain; }

    bool EditPatternIsPlayPattern()
    {
        return m_PosEdit == m_PosPlay;
    }

    void SetStepValCurrentEditPattern(int i)
    {
        if ( !m_Patterns.empty() )
            m_Patterns.at(m_PosEdit).SetStepValue(i);
    }

    void SetLabelCurrentEditPattern(const char * label)
    {
        if ( !m_Patterns.empty() )
            m_Patterns.at(m_PosEdit).SetLabel(label);
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

    void Step(Cluster & cluster, double phase, double stepValue);

    std::string EditPatternToString();
    std::string PlayPatternToString();

    std::string PatternSelectionList(int start, int rows);

    int PlayPatternListCount();
    int RealTimeListCount();
    std::string RealTimeListToStringForDisplay(int n);
    std::string PlayPatternListToString(int n);
    bool PlayPositionInfo(int & listIndex, int & offset, int & length);

    std::string PatternChainToString();
    std::string PatternChainToStringForDisplay(int firstRow, int rows);

    void UpdatePattern(StepList & noteList);
    void UpdatePattern(std::map<double,Note> & realTimeList, double quantum);
    void SetFieldsFromString(std::string s);
    bool LoadFromString(std::string s, int & created, int & updates);
    void UpdatePatternChainFromSimpleString(std::string s); // Old style, from chain command, no support for jumps.
    void UpdatePatternChainFromString(std::string s);

    StepList & CurrentEditStepList();
    RealTimeList & CurrentEditRealTimeList();
    Pattern & CurrentPlayPattern();
    Pattern & CurrentEditPattern();

    int CurrentPosEdit() { return m_PosEdit; }
    int CurrentPosPlay() { return m_PosPlay; }
    int CurrentEditPatternID() { return m_PosEdit + 1; }
    int CurrentPlayPatternID() { return m_PosPlay + 1; }

    int AddEmptyPattern()
    {
        m_Patterns.push_back(m_DefaultPattern);
        m_Patterns.back().NewList();
        m_Patterns.back().SetLabel("New");


        if ( m_EditPosFollowsPlay )
            return m_Patterns.size() - 1;
        else
            return m_PosEdit = m_Patterns.size() - 1;

    }

    int CopyCurrentPattern()
    {
        m_Patterns.push_back(m_Patterns.at(m_PosEdit));
        m_Patterns.back().ResetPosition();
        m_Patterns.back().SetLabel((m_Patterns.back().Label() + ", copy").c_str());

        if ( m_EditPosFollowsPlay )
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

        if ( !m_EditPosFollowsPlay )
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
            if ( m_EditPosFollowsPlay /*&& m_PatternChainMode == PC_MODE_NONE*/ )
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
            m_EditPosFollowsPlay = false;
        }
    }

    void UpEditPos() { SetEditPos(m_PosEdit + 1); }
    void DownEditPos() { SetEditPos(m_PosEdit - 1); }
    void UpListPos();
    void DownListPos();
    void UpRTListPos();
    void DownRTListPos();

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
        m_PosPatternChain = 0; // This seems odd, but it's incremented immediately on phase
    }

    void StorePatternPlayData( unsigned char mask = PLAY_DATA_ALL);
    std::string LoadPatternPlayData( unsigned char mask = PLAY_DATA_ALL );
    std::string ShowPatternPlayData();

    void SetStepValue(double step);
    void SetGateLength(double gate);
    void SetGateHold(bool val);
    void SetNoteVelocity(unsigned char vel);

    double StepValue();
    double GateLength();
    bool GateHold();
    unsigned char NoteVelocity();

    TranslateTable & TranslateTableForEdit(bool setFocus = true);
    TranslateTable & TranslateTableForPlay();

    FeelMap & FeelMapForEdit(bool setFocus = true);
    FeelMap & FeelMapForPlay();

    PatternChain & PatternChainForEdit() { return m_PatternChain; }

    void SetUsePatternPlayData( bool val );
    bool UsePatternPlayData() { return m_UsePatternPlayData; }

    std::string ListManager(std::string commandString, std::vector<std::string> & tokens);
    void DeleteCurrentRealTimeList();

    virtual void SetStatus();
    virtual void SetFocus()
    {
        CursorKeys::SetFocus();
        // SetStatus();
    }

    protected:
        enum pattern_store_focus_t {
            psf_pattern,
            psf_list,
            psf_rt_list,
            psf_trig_list,
            number_psf_focus_modes
        };


        virtual bool HandleKey(key_type_t k);
        pattern_store_focus_t m_PatternStoreFocus;

};

#endif // PATTERN_STORE_H_INCLUDED
