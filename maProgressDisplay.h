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

#ifndef MAPROGRESSDISPLAY_H
#define MAPROGRESSDISPLAY_H

#include "maItemMenu.h"


class ProgressDisplay : public ItemMenu
{
    public:
        ProgressDisplay();
        ~ProgressDisplay();

        void Update();
        virtual void SetStatus();

    protected:

    private:

        double m_Progress = 0;
        double m_StepWidth = 4;
        double m_Phase = 0;
        double m_Beat = 0;
        unsigned int m_ScheduleTimeSeconds = 0;
        unsigned int m_ScheduleTimeNanoSeconds = 0;

};

extern ProgressDisplay g_ProgressBar;

#endif // MAPROGRESSDISPLAY_H
