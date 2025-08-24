#ifndef WAV_PLAYER_H
#define WAV_PLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

int  wav_play_start(const char *filename);
void wav_play_stop(void);
int  wav_get_length_sec(void);

#ifdef __cplusplus
}
#endif

#endif // WAV_PLAYER_H