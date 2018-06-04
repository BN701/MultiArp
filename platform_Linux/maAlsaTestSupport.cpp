/* midiroute.c by Matthias Nagorni */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

#define MAX_MIDI_PORTS   4

int open_seq(snd_seq_t **seq_handle, int in_ports[], int out_ports[], int num_in, int num_out);
void midi_route(snd_seq_t *seq_handle, int out_ports[], int split_point);

/* Open ALSA sequencer wit num_in writeable ports and num_out readable ports. */
/* The sequencer handle and the port IDs are returned.
                     */
int open_seq(snd_seq_t **seq_handle, int in_ports[], int out_ports[], int num_in, int num_out)
{

    int l1;
    char portname[64];

    if (snd_seq_open(seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0)
    {
        fprintf(stderr, "Error opening ALSA sequencer.\n");
        return(-1);
    }

    snd_seq_set_client_name(*seq_handle, "MultiArp Blue PC");
    for (l1 = 0; l1 < num_in; l1++)
    {
        sprintf(portname, "MultiArp Blue PC IN %d", l1);
        if ((in_ports[l1] = snd_seq_create_simple_port(*seq_handle, portname,
                            SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                            SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
        {
            fprintf(stderr, "Error creating sequencer port.\n");
            return(-1);
        }
    }

    for (l1 = 0; l1 < num_out; l1++)
    {
        sprintf(portname, "MultiArp Blue PC OUT %d", l1);
        if ((out_ports[l1] = snd_seq_create_simple_port(*seq_handle, portname,
                             SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
                             SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
        {
            fprintf(stderr, "Error creating sequencer port.\n");
            return(-1);
        }
    }

    snd_config_update_free_global();

    return(0);
}

snd_seq_t *seq_handle;
int in_ports[MAX_MIDI_PORTS], out_ports[MAX_MIDI_PORTS];


/* Read events from writeable port and route them to readable port 0  */
/* if NOTEON / OFF event with note < split_point. NOTEON / OFF events */
/* with note >= split_point are routed to readable port 1. All other  */
/* events are routed to both readable ports.                          */

void midi_route(snd_seq_t *seq_handle, int out_ports[], int split_point)
{

    snd_seq_event_t *ev;

    do
    {
        snd_seq_event_input(seq_handle, &ev);

        snd_seq_ev_set_subs(ev);
        snd_seq_ev_set_direct(ev);

        if ((ev->type == SND_SEQ_EVENT_NOTEON)||(ev->type == SND_SEQ_EVENT_NOTEOFF))
        {
            if (ev->data.note.note < split_point)
            {
                snd_seq_ev_set_source(ev, out_ports[0]);
            }
            else
            {
                snd_seq_ev_set_source(ev, out_ports[1]);
            }
            snd_seq_event_output_direct(seq_handle, ev);
        }
        else
        {
            snd_seq_ev_set_source(ev, out_ports[0]);
            snd_seq_event_output_direct(seq_handle, ev);
            snd_seq_ev_set_source(ev, out_ports[1]);
            snd_seq_event_output_direct(seq_handle, ev);
        }
        snd_seq_free_event(ev);
    }
    while (snd_seq_event_input_pending(seq_handle, 0) > 0);
}

void alsa_midi_free_event(snd_seq_event_t *ev)
{
    snd_seq_free_event(ev);
}


bool alsa_midi_read_input(snd_seq_event_t **ev)
{
    return snd_seq_event_input(seq_handle, ev) >= 0;
}

bool alsa_midi_input_pending()
{
    return snd_seq_event_input_pending(seq_handle, 0) > 0;
}

bool alsa_midi_write_event(snd_seq_event_t *ev)
{
    int r;
//    ev->time.time.tv_sec = 0;
//    ev->time.time.tv_nsec = 0;
    r = snd_seq_ev_set_subs(ev);
    r = snd_seq_ev_set_direct(ev);
    r = snd_seq_ev_set_source(ev, out_ports[0]);
    int r2 = snd_seq_event_output_direct(seq_handle, ev);
    const char *strError = snd_strerror(r2);
    return r2 >= 0;
}

bool init_alsa_test_support()
{
    return (open_seq(&seq_handle, in_ports, out_ports, 1, 1) >= 0);
}

int alsa_midi_test_support_GetFileDescriptorCount()
{
    return snd_seq_poll_descriptors_count(seq_handle, POLLIN);
}

bool alsa_midi_test_support_GetFileDescriptors(struct pollfd * pfd, int npfd)
{
    snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);
    return true; // TODO: How to check for success. (Or should we just be throwing exceptions?)
}

//int main(int argc, char *argv[])
//{
//    int npfd;
//    struct pollfd *pfd;
//
//
//
//    npfd = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
//    pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
//    snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);
//    while (1)
//    {
//        if (poll(pfd, npfd, 100000) > 0)
//        {
//            midi_route(seq_handle, out_ports, split_point);
//        }
//    }
//
//}
