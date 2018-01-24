#ifndef BASEUI_H
#define BASEUI_H

struct InOutPair;


class BaseUI
{
    public:

        enum text_attribute_t
        {
            attr_normal = 0,
            attr_bold = 1,
            attr_underline = 2
        };

        enum window_area_t
        {
            whole_screen,          // Position relative to whole screen.
            big_panel,             // Botton half of screen, used for all sorts.
            small_panel,           // Pattern Status, or pattern chain. Under progress bar, to the left.
            progress_panel,        // Extra progress panel, to the right of Pattern Status
            edit_list_panel,       // Pattern List, under command line.
            edit_summary_panel,    // Pattern Summary, under command line to the right of Pattern List
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

        virtual void Text(window_area_t area, int row, int col, const char * text, text_attribute_t attribute) = 0;

        virtual void Progress(double progress, double width, double beat, int pattern_progress) = 0;

        void SetBigPanelPage( big_panel_page_t val ) { m_BigPanelPage = val; }
        void NextBigPanelPage( int direction );
        big_panel_page_t BigPanelPage() { return m_BigPanelPage; }
        void ToggleBigPanelHold() { m_BigPanelHold = ! m_BigPanelHold; }
        bool BigPanelHold() { return m_BigPanelHold; }

    protected:

    private:
        big_panel_page_t m_BigPanelPage = one;
        bool m_BigPanelHold = false;

};

#endif // BASEUI_H
