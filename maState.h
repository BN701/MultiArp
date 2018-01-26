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

#include "maTranslateTable.h"

class State
{
    public:
        /** Default constructor */
        State();
        /** Default destructor */
        virtual ~State();

        void Step(double stepValueMultiplier);

        void Progress(double & progress, double & stepWidth);
        /** Access m_StepValue
         * \return The current value of m_StepValue
         */
        double CurrentStepValue() { return m_CurrentStepValue; }
        double LastUsedStepValue() { return m_LastUsedStepValue; }
        /** Set m_StepValue
         * \param val New value to set
         */
        void SetCurrentStepValue(double val)
        {
            if ( val <= 0 ) // !!!
                return;
            m_CurrentStepValue = val;
        }
        /** Access m_Quantum
         * \return The current value of m_Quantum
         */
        double Quantum() { return m_Quantum; }
        /** Set m_Quantum
         * \param val New value to set
         */
        void SetQuantum(double val) { m_Quantum = val; }
        /** Access m_RunState
         * \return The current value of m_RunState
         */
        bool RunState() { return m_RunState; }
        /** Set m_RunState
         * \param val New value to set
         */
        void SetRunState(bool val) { m_RunState = val; }
        /** Access m_Beat
         * \return The current value of m_Beat
         */
        double Beat() { return m_Beat; }
        /** Set m_Beat
         * \param val New value to set
         */
        void SetBeat(double val) { m_Beat = val; }
        /** Access m_Step
         * \return The current value of m_Step
         */
        // int64_t Step() { return m_Step; }
        /** Set m_Step
         * \param val New value to set
         */
        // void SetStep(int64_t val) { m_Step = val; } // Don't think we ever use this directly.
        /** Access m_Phase
         * \return The true if value of m_Phase is zero
         */
        double PhaseIsZero() { return m_Phase == 0; } // !!!!
        /** Access m_Phase
         * \return The current value of m_Phase
         */
        double Phase() { return m_Phase; }
        /** Set m_Phase
         * \param val New value to set
         */
        void SetPhase(double val) { m_Phase = val; }
        /** Access m_PatternReset
         * \return The current value of m_PatternReset
         */
        int PatternReset() { return m_PatternReset; }
        /** Set m_PatternReset
         * \param val New value to set
         */
        void SetPatternReset(int val) { m_PatternReset = val; }

        /*
            These accessors are used as once-only pairs. A new value is set and
            a flag is set. When the flag is checked the new value is applied and
            the flag is cleared.
         */

        void SetNewRunStatePending(bool val, int defer = 0);
        bool NewRunStatePending();
        int DeferStop() { return m_DeferStop; }

        void SetNewQuantumPending( double val );
        bool NewQuantumPending();

    protected:

    private:

        double m_CurrentStepValue;
        double m_LastUsedStepValue;
        double m_Quantum; //!< Member variable "m_Quantum"
        bool m_RunState; //!< Member variable "m_RunState"
        double m_Beat; //!< Member variable "m_Beat"
        double m_Phase; //!< Member variable "m_Phase"
        int m_PatternReset; //!<Member variable "m_PatternReset"

        bool m_NewRunStatePending;
        bool m_NewRunState;
        bool m_NewQuantumPending;
        int m_NewQuantum;

        int m_DeferStop;

};

#endif // STATE_H
