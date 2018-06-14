#ifndef ALSA_TYPES_H_INCLUDED
#define ALSA_TYPES_H_INCLUDED

//
// When we don't have alsa, lets see if we can use their types ...
//
////////////////////////////////////////////////////////////////////////

#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
typedef unsigned char snd_seq_event_type_t;

typedef struct snd_seq_ev_raw8 {
	unsigned char d[12];		/**< 8 bit value */
} snd_seq_ev_raw8_t;

typedef struct snd_seq_ev_raw32 {
	unsigned int d[3];		/**< 32 bit value */
} snd_seq_ev_raw32_t;

typedef struct snd_seq_real_time {
	unsigned int tv_sec;	/* seconds */
	unsigned int tv_nsec;	/* nanoseconds */
} snd_seq_real_time_t;

typedef unsigned int snd_seq_tick_time_t;	/* midi ticks */

union snd_seq_timestamp {
	snd_seq_tick_time_t tick;
	struct snd_seq_real_time time;
};

struct snd_seq_addr {
	unsigned char client;	/**< Client number:         0..255, 255 = broadcast to all clients */
	unsigned char port;	/**< Port within client:    0..255, 255 = broadcast to all ports */
};

struct snd_seq_ev_note {
	unsigned char channel;
	unsigned char note;
	unsigned char velocity;
	unsigned char off_velocity;	/* only for SNDRV_SEQ_EVENT_NOTE */
	unsigned int duration;		/* only for SNDRV_SEQ_EVENT_NOTE */
};

typedef struct snd_seq_ev_ctrl {
	unsigned char channel;		/**< channel number */
	unsigned char unused[3];	/**< reserved */
	unsigned int param;		/**< control parameter */
	signed int value;		/**< control value */
} snd_seq_ev_ctrl_t;

typedef struct snd_seq_event {
	snd_seq_event_type_t type;	/* event type */
	unsigned char flags;		/* event flags */
	char tag;

	unsigned char queue;		/* schedule queue */
	union snd_seq_timestamp time;	/* schedule time */


	struct snd_seq_addr source;	/* source address */
	struct snd_seq_addr dest;	/* destination address */

	union {				/* event data... */
		struct snd_seq_ev_note note;
		struct snd_seq_ev_ctrl control;
		struct snd_seq_ev_raw8 raw8;
		struct snd_seq_ev_raw32 raw32;
//		struct snd_seq_ev_ext ext;
//		struct snd_seq_ev_queue_control queue;
//		union snd_seq_timestamp time;
//		struct snd_seq_addr addr;
//		struct snd_seq_connect connect;
//		struct snd_seq_result result;
//		struct snd_seq_ev_quote quote;
	} data;
} snd_seq_event_t;

#define SND_SEQ_EVENT_NOTEON    6
#define SND_SEQ_EVENT_NOTEOFF   7
#define SND_SEQ_EVENT_CONTROLLER 10
#define SND_SEQ_EVENT_ECHO     50

#endif
#endif // ALSA_TYPES_H_INCLUDED
