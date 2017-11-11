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

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>


#include "maFeelMap.h"
#include "maUtility.h"

using namespace std;

#define LOG_ON 0

#if LOG_ON
ofstream fLog;
#endif

FeelMap::FeelMap():
    m_Active(true),
    m_EditPoint(1)
{
    //ctor
#if LOG_ON
    fLog.open("FeelMap.log");
#endif

#if 0
    m_StretchPoints.push_back(0.0);
    m_StretchPoints.push_back(0.6);
    m_StretchPoints.push_back(1.0); // Do we need the last one, it's always 1.0, after all.
    m_Points = 2;
#endif

#if 0
    m_StretchPoints.push_back(0.0);
    m_StretchPoints.push_back(0.5);
    m_StretchPoints.push_back(0.75);
    m_StretchPoints.push_back(1.0); // Do we need the last one, it's always 1.0, after all.
    m_Points = 3;
#endif

#if 0
    m_StretchPoints.push_back(0.0);
    m_StretchPoints.push_back(0.3);
    m_StretchPoints.push_back(0.6);
    m_StretchPoints.push_back(1.0); // Do we need the last one, it's always 1.0, after all.
    m_Points = 3;
#endif

#if 0
    m_StretchPoints.push_back(0.0);
    m_StretchPoints.push_back(0.30);
    m_StretchPoints.push_back(0.5);
    m_StretchPoints.push_back(0.80);
    m_StretchPoints.push_back(1.0); // Do we need the last one, it's always 1.0, after all.
    m_Points = 4;
#endif

}

FeelMap::~FeelMap()
{
    //dtor
#if LOG_ON
    fLog.close();
#endif
}

void FeelMap::New(vector<string> & tokens)
{
    m_StretchPoints.clear();
    m_StretchPoints.push_back(0.0);
    m_StretchPoints.push_back(1.0);

    if ( tokens.size() > 2 )
    {
        for ( int i = 2; i < tokens.size(); i++ )
        {
            try
            {
                double t = stod(tokens.at(i));

                // Check if we're creating a default array of size 't',
                // (Don't allow higher than 12, mostly because we
                // can't fit any more on one line of the display.)

                if ( i == 2 && t > 1 && t < 13 )
                {
                    m_Points = t;
                    m_StretchPoints.resize(m_Points + 1);
                    m_EditPoint = 1;
                    Respace();
                    return;
                }

                if ( t > 0 && t < 1.0 )
                    m_StretchPoints.push_back(t); // We'll sort them after the loop.
            }
            catch(...)
            {
                // Invalid parameter. Do nothing and try the next one.
            }
        }
        sort(m_StretchPoints.begin(), m_StretchPoints.end());
    }

    m_Points = m_StretchPoints.size() - 1;

    if ( m_Points > 1 )
        m_EditPoint = 1;
    else
        m_EditPoint = 0;

    SetStatus();
}

void FeelMap::Add()
{
    double val = m_StretchPoints.at(m_EditPoint) + (m_StretchPoints.at(m_EditPoint + 1) - m_StretchPoints.at(m_EditPoint))/2;

    vector<double>::iterator it = m_StretchPoints.begin();
    m_StretchPoints.insert(it + m_EditPoint + 1, val);

    m_Points = m_StretchPoints.size() - 1;
    m_EditPoint += 1;
    SetStatus();
}

void FeelMap::Remove()
{
    if ( m_Points <= 1 )
        return;

    vector<double>::iterator it = m_StretchPoints.begin();
    m_StretchPoints.erase(it + m_EditPoint);

    m_Points = m_StretchPoints.size() - 1;
    if ( m_EditPoint == m_Points )
        m_EditPoint -= 1;
    SetStatus();
}

void FeelMap::Respace()
{
    if ( m_Points <= 1 )
        return;

    for ( int i = 0; i < m_Points + 1; i++ )
        m_StretchPoints.at(i) = double(i)/(m_Points);

    SetStatus();
}

