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

#ifndef TRANSLATETABLE_H
#define TRANSLATETABLE_H

#include <string>
#include <vector>

#include "maCursorKeys.h"

enum accidentals_mode_t {
    accmode_upper,
    accmode_lower,
    upper_mute_if_clash,
    lower_mute_if_clash,
    number_accidentals_modes
};

enum scale_premap_mode_t {
    premap_off,
    premap_mute,
    premap_leave,
    premap_upper,
    premap_lower,
    number_premap_modes
};

enum scale_t {
    major,
    harmonic_minor,
    major_dorian,
    major_phrygian,
    major_lydian,
    major_mixolydian,
    natural_minor,
    major_locrian,
    major_pentatonic,
    minor_pentatonic,
    major_blues,
    minor_blues,
    scroll_top,           // Values are allowed beyond this point, but browsing them
                          // isn't an option in case scales are empty or behaviour
                          // undefined.
    scale_unknown,
    from_list,
    number_scale_types
};

// #include "maPatternStore.h"
class StepList; // Passed as parameter to one of the SetScale() methods.

// This isn't used at the moment, but will come in
// handy if we do pre-calculations when setting up
// or changing scales and associated settings.

typedef std::vector<std::vector<int>> matrix_t;

class TranslateTable : public CursorKeys
{
    public:
        TranslateTable();
        virtual ~TranslateTable();


        void Reset();

        void FromString(std::string s);
        std::string ToString(const char * prefix = NULL);

        void LoadTableFromString(const char *table);
        void FindNextTranslation(int dir);
        int TranslateUsingTable(int root, int scale, int degree, int note);
        bool CanTranslate(int scale, int degree);

        int TranslateUsingNoteMap(int note);
        int PreMapScale(int note);
        int Translate(int note);
        // int Transpose(int n) { return n + m_Transpose; };

        int Transpose() { return m_Transpose; }
        void SetTranspose(int val) { m_Transpose = val; }
        void SetNewTransposePending( int val );
        bool NewTransposePending();

        void SetScale(scale_t scale);
        bool SetScale(std::string & name);
        bool SetScale(StepList & list);
        void NextScale( int dir );
        int Scale() { return m_Scale; }

        void SetRoot(int val) { m_Root = val; }
        bool SetRoot(std::string & root);
        int Root() { return m_Root; }

        void SetDegreeShift(int val) { m_DegreeShift = val; }
        int DegreeShift() { return m_DegreeShift; }

        void NextAccidentalsMode( int dir );
        // void SetPremap( bool val ) { m_PreMapScale = val; }
        void NextPremapMode( int dir );
        scale_premap_mode_t PremapMode() { return m_PremapMode; }

        const char * PremapModeName();
        std::string RootName();
        const char * ScaleName();
        const char * ShiftName();
        const char * AccidentalsModeName();
        std::string ShowScale();
        std::string ShowNoteMap();

        virtual void SetStatus();
        virtual void SetFocus()
        {
            CursorKeys::SetFocus();
            // SetStatus();
        }

    protected:
        enum translate_table_focus_t {
            chromatic_shift,
            tonal_shift,
            root,
            scale,
            premap,
            accidentals,
            number_tt_focus_modes
        };

        virtual bool HandleKey(key_type_t k);
        translate_table_focus_t m_TranslateTableFocus;


    private:

        void SetupScaleMaps();
        int ShiftSum(int start, int degreeShift);

        std::vector<matrix_t> m_Tables;

        // int m_NoteMap[12];          // Maps semitons to scale degrees
        std::vector<int> m_NoteMap;
        // int m_Intervals[12];        // Lists steps, in semitones, for each degree.
        std::vector<int> m_Intervals;

        int m_DegreeShiftSemis;
        int m_DegreeShift;
        int m_Root;
        int m_ScaleDegrees;
        int m_Transpose;

        scale_t m_Scale;
        scale_premap_mode_t m_PremapMode;
        accidentals_mode_t m_AccidentalsMode;

        bool m_NewTransposePending;
        int m_NewTranspose;

};

#endif // TRANSLATETABLE_H
