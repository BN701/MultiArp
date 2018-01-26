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
}

BaseUI::~BaseUI()
{
    //dtor
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