string FeelMap::ToString(const char * prefix)
{
    char buff[100];
    string result;

    if ( prefix != NULL )
    {
        result += prefix;
        result += ' ';
    }

    result += "Feel Map";
    if ( m_StretchPoints.size() > 2 )
        for ( int i = 1; i < m_StretchPoints.size() - 1; i++ )
        {
            result += " ";
            sprintf(buff, "%.3f", m_StretchPoints.at(i));
            result += buff;
        }

    return result;
}

void FeelMap::FromString(string s)
{
    if ( s.find("Feel Map") == string::npos )
        throw string("FeelMap::FromString() - Not a valid field list.");

    vector<string> tokens = split(s.c_str(), ' ');

    New(tokens);
}

void FeelMap::SetStatus()
{
    char buff[200];

    m_Status = "Feel: ";
    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( ! m_Active )
    {
        m_Status += " Off";
        return;
    }

    if ( m_StretchPoints.size() < 2 )
    {
        m_Status += "No stretch points set.";
        return;
    }

    int pos = 0;

    for ( int i = 0; i < m_StretchPoints.size(); i++ )
    {
        pos = m_Status.size();
        sprintf(buff, " %.2f", m_StretchPoints.at(i));
        m_Status += buff;
        m_FieldPositions.emplace_back(pos + 1, m_Status.size() - pos - 1);
    }

    if ( m_Points > 1 )
        m_Highlights.push_back(m_FieldPositions.at(m_EditPoint));
}

bool FeelMap::HandleKey(key_type_t k)
{
    if ( ! m_Active || m_EditPoint == 0 || m_EditPoint == m_StretchPoints.size() - 1 )
        return true;

    int tEditPoint = m_EditPoint;
    double tStretchPoint = m_StretchPoints.at(m_EditPoint);

    switch ( k )
    {

    case left:
    case right:
        if ( k == left )
            tEditPoint -= 1;
        else
            tEditPoint += 1;
        if ( tEditPoint >= 1 && tEditPoint < m_StretchPoints.size() - 1 )
            m_EditPoint = tEditPoint;
        break;
    case up:
    case down:
        if ( k == up )
            tStretchPoint += 0.01;
        else
            tStretchPoint -= 0.01;
        if ( tStretchPoint > m_StretchPoints.at(m_EditPoint -1) && tStretchPoint < m_StretchPoints.at(m_EditPoint + 1) )
            m_StretchPoints.at(m_EditPoint) = tStretchPoint;
        break;

    default:
        return false;
    }

    SetStatus();

    return true;
}


double FeelMap::Adjust( double beat )
{
    if ( ! m_Active )
        return beat;

    if ( m_StretchPoints.empty() )
        return beat;

    int wholeBeats = lround(1000 * beat)/1000; // Round to 3 DP.

    double t = beat - wholeBeats;

    int milliBeats = lround(1000 * t); // For safe comparisons.

    if ( milliBeats == 0 )
        return beat;

    int n = lround(t * m_Points);

    double P = static_cast<double>(m_Points);
    double T;

    char buff[100];

    if ( n == 0 )
    {
        T = P * m_StretchPoints.at(1) * t;
#if LOG_ON
        sprintf(buff, "Beat %6.2f -> %6.2f (n = %i)\n", beat, T, n);
#endif
    }
    else
    {
        double windowIn = 1.0 / P;
        double windowOut = m_StretchPoints.at(n) - m_StretchPoints.at(n-1);
        double tFrac = t - static_cast<double>(n-1)/P;
        T = m_StretchPoints.at(n-1) + tFrac * windowOut / windowIn;
#if LOG_ON
        sprintf(buff, "t %6.2f -> %6.2f (n = %i, wIn = %.3f wOut = %.3f tFrac %.3f)\n", t, T, n, windowIn, windowOut, tFrac);
#endif
    }

    // T = m_StretchPoints.at(n-1) + (1.0 * m_Points * t - 1) * (m_StretchPoints.at(n) - m_StretchPoints.at(n-1));

    T += wholeBeats;

#if LOG_ON
    fLog << buff;
#endif

    return T;
}
