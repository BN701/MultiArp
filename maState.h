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

        void Step();

//        /** Clear m_StepValueOverride
//         * \return Clear current value of m_StepValueOverride
//         */
//        void ClearStepValueFromPattern();
//        /** Set m_StepValueOverride
//         * \param val New value to set
//         */
//        void SetStepValueFromPattern(double val);
//        /** Access m_UseGlobalPlayData
//         * \return The current value of m_UseGlobalPlayData
//         */
//        bool UsePatternPlayData() { return m_UsePatternPlayData; }
//        /** Set m_StepValue
//         * \param val New value to set
//         */
//        void SetUsePatternPlayData(bool val) { m_UsePatternPlayData = val; }
        /** Access m_StepValue
         * \return The current value of m_StepValue
         */
        double CurrentStepValue() { return m_CurrentStepValue; }
        /** Set m_StepValue
         * \param val New value to set
         */
        void SetCurrentStepValue(double val)
        {
            if ( val <= 0 ) // !!!
                return;
            m_CurrentStepValue = val;
        }
//        /** Access m_Transpose
//         * \return The current value of m_Transpose
//         */
//        int Transpose() { return m_Transpose; }
//        /** Set m_Transpose
//         * \param val New value to set
//         */
//        void SetTranspose(int val) { m_Transpose = val; }
        /** Access m_Quantum
         * \return The current value of m_Quantum
         */
        double Quantum() { return m_Quantum; }
        /** Set m_Quantum
         * \param val New value to set
         */
        void SetQuantum(double val) { m_Quantum = val; }
//        /** Access m_GateLength
//         * \return The current value of m_GateLength
//         */
//        double GateLength() { return m_GateLength; }
//        /** Set m_GateLength
//         * \param val New value to set
//         */
//        void SetGateLength(double val) { m_GateLength = val; }
//        /** Access m_NoteVelocity
//         * \return The current value of m_NoteVelocity
//         */
//        unsigned int NoteVelocity() { return m_NoteVelocity; }
//        /** Set m_NoteVelocity
//         * \param val New value to set
//         */
//        void SetNoteVelocity(unsigned char val) { m_NoteVelocity = val; }
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

//        void SetExtendGateOverRests(bool val) { m_ExtendGateOverRests = val; }
//        bool ExtendGateOverRests() { return m_ExtendGateOverRests; }

        /*
            These accessors are used as once-only pairs. A new value is set and
            a flag is set. WHen the flag is checked the new value is applied and
            the flag is cleared.
         */

        void SetNewRunStatePending(bool val, int defer = 0);
        bool NewRunStatePending();
        int DeferStop() { return m_DeferStop; }

//        void SetNewStepValuePending( double val );
//        bool NewStepValuePending();

//        void SetNewTransposePending( int val );
//        bool NewTransposePending();

        void SetNewQuantumPending( double val );
        bool NewQuantumPending();

        // Apply state variables to notes and other things.

//        double StepLengthmSec(double tempo);
//        double GateLengthmSec(double tempo, double gate = -1.0);
//        double CalculateNoteDuration(double tempo, int extraSteps, double gate = -1.0, bool extendOverride = false);
//        double StepValueInUse();

        // int Transpose(int n) { return m_TransposeTables.Transpose(m_Root, m_Scale, m_Transpose, n); };
        // int Transpose(int n) { return n + m_Transpose; };

    protected:

    private:

        double m_CurrentStepValue;
        // double m_StepValue; //!< Member variable "m_StepValue"
//        double m_StepValueFromPattern;
//        bool m_validValueFromPattern;
        double m_Quantum; //!< Member variable "m_Quantum"
//        double m_GateLength; //!< Member variable "m_GateLength"
//        unsigned char m_NoteVelocity; //!< Member variable "m_NoteVelocity"
        bool m_RunState; //!< Member variable "m_RunState"
        // int64_t m_Step; //!< Member variable "m_Step"
        double m_Beat; //!< Member variable "m_Beat"
        double m_Phase; //!< Member variable "m_Phase"
        int m_PatternReset; //!<Member variable "m_PatternReset"
//        bool m_ExtendGateOverRests;

//        bool m_UsePatternPlayData;

        // int m_Root;
        // int m_Scale;
        // int m_Transpose; //!< Member variable "m_Transpose"

        bool m_NewRunStatePending;
        bool m_NewRunState;
//        bool m_NewStepValuePending;
//        double m_NewStepValue;
//        bool m_NewTransposePending;
//        int m_NewTranspose;
        bool m_NewQuantumPending;
        int m_NewQuantum;

        int m_DeferStop;

};

#endif // STATE_H
