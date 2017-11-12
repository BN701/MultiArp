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

#include "maState.h"

State::State():
    m_CurrentStepValue(16.0),
    m_Quantum(16.0),
    m_RunState(false),
    m_Beat(-4/m_CurrentStepValue),
    m_Phase(0),
    m_NewRunStatePending(false),
    m_NewRunState(false),
    m_NewQuantumPending(false),
    m_NewQuantum(0),
    m_DeferStop(0)
{
    //ctor
}

State::~State()
{
    //dtor
}

void State::Step(double stepValueMultiplier)
{
    m_LastUsedStepValue = stepValueMultiplier * m_CurrentStepValue;
    m_Beat += 4 / m_LastUsedStepValue;
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

