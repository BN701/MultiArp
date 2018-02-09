#include "maBaseUI.h"

#include <cmath>
#include <unordered_map>

void Rectangle::ScaleD2I(double scale)
{
    m_iX = floor(m_dX * scale);
    m_iY = floor(m_dY * scale);
    m_iWidth = ceil(m_dWidth * scale);
    m_iHeight = ceil(m_dHeight * scale);
}

BaseUI::BaseUI()
{
    //ctor

//    m_SmallPanel = newwin(4, 56, 2, 4);
//    m_ProgressPanel = newwin(2, 15, 3, 61);
//    m_EditListPanel = newwin(4, 20, 8, 4);
//    m_EditSummaryPanel = newwin(4, 52, 8, 24);
//    m_BigPanel = newwin(11, 80, 12, 0);
//    m_BigPanelExtra = newwin(4, 72, 8, 4);

}

BaseUI::~BaseUI()
{
    //dtor
}

WindowRect & BaseUI::AreaToWindowRect(window_area_t area)
{
    switch (area)
    {
        case big_panel:
            return m_BigPanel;
        case small_panel:
            return m_SmallPanel;
        case progress_panel:
            return m_ProgressPanel;
        case edit_list_panel:
            return m_EditListPanel;
        case edit_summary_panel:
            return m_EditSummaryPanel;
        case big_panel_extra:
            return m_BigPanelExtra;
        default:
            return m_WholeScreen;
    }
}

void BaseUI::MapToFullScreen(window_area_t area, int & row, int & col)
{
    AreaToWindowRect(area).MapToFullScreen(col, row);

//    switch (area)
//    {
//        case whole_screen:
//            break;
//        case big_panel:
//            m_BigPanel.MapToFullScreen(col, row);
//            break;
//        case small_panel:
//            m_SmallPanel.MapToFullScreen(col, row);
//            break;
//        case progress_panel:
//            m_ProgressPanel.MapToFullScreen(col, row);
//            break;
//        case edit_list_panel:
//            m_EditListPanel.MapToFullScreen(col, row);
//            break;
//        case edit_summary_panel:
//            m_EditSummaryPanel.MapToFullScreen(col, row);
//            break;
//        case big_panel_extra:
//            m_BigPanelExtra.MapToFullScreen(col, row);
//            break;
//    }

}

std::unordered_map<int, const char *> g_KeyNames =
{
    {BaseUI::key_none, "key_none"},
    {BaseUI::key_ctrl_a, "key_ctrl_a"},
    {BaseUI::key_ctrl_c, "key_ctrl_c"},
    {BaseUI::key_ctrl_v, "key_ctrl_v"},
    {BaseUI::key_backspace, "key_backspace"},
    {BaseUI::key_space, "key_space"},
    {BaseUI::key_tab, "key_tab"},
    {BaseUI::key_shift_tab, "key_shift_tab"},
    {BaseUI::key_linefeed, "key_linefeed"},
    {BaseUI::key_return, "key_return"},
    {BaseUI::key_escape, "key_escape"},
    {BaseUI::key_menu_control, "key_menu_control"},

    {BaseUI::key_up, "key_up"},
    {BaseUI::key_down, "key_down"},
    {BaseUI::key_right, "key_right"},
    {BaseUI::key_left, "key_left"},
    {BaseUI::key_home, "key_home"},
    {BaseUI::key_end, "key_end"},

    {BaseUI::key_shift_up, "key_shift_up"},
    {BaseUI::key_shift_down, "key_shift_down"},
    {BaseUI::key_shift_right, "key_shift_right"},
    {BaseUI::key_shift_left, "key_shift_left"},

    {BaseUI::key_alt_up, "key_alt_up"},
    {BaseUI::key_alt_down, "key_alt_down"},
    {BaseUI::key_alt_right, "key_alt_right"},
    {BaseUI::key_alt_left, "key_alt_left"},

    {BaseUI::key_alt_shift_up, "key_alt_shift_up"},
    {BaseUI::key_alt_shift_down, "key_alt_shift_down"},
    {BaseUI::key_alt_shift_right, "key_alt_shift_right"},
    {BaseUI::key_alt_shift_left, "key_alt_shift_left"},

    {BaseUI::key_ctrl_up, "key_ctrl_up"},
    {BaseUI::key_ctrl_down, "key_ctrl_down"},
    {BaseUI::key_ctrl_right, "key_ctrl_right"},
    {BaseUI::key_ctrl_left, "key_ctrl_left"},

    {BaseUI::key_ctrl_shift_up, "key_ctrl_shift_up"},        // Not received with Ubuntu/xfce4-terminal
    {BaseUI::key_ctrl_shift_down, "key_ctrl_shift_down"},      // Not received with Ubuntu/xfce4-terminal
    {BaseUI::key_ctrl_shift_right, "key_ctrl_shift_right"},
    {BaseUI::key_ctrl_shift_left, "key_ctrl_shift_left"},

    {BaseUI::key_insert, "key_insert"},
    {BaseUI::key_delete, "key_delete"},
    {BaseUI::key_page_up, "key_page_up"},
    {BaseUI::key_page_down, "key_page_down"},
    {BaseUI::key_shift_delete, "key_shift_delete"},
    {BaseUI::key_ctrl_delete, "key_ctrl_delete"},

    {BaseUI::key_ctrl_page_up, "key_ctrl_page_up"},
    {BaseUI::key_ctrl_page_down, "key_ctrl_page_down"},
    {BaseUI::key_alt_page_up, "key_alt_page_up"},
    {BaseUI::key_alt_page_down, "key_alt_page_down"}
};

const char * BaseUI::KeyName(key_command_t key)
{
    if ( g_KeyNames.count(key) != 0 )
        return g_KeyNames[key];
    else
        return "unknown key";
}

void BaseUI::NextBigPanelPage( int direction )
{
    if ( direction > 0 )
    {
        m_BigPanelPage = static_cast<big_panel_page_t>(m_BigPanelPage + 1);
        if ( m_BigPanelPage == num_big_panel_pages )
            m_BigPanelPage = one;
    }
    else
    {
        m_BigPanelPage = static_cast<big_panel_page_t>(m_BigPanelPage - 1);
        if ( m_BigPanelPage < one )
            m_BigPanelPage = static_cast<big_panel_page_t>(num_big_panel_pages - 1);
    }
}
