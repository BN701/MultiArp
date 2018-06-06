#ifndef BASEUI_H
#define BASEUI_H

//#include "maListBuilder.h"

#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
//#include <Arduino.h>
#include <cstdint>
void EepromStore(uint8_t value);

#define SP_SEQUENCER        1
#define SP_LISTBUILDER      2
#define SP_UI               3
#define SP_STATE            4
#define SP_PATTERNSTORE     5
#define SP_CURSORKEYS       0
#define SP_TRANSLATETABLE   10
#define SP_SETUPSCALEMAPS   20
#define SP_UTILS_SPLIT      30
#endif

#include <map>

struct InOutPair;

class Rectangle
{
    public:
        int m_iX = 0;
        int m_iY = 0;
        int m_iWidth = 0;
        int m_iHeight = 0;

        double m_dX = 0;
        double m_dY = 0;
        double m_dWidth = 0;
        double m_dHeight = 0;

    Rectangle(int x, int y, int width, int height):
        m_iX(x),
        m_iY(y),
        m_iWidth(width),
        m_iHeight(height)
    {}

    Rectangle(double x, double y, double width, double height):
        m_dX(x),
        m_dY(y),
        m_dWidth(width),
        m_dHeight(height)
    {}

    void ScaleD2I(double scale = 1.0);
};

class MenuListDisplayInfo : public Rectangle
{
    public:
        int m_ScrollStart = 0;
        int m_ScrollHeight;
        int m_PreviousListSize = 0;

        MenuListDisplayInfo(int x, int y, int width, int height):
            Rectangle(x, y, width, height),
            m_ScrollHeight(height)
        {}
};

typedef std::map<int, MenuListDisplayInfo> dot_position_table_t;

class WindowRect : public Rectangle
{
    public:
        WindowRect(int height, int width, int row, int col):    // Field order here is from NCurses.
            Rectangle(col, row, width, height)                  // Note the order reversal: row/col -> col/row!
        {}

        void MapToFullScreen(int & x, int & y)
        {
            x += m_iX;
            y += m_iY;
        }

        int m_Background = 0;
        int m_Foreground = 0;
};

class BaseUI
{
    public:
        enum key_command_t
        {
            key_none = 256,   // Allow pass through for ASCII characters.,
            key_ctrl_a,
            key_ctrl_c,
            key_ctrl_v,
            key_backspace,
            key_space,          // Can be used command key as well as simple ASCII 32
            key_tab,
            key_shift_tab,
            key_linefeed,
            key_return,
            key_escape,
            key_menu_control,   // Boundary marker, not actually used as a key.
            key_down,
            key_up,
            key_left,
            key_right,
            key_home,
            key_end,
            key_shift_left,
            key_shift_right,
            key_delete,
            key_insert,
            key_shift_delete,
            key_ctrl_delete,
            key_ctrl_left,
            key_ctrl_right,
            key_ctrl_up,
            key_ctrl_down,
            key_ctrl_shift_left,
            key_ctrl_shift_right,
            key_ctrl_shift_up,      // Not available with ANSI (Ubuntu/xfce4-terminal)
            key_ctrl_shift_down,    // Not available with ANSI (Ubuntu/xfce4-terminal)
            key_alt_right,
            key_alt_left,
            key_alt_up,
            key_alt_down,
            key_alt_shift_right,
            key_alt_shift_left,
            key_alt_shift_up,
            key_alt_shift_down,
            key_shift_up,
            key_shift_down,
            key_page_up,
            key_page_down,
            key_shift_page_up,      // Not available with ANSI (Ubuntu/xfce4-terminal)
            key_shift_page_down,    // Not available with ANSI (Ubuntu/xfce4-terminal)
            key_ctrl_page_up,
            key_ctrl_page_down,
            key_alt_page_up,
            key_alt_page_down,
            key_cmd_base,
            key_cmd_enter,
            key_cmd_back,
            key_cmd_up,
            key_cmd_down,
            key_cmd_left,
            key_cmd_right,
            key_cmd_insert_left,
            key_cmd_insert_right,
            key_cmd_copy_left,
            key_cmd_copy_right,
            key_cmd_move_left,
            key_cmd_move_right,
            key_cmd_move_up,
            key_cmd_move_down,
            key_cmd_shift_left,
            key_cmd_shift_right,
            key_cmd_delete,
            key_cmd_undo,
            key_cmd_inc,
            key_cmd_inc_2,
            key_cmd_dec,
            key_cmd_dec_2
        };

        enum display_object_type_t  // Hopefully just use these for sanity checks.
        {
            dot_base,
            dot_pattern_store,
            dot_pattern,
            dot_pattern_menu_list,
            dot_step_list_group,
            dot_rt_list_group,
            dot_step_list,
            dot_rt_list,
            dot_cluster,
            dot_note
        };

        enum text_attribute_t
        {
            attr_normal = 0,
            attr_bold = 1,
            attr_underline = 2,
            attr_reverse = 4,
            attr_blink = 8
        };

        enum window_area_t
        {
            whole_screen,          // Position relative to whole screen.
            big_panel,             // Botton half of screen, used for all sorts.
            small_panel,           // Pattern Status, or pattern chain. Under progress bar, to the left.
            progress_panel,        // Extra progress panel, to the right of Pattern Status
            pattern_list_panel,       // Pattern List, under command line.
            pattern_list_summary_panel,    // Pattern Summary, under command line to the right of Pattern List
            big_panel_extra        // Overwrites pattern list and summary when in Big Panel is in Page 3 mode.
       };

        enum big_panel_page_t
        {
            one,
            two,
            three,
            num_big_panel_pages
        };


        BaseUI();
        virtual ~BaseUI();

//        virtual void Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute, double scale) = 0;
        virtual void MoveCursorToHome() {}
        virtual void SetTopLine(int midiChannel, double stepValue, double tempo, double quantum, int runState, int recState, int midiMode) = 0;
        virtual void Progress(double progress, double stepWidth, double beat,
        int pattern_progress, double rtBeat, unsigned int queueSecs, unsigned int queueNano) = 0;

        void SetBigPanelPage( big_panel_page_t val ) { m_BigPanelPage = val; }
        void NextBigPanelPage( int direction );
        big_panel_page_t BigPanelPage() { return m_BigPanelPage; }
        void ToggleBigPanelHold() { m_BigPanelHold = ! m_BigPanelHold; }
        bool BigPanelHold() { return m_BigPanelHold; }

        void MapToFullScreen(window_area_t area, int & row, int & col);
        WindowRect & AreaToWindowRect(window_area_t area);

        int BaseRow(window_area_t area);

        static const char * KeyName(key_command_t key);

        virtual bool GetDisplayInfo(int dot, MenuListDisplayInfo * & displayInfo) = 0;

    protected:

        int m_HomeCol = 4;
        int m_HomeRow = 6;

        WindowRect m_WholeScreen = WindowRect(25, 80, 0, 0);
        WindowRect m_SmallPanel = WindowRect(4, 56, 2, 4);
        WindowRect m_ProgressPanel = WindowRect(2, 15, 3, 61);
        WindowRect m_PatternListPanel = WindowRect(4, 20, 8, 4);
        WindowRect m_PatternListSummaryPanel = WindowRect(4, 52, 8, 24);
        WindowRect m_BigPanel = WindowRect(11, 80, 12, 0);
        WindowRect m_BigPanelExtra = WindowRect(4, 72, 8, 4);

    private:
        big_panel_page_t m_BigPanelPage = one;
        bool m_BigPanelHold = false;

};

#endif // BASEUI_H
