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
