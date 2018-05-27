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

#ifndef PATTERN_STORE_H_INCLUDED
#define PATTERN_STORE_H_INCLUDED

#include <vector>
#include <string>
#include <map>

#include "maChainLink.h"
#include "maCommand.h"
#include "maItemMenu.h"
#include "maFeelMap.h"
#include "maNotes.h"
#include "maPattern.h"
#include "maPatternChain.h"

// Reset modes.

#define RESET_NONE 0
#define RESET_ALL 1
#define RESET_CURRENT 2

// Patternchain modes

//#define PC_MODE_NONE 0
//#define PC_MODE_NATURAL 1
//#define PC_MODE_FORCED 2

// class State;

struct PatternStore : public ItemMenu
{
    ItemMenu m_MenuListWindow;
    std::vector<int>::size_type m_PosPlay;
    std::vector<int>::size_type m_PosEdit;
//    std::vector<int>::size_type m_PosPatternChain;

//    int m_PatternChainMode;
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
    double m_StepValueMultiplier = 1.0;

    PatternStore(/*TranslateTable & table*/):
        m_PosPlay(0),
        m_PosEdit(0),
//        m_PosPatternChain(0),
//        m_PatternChainMode(PatternChain::off),
        m_ResetOnPatternChange(true),
        m_PhaseIsZero(false),
        m_EditPosFollowsPlay(true),
        m_NewPatternPending(false),
        m_NewPattern(-1),
        m_PatternChanged(false),
        m_UsePatternPlayData(false)
    {
        m_DisplayObjectType = BaseUI::dot_pattern_store;
        m_MenuListWindow.ClearStatus();
        m_MenuListWindow.SetVisible(true);
        m_MenuListWindow.SetType(BaseUI::dot_pattern_menu_list);
        m_Visible = true;
        SetRedraw();
    };

    bool Empty() { return m_Patterns.empty(); }
    bool PatternChainEmpty() { return m_PatternChain.empty(); }

    std::string PatternStatusPlay();
    std::string PatternStatusEdit();
    std::string PatternOverview();

    int PatternCount() { return m_Patterns.size(); }

    std::string SetNewPatternOrJump( int val );
    void SetNewPatternPending( int val );
    bool NewPatternPending(bool clearAndReset = true);

    void SetEditFocusFollowsPlay(bool bVal)
    {
        m_EditPosFollowsPlay = bVal;
        if ( m_EditPosFollowsPlay )
            m_PosEdit = m_PosPlay;
    }

    bool EditFocusFollowsPlay() { return m_EditPosFollowsPlay; }
    void SetResetOnPatternChange(bool bVal) { m_ResetOnPatternChange = bVal; }
    void SetPatternChainMode(PatternChain::pattern_chain_mode_t bVal) { m_PatternChain.SetMode(bVal); }
    PatternChain::pattern_chain_mode_t PatternChainMode() { return m_PatternChain.Mode(); }
    std::vector<ChainLink>::size_type CurrentPosPatternChain() { return m_PatternChain.PosPlay(); }
    bool EditPatternIsPlayPattern() { return m_PosEdit == m_PosPlay; }

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

    void SetPhaseIsZero() { m_PhaseIsZero = true; }

    void Step(Cluster & cluster, TrigRepeater & repeater, double phase, double stepValue, double globalBeat);
    double StepValueMultiplier() { return m_StepValueMultiplier; }

    std::string EditPatternToString();

    std::string PatternSelectionList(unsigned start, unsigned rows);

    std::string PatternChainToString();
    std::string PatternChainToStringForDisplay(int firstRow, int rows);

    std::string ToString();
    bool FromString(std::string s, int & created, int & updates);
    void UpdatePattern(StepList & noteList);
    void UpdatePattern(std::map<double,Note> & realTimeList, double quantum);
#ifndef MA_BLUE
    void UpdatePatternFromMidiFile(std::string s);
#endif
    void SetFieldsFromString(std::string s);
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

    double LastRealTimeBeat();

    int AddEmptyPattern(std::vector<std::string> & tokens, int firstToken);
    int CopyCurrentPattern();
    void ClearCurrentPattern()
    {
        m_Deleted.push_back(m_Patterns.at(m_PosEdit));
        m_Patterns.at(m_PosEdit).Clear();
    }

    void DeleteCurrentPattern();
    void DeleteAllPatterns();
    void PopDeletedPattern();

    bool PatternChanged(bool resetValue = false)
    {
        bool result = m_PatternChanged;
        if ( resetValue )
            m_PatternChanged = false;
        return result;
    }

    void SetPlayPos( std::vector<int>::size_type p );
    void SetEditPos( std::vector<int>::size_type p );
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
        m_PatternChain.ResetPosPlay();
//        m_PosPatternChain = 0; // This seems odd, but it's incremented immediately on phase
    }

    void StorePatternPlayData( unsigned char mask = PLAY_DATA_ALL );
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
        ItemMenu::SetFocus();
    }

    void OpenCurrentItemMenu();

    protected:
        virtual bool HandleKey(BaseUI::key_command_t k);

};

extern PatternStore g_PatternStore;

#endif // PATTERN_STORE_H_INCLUDED
