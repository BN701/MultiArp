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

#ifndef MAALSASEQUENCERQUEUE_H
#define MAALSASEQUENCERQUEUE_H

#include <alsa/asoundlib.h>


class AlsaSequencerQueue
{
    public:
        /** Default constructor */
        AlsaSequencerQueue();
        /** Default destructor */
        virtual ~AlsaSequencerQueue();

        /** Access m_queueId
         * \return The current value of m_queueId
         */

        int GetQueueId() { return mQueueId; }

        int Create(snd_seq_t * h, int pool_size);

        bool Start();

    protected:
        snd_seq_t *mSeqHandle;
        int mQueueId; //!< Member variable "qeueId"

    private:
};

#endif // MAALSASEQUENCERQUEUE_H
