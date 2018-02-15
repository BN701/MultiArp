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

#ifndef STATE_H
#define STATE_H

#include<inttypes.h>
#include<string>

#if defined(MA_BLUE)
#include "alsa_types.h"
#endif

#include "maTranslateTable.h"

class State
{
    public:
        State();
        virtual ~State();

        void Step(double stepValueMultiplier);
        void Progress(double & progress, double & stepWidth);

        double CurrentStepValue() { return m_CurrentStepValue; }
        double LastUsedStepValue() { return m_LastUsedStepValue; }

        void SetCurrentStepValue(double val)
        {
            if ( val <= 0 ) // !!!
                return;
            m_CurrentStepValue = val;
        }

        double Quantum() { return m_Quantum; }
        void SetQuantum(double val) { m_Quantum = val; }
        bool NewQuantumPending();
        void SetNewQuantumPending( double val );


        void SetRunState(bool val) { m_RunState = val; }
        bool RunState() { return m_RunState; }
        void SetNewRunStatePending(bool val, int defer = 0);
        bool NewRunStatePending();

        int DeferStop() { return m_DeferStop; }

        double Beat() { return m_Beat; }
        void SetBeat(double val) { m_Beat = val; }

        bool PhaseIsZero();
        double Phase() { return m_Phase; }
        void SetPhase(double val) { m_Phase = val; }

        int PatternReset() { return m_PatternReset; }
        void SetPatternReset(int val) { m_PatternReset = val; }

    protected:

    private:

        double m_CurrentStepValue;
        double m_LastUsedStepValue;
        double m_Quantum;
        bool m_RunState;
        double m_Beat;
        double m_Phase;
        int m_PatternReset;

        bool m_NewRunStatePending;
        bool m_NewRunState;
        bool m_NewQuantumPending;
        int m_NewQuantum;

        int m_DeferStop;

#if defined(MA_BLUE)
    public:

        double Tempo() { return m_Tempo; }
        void SetTempo( double val ) { m_Tempo = val; }

        double TimeLineMicros() { return m_TimeLineMicros; }

        double BeatFromEvent(snd_seq_event_t *ev);

    private:
        double m_Tempo = 120.0;
        double m_TimeLineMicros = 0;  // Incremented by every call to step, tracking beat position, adjusted for tempo.
#endif

};

#endif // STATE_H
