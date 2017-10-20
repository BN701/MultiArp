#include "maState.h"
// #include "maPatternStore.h"

State::State():
    m_CurrentStepValue(4.0),
//    m_StepValueFromPattern(0),
//    m_validValueFromPattern(false),
//    m_Transpose(0),
    m_Quantum(4.0),
//    m_GateLength(1.0),
//    m_NoteVelocity(64),
    m_RunState(false),
//    m_Step(-1),
    m_Beat(-4/m_CurrentStepValue),
    m_Phase(0),
//    m_ExtendGateOverRests(false),
//    m_Root(60), // Middle C
//    m_Scale(SCALE_CHROMATIC),
    m_NewRunStatePending(false),
    m_NewRunState(false),
//    m_NewStepValuePending(false),
//    m_NewStepValue(0),
//    m_NewTransposePending(false),
//    m_NewTranspose(0),
    m_NewQuantumPending(false),
    m_NewQuantum(0),
//    m_UsePatternPlayData(true),
    m_DeferStop(0)
{
    //ctor
}

State::~State()
{
    //dtor
}

void State::Step()
{
//    m_Step++;
//    if ( m_UsePatternPlayData && m_validValueFromPattern )
//        m_Beat += 4 / m_StepValueFromPattern;
//    else
//        m_Beat += 4 / m_StepValue;

    m_Beat += 4 / m_CurrentStepValue;
}

//double State::StepValueInUse()
//{
//    if ( m_UsePatternPlayData && m_validValueFromPattern )
//        return m_StepValueFromPattern;
//    else
//        return m_StepValue;
//}

//void State::SetStepValueFromPattern(double val)
//{
//    m_StepValueFromPattern = val;
//    m_validValueFromPattern = true;
//}
//
//
//void State::ClearStepValueFromPattern()
//{
//    m_StepValueFromPattern = 0;
//    m_validValueFromPattern = false;
//}
//


//double State::StepLengthmSec(double tempo)
//{
//    /*
//          V, m_StepValue, is 4 x 'steps per beat'. (This gives the familiar
//          eighth, sixteenths, etc). T, tempo, is 'beats per minute'.
//
//          Steps per beat, v = V/4.
//          Steps per minute = Tv = TV/4
//          Steps per second = TV/240
//          Step length in mSec = 1000*240/TV
//     */
//
//    return 240000.0/(tempo * m_CurrentStepValue);
//}
//
//double State::GateLengthmSec(double tempo, double gate)
//{
//    if ( gate > 0 )
//        return gate * StepLengthmSec(tempo);
//    else
//        return m_GateLength * StepLengthmSec(tempo);
//}
//
//double State::CalculateNoteDuration(double tempo, int extraSteps, double gate, bool extendOverride)
//{
//    if ( extendOverride || m_ExtendGateOverRests )
//        return extraSteps * StepLengthmSec(tempo) + GateLengthmSec(tempo, gate);
//    else
//        return GateLengthmSec(tempo, gate);
//}
//
void State::SetNewRunStatePending( bool val, int defer )
{
    m_NewRunStatePending = true;
    m_NewRunState = val;
    m_DeferStop = defer;
}


bool State::NewRunStatePending()
{
    if ( m_NewRunStatePending )
    {
        m_RunState = m_NewRunState;
        m_NewRunStatePending = false;
        return true;
    }
    else
        return false;
}

//void State::SetNewStepValuePending( double val )
//{
//    m_NewStepValuePending = true;
//    m_NewStepValue = val;
//}
//
//bool State::NewStepValuePending()
//{
//    if ( m_NewStepValuePending )
//    {
//        m_StepValue = m_NewStepValue;
//        m_NewStepValuePending = false;
//        return true;
//    }
//    else
//        return false;
//}
//
//void State::SetNewTransposePending( int val )
//{
//    m_NewTransposePending = true;
//    m_NewTranspose = val;
//}
//
//bool State::NewTransposePending()
//{
//    if ( m_NewTransposePending )
//    {
//        m_Transpose = m_NewTranspose;
//        m_NewTransposePending = false;
//        return true;
//    }
//    else
//        return false;
//}

void State::SetNewQuantumPending( double val )
{
    m_NewQuantumPending = true;
    m_NewQuantum = val;
}

bool State::NewQuantumPending()
{
    if ( m_NewQuantumPending )
    {
        m_Quantum = m_NewQuantum;
        m_NewQuantumPending = false;
        return true;
    }
    else
        return false;
}

