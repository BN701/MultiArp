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

#ifndef MARTLIST_H_INCLUDED
#define MARTLIST_H_INCLUDED

#include <vector>
#include <string>
#include <map>

#include "maItemMenu.h"
#include "maNotes.h"

struct RealTimeListParams : public ItemMenu
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

    bool m_EchoesWrapAtQuantum = false;
    bool m_EchoesDeleteAtQuantum = true;

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

        virtual bool HandleKey(BaseUI::key_command_t k);
        rt_params_menu_focus_t m_RTParamsFocus = rtp_loop_start;

};


struct RealTimeList : public ItemMenu
{
    static RealTimeList EmptyList;
    std::map<double,Note> m_RealTimeList;

//    bool m_Complete = false;
//    double m_InternalBeat = 0;

    double m_LastRequestedStepValue = 4.0;
    double m_LastRequestedPhase = 0.0;
    bool m_LastStepPhaseZero = false;

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
    bool PhazeIsZero() { return m_LastStepPhaseZero; }

    void FromString(std::string s);
    std::string ToString();
    std::string ToStringForDisplay(int & offset, int & length, unsigned width = 75);

    void SetMultiplier( double val ) { m_Params.m_Multiplier = val; }
    double AdjustedQuantum() { return m_Params.m_Multiplier * m_Params.m_Quantum; }
    unsigned long PhaseLength();
    void SetQuantum( double val ) { m_Params.m_Quantum = val; }

    bool NoteTargetAction(std::map<double,Note>::iterator mapEntry, double newPhase);
    std::map<double,Note>::iterator MoveNote(std::map<double, Note>::iterator, double newPhase);
    std::map<double,Note>::iterator CopyNote(std::map<double, Note>::iterator);
    void BeginEcho(double inc, double target, int interval);
    void DoEchoes();

    virtual void SetStatus();
    protected:

        virtual bool HandleKey(BaseUI::key_command_t k);
        int m_RTListFocus = 0;
        int m_SubMenus = 1; // How many submenu items will there be in the ItemMenu menu?

        RealTimeListParams m_Params;

        std::vector<Note> m_UndoList;
};



#endif // MARTLIST_H_INCLUDED
