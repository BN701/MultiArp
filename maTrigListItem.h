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


#ifndef TRIGLISTITEM_H
#define TRIGLISTITEM_H

#include <string>

#include "maCursorKeys.h"

struct ArpeggioStage
{
    int m_Position = 0;
    int m_Interval = 1;
    int m_Steps = 0;

    ArpeggioStage()
    {}

    ArpeggioStage(int interval, int steps):
        m_Interval(interval),
        m_Steps(steps)
    {}

    void Increment(bool shift)
    {
        if ( shift )
            m_Steps += 1;
        else
            m_Interval += 1;
    }

    void Decrement(bool shift)
    {
        if ( shift )
        {
            if ( m_Steps > 1 )
                m_Steps -= 1;
        }
        else
            m_Interval -= 1;
    }


    bool Step(int & interval, bool stepNext)
    {
        interval += m_Interval * m_Position;
        if ( stepNext )
        {
            m_Position += 1;
            if ( m_Position == m_Steps )
            {
                m_Position = 0;
                return true;
            }
        }
        return false;
    }

    void FromString(std::string s);
};


class TrigRepeater : public CursorKeys
{
    public:
        enum decay_mode_t
        {
            off,
            exponential,
            linear,
            exponential_unlimited,
            linear_unlimited,
            num_decay_modes
        };

        void SetRepeats( int val ) { m_Repeats = val; }
        int Repeats() { return m_Repeats; }
        void SetRepeatTime( double val ) { m_RepeatTime = val; }
        double RepeatTime() { return m_RepeatTime; }
        void SetVelocityDecay( double val) {m_VelocityDecay = val; }
        double VelocityDecay() { return m_VelocityDecay; }
        void SetDecayMode( decay_mode_t val ) { m_DecayMode = val; }
        decay_mode_t DecayMode() { return m_DecayMode; }

        void NextVelocityDecayMode( int dir );

        std::vector<ArpeggioStage> & Arpeggio() { return m_Arpeggio; }

        void Init(double tempo, double stepLengthMilliSecs);
        void Reset(int noteVelocity);
        bool Step(int64_t & queue_delta, int & interval, unsigned char & velocity);

        void ArpeggioFromString(std::string s);

        void AddArpStage(int interval, int steps)
        {
            m_Arpeggio.push_back(ArpeggioStage(interval, steps));
        }

    private:
        // Persistent variables

        int m_Steps = 0;
        int m_Repeats = 0;
        double m_RepeatTime = 0.0; // Zero means split evenly across step.
        double m_VelocityDecay = 0.0;
        decay_mode_t m_DecayMode = off;

        std::vector<ArpeggioStage> m_Arpeggio;

        // Working variables

        int64_t m_RepeatTimeMicroSecs = 0;
        double m_NoteVelocity = 0;
        double m_VelocityDecrement = 0;

};

class TrigListItem : public CursorKeys
{
    public:
        TrigListItem();
        virtual ~TrigListItem();

        std::vector<int> & Trigs() { return m_Trigs; }
        double Multiplier() { return m_Multiplier; }
        bool Skip() { return m_Skip; }
        bool Mute() { return m_Mute; }

        TrigRepeater & Repeater() { return m_Repeater; }

        std::string ToString();
        void FromString(std::string s);
        void FromSimpleString(std::string s);

        std::string TrigMaskToString();

        void TrigsFromMask(unsigned long trigMask);
        unsigned long MaskFromTrigs();

        std::string MenuString(int width = 0);

    virtual void SetStatus();
    protected:
        virtual bool HandleKey(key_type_t k);
        enum trig_list_item_menu_focus_t
        {
            tlif_trigs,
            tlif_multiplier,
            tlif_skip,
            tlif_mute,
            tlif_repeats,
            tlif_repeat_time,
            tlif_decay_mode,
            tlif_velocity_decay,
            num_tlif_types
        };

        trig_list_item_menu_focus_t m_TrigListItemFocus = tlif_trigs;

    private:
        std::vector<int> m_Trigs = {0}; // Default for individual item is to trigger the first list.
        unsigned long m_TrigMask = 1;    // Cheat for scrolling values in menu. (Default matches m_Trigs default.)
        double m_Multiplier = 1.0;
        bool m_Skip = false;
        bool m_Mute = false;

        TrigRepeater m_Repeater;
};

#endif // TRIGLISTITEM_H
