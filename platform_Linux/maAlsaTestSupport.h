#ifndef MAALSATESTSUPPORT_INCLUDED
#define MAALSATESTSUPPORT_INCLUDED

void init_alsa_test_support();
void alsa_midi_free_event(snd_seq_event_t *ev);
bool alsa_midi_read_input(snd_seq_event_t **ev);
bool alsa_midi_input_pending();
void alsa_midi_write_event(snd_seq_event_t *ev);
int alsa_midi_test_support_GetFileDescriptorCount();
bool alsa_midi_test_support_GetFileDescriptors(struct pollfd * pfd, int npfd);

#endif // MAALSATESTSUPPORT_INCLUDED
