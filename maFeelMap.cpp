#include <cmath>
#include <iostream>
#include <fstream>


#include "maFeelMap.h"

using namespace std;

ofstream fLog;

FeelMap::FeelMap():
    m_EditPoint(1)
{
    //ctor
#if 0
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

#if 1
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
#if 0
    fLog.close();
#endif
}


void FeelMap::SetStatus()
{
    char buff[200];

    m_Status.clear();
    m_FieldPositions.clear();
    m_Highlights.clear();

    if ( m_StretchPoints.size() < 2 )
    {
        m_Status = "No stretch points set.";
        return;
    }

    m_Status = "Stretchpoints: ";
    int pos = 0;

    for ( int i = 0; i < m_StretchPoints.size(); i++ )
    {
        pos = m_Status.size();
        sprintf(buff, " %.2f", m_StretchPoints.at(i));
        m_Status += buff;
        m_FieldPositions.emplace_back(pos + 1, m_Status.size() - pos - 1);
    }

    m_Highlights.push_back(m_FieldPositions.at(m_EditPoint));
}

bool FeelMap::HandleKey(key_type_t k)
{
    if ( m_EditPoint == 0 || m_EditPoint == m_StretchPoints.size() - 1 )
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
    }

    SetStatus();

    return true;
}


double FeelMap::Feel( double beat )
{
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
        sprintf(buff, "Beat %6.2f -> %6.2f (n = %i)\n", beat, T, n);
    }
    else
    {
        double windowIn = 1.0 / P;
        double windowOut = m_StretchPoints.at(n) - m_StretchPoints.at(n-1);
        double tFrac = t - static_cast<double>(n-1)/P;
        T = m_StretchPoints.at(n-1) + tFrac * windowOut / windowIn;
        sprintf(buff, "t %6.2f -> %6.2f (n = %i, wIn = %.3f wOut = %.3f tFrac %.3f)\n", t, T, n, windowIn, windowOut, tFrac);
    }

    // T = m_StretchPoints.at(n-1) + (1.0 * m_Points * t - 1) * (m_StretchPoints.at(n) - m_StretchPoints.at(n-1));

    T += wholeBeats;

    fLog << buff;

    return T;
}
