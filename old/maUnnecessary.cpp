/*
    Items in here came from the original miniArp.c example
    by Matthias Nagorni. I'm keeping it in case I need it
    for reference.

    The file will compile, but I haven't bothered with
    a header file as nothing else makes use of it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <signal.h>
#include <alsa/asoundlib.h>

#define TICKS_PER_QUARTER 128
#define MAX_SEQ_LEN        64


int bpm0, bpm, tempo, swing, sequence[3][MAX_SEQ_LEN], seq_len;

char seq_filename[1024];
snd_seq_tick_time_t tick;

snd_seq_t *seq_handle;
int queue_id, port_in_id, port_out_id;
int gTranslate;

void set_tempo()
{

    /*
     * The content of snd_seq_queue_tempo_t is hidden from us, hence we have to
     * use library functions to allocate it and set member variables. This is a
     * pain for debugging as we can't watch what's going on.
     */

    snd_seq_queue_tempo_t *queue_tempo;
    snd_seq_queue_tempo_malloc(&queue_tempo);

    /*
     * This calculation reduces to:
     *
     * tempo = 60,000,000 / (bpm * 128) * 128 (Units are microseconds per tick.)
     *
     */

    tempo = (int)(6e7 / ((double)bpm * (double)TICKS_PER_QUARTER) * (double)TICKS_PER_QUARTER);

    // Set some values on the tempo struct

    snd_seq_queue_tempo_set_tempo(queue_tempo, tempo);
    snd_seq_queue_tempo_set_ppq(queue_tempo, TICKS_PER_QUARTER);

    // Apply the tempo struct to the queue and actually set the tempo.

    snd_seq_set_queue_tempo(seq_handle, queue_id, queue_tempo);

    // We're done messing with the tempo, so de-allocate the struct that we used.

    snd_seq_queue_tempo_free(queue_tempo);
}

snd_seq_tick_time_t get_tick()
{

    snd_seq_queue_status_t *status;
    snd_seq_tick_time_t current_tick;
    const snd_seq_real_time_t *current_time;

    snd_seq_queue_status_malloc(&status);
    snd_seq_get_queue_status(seq_handle, queue_id, status);
    current_tick = snd_seq_queue_status_get_tick_time(status);
    current_time = snd_seq_queue_status_get_real_time(status);
    snd_seq_queue_status_free(status);

    int64_t queue_micros = (int64_t) current_time->tv_sec * 1000000 + (int64_t) current_time->tv_nsec / 1000;

    printf("Tick: %i, Queue: %9" PRId64 "\n", current_tick, queue_micros);

    return(current_tick);
}

void clear_queue()
{

    snd_seq_remove_events_t *remove_ev;

    snd_seq_remove_events_malloc(&remove_ev);
    snd_seq_remove_events_set_queue(remove_ev, queue_id);
    snd_seq_remove_events_set_condition(remove_ev, SND_SEQ_REMOVE_OUTPUT | SND_SEQ_REMOVE_IGNORE_OFF);
    snd_seq_remove_events(seq_handle, remove_ev);
    snd_seq_remove_events_free(remove_ev);
}

void arpeggio()
{

    // Event struct

    printf("arpeggio() called ...\n");
    get_tick();

    snd_seq_event_t ev;
    int l1;
    double dt; // Swing adjustment.

    for (l1 = 0; l1 < seq_len; l1++)
    {
        dt = (l1 % 2 == 0) ? (double)swing / 16384.0 : -(double)swing / 16384.0;

        // Macro: fills event record with NULLS
        snd_seq_ev_clear(&ev);

        // Macro: fills in event record with note data, ch, key, vel, dur. Also sets event type to Note
        snd_seq_ev_set_note(&ev, 0, sequence[2][l1] + gTranslate, 127, sequence[1][l1]);

        // Macro: schedules the event in MIDI tick mode
        snd_seq_ev_schedule_tick(&ev, queue_id,  0, tick);

        // Macro: sets the source port id number on the event record
        snd_seq_ev_set_source(&ev, port_out_id);

        // Macro: sets the destination as the subscribers (and sets dest port to UNKNOWN)
        snd_seq_ev_set_subs(&ev);

        // Macro: send the event to the queue, maybe? There's no description for this call in
        // the header file, and the implementation is hidden.
        snd_seq_event_output_direct(seq_handle, &ev);

        // Increment tick counter. (Timestamp.)
        tick += (int)((double)sequence[0][l1] * (1.0 + dt));
    }

    // Schedule an event to be fired back to our own app which prompts another
    // arpeggio to be placed in the queue.

    snd_seq_ev_clear(&ev);
    ev.type = SND_SEQ_EVENT_ECHO;
    snd_seq_ev_schedule_tick(&ev, queue_id,  0, tick);
    snd_seq_ev_set_dest(&ev, snd_seq_client_id(seq_handle), port_in_id);
    snd_seq_event_output_direct(seq_handle, &ev);
}

void parse_sequence()
{

    FILE *f;
    char c;

    if (!(f = fopen(seq_filename, "r")))
    {
        fprintf(stderr, "Couldn't open sequence file %s\n", seq_filename);
        exit(1);
    }
    seq_len = 0;
    while((c = fgetc(f))!=EOF)
    {
        switch (c)
        {
        case 'c':
            sequence[2][seq_len] = 0;
            break;
        case 'd':
            sequence[2][seq_len] = 2;
            break;
        case 'e':
            sequence[2][seq_len] = 4;
            break;
        case 'f':
            sequence[2][seq_len] = 5;
            break;
        case 'g':
            sequence[2][seq_len] = 7;
            break;
        case 'a':
            sequence[2][seq_len] = 9;
            break;
        case 'h':
            sequence[2][seq_len] = 11;
            break;
        default:
            goto done;
        }
        c =  fgetc(f);
        if (c == '#')
        {
            sequence[2][seq_len]++;
            c =  fgetc(f);
        }
        sequence[2][seq_len] += 12 * atoi(&c);
        c =  fgetc(f);
        sequence[1][seq_len] = TICKS_PER_QUARTER / atoi(&c);
        c =  fgetc(f);
        sequence[0][seq_len] = TICKS_PER_QUARTER / atoi(&c);
        seq_len++;
    }
done:
    fclose(f);
}

