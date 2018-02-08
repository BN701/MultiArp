#include "maBaseUI.h"

#include <cmath>

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
