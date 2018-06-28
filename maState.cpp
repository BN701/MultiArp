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

#include <cmath>
#include "maState.h"
#include "maUtility.h"

using namespace std;

#ifdef MA_BLUE

#include <math.h>
#include "maSequencer.h"

#else

#define LINK_PLATFORM_LINUX
#include <ableton/Link.hpp>

// Global Link instance.

extern ableton::Link g_Link;
extern chrono::microseconds g_LinkStartTime;

#include "platform_Linux/maAlsaSequencer.h"

#endif // MA_BLUE


State g_State;  // Global State instance.

State::State()
{
    //ctor
}

State::~State()
{
    //dtor
}

int64_t State::TimeAtBeat(double beat)
{
#if defined(MA_BLUE)
    return (beat - m_BeatLastTempoChange) * 60000000/m_Tempo + m_TimeLastTempoChange;
#else
//    ableton::Link::Timeline timeline = g_Link.captureAppTimeline();
    ableton::Link::SessionState timeline = g_Link.captureAppSessionState();
    chrono::microseconds t_next_usec = timeline.timeAtBeat(beat, m_Quantum);

    if ( g_LinkStartTime.count() < 0 )
    {
       g_LinkStartTime = t_next_usec;
       return 0;
    }
    else
    {
       return t_next_usec.count() - g_LinkStartTime.count();
    }
#endif
}

double State::Tempo()
{
#if defined(MA_BLUE)
    return m_Tempo;
#else
//    ableton::Link::Timeline timeline = g_Link.captureAppTimeline();
    ableton::Link::SessionState timeline = g_Link.captureAppSessionState();
    return timeline.tempo();
#endif
}

double State::PhaseAtBeat(double beat)
{
    return fmod(beat, m_Quantum);
}


void State::Step(double stepValueMultiplier)
{
    m_LastUsedStepValue = m_CurrentStepValue;
    double beatInc = 4.0 * stepValueMultiplier / m_LastUsedStepValue;
    m_Beat += beatInc;
#if defined(MA_BLUE)
    m_TimeLineMicros += beatInc * 60000000 / m_Tempo;
#endif
//    m_Beat += 4 / m_CurrentStepValue;
}

void State::Progress(double & progress, double & stepWidth)
{
    progress = m_Phase/m_Quantum;
    stepWidth =  4.0/(m_CurrentStepValue * m_Quantum);
}

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

bool State::PhaseIsZero()
{
    return lround(1000.0 * m_Phase) == 0;
}

//bool State::LastStep()
//{
//    return lround(1000.0 * (m_Phase + 4/m_CurrentStepValue - m_Quantum)) == 0;
//}

#if defined(MA_BLUE)
double State::BeatFromEvent(snd_seq_event_t *ev)
{
//    uint64_t tMicros = static_cast<int64_t>(ev->time.time.tv_sec) * 1000000 + ev->time.time.tv_nsec / 1000;
    uint64_t tMicros = EventTimeToMicros(*ev);
    double tDelta = (tMicros - m_TimeLineMicros)/60000000.0;
    return m_Beat + tDelta * m_Tempo;
}
#endif

