#ifndef FEELMAP_H
#define FEELMAP_H

#include <vector>

#include "maCursorKeys.h"

class FeelMap : public CursorKeys
{
    public:
        FeelMap();
        virtual ~FeelMap();

        double Feel(double beat);

        virtual void SetFocus()
        {
            CursorKeys::SetFocus();
            SetStatus();
        }

    protected:
        void SetStatus();

        virtual bool HandleKey(key_type_t k);
        int m_EditPoint;

    private:

        int m_Points;
        std::vector<double> m_StretchPoints;
};

#endif // FEELMAP_H
