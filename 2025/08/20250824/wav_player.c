#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

typedef struct {
    char riff[4];
    uint32_t overall_size;
    char wave[4];
    char fmt_chunk_marker[4];
    uint32_t length_of_fmt;
    uint16_t format_type;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byterate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char data_chunk_header[4];
    uint32_t data_size;
} WAVHeader;

typedef struct {
    HANDLE thread;
    wchar_t filename[MAX_PATH];
    char mb_filename[MAX_PATH]; // 追加
    int is_playing;
    int is_paused;
    int stop_flag;
    DWORD play_start_time;
    DWORD pause_time;
    DWORD total_paused;
    uint32_t data_size;
    uint32_t byterate;
} WAVPlayer;

static WAVPlayer g_player = {0};

DWORD WINAPI wav_play_thread(LPVOID arg) {
    PlaySoundW(g_player.filename, NULL, SND_FILENAME | SND_ASYNC);
    g_player.play_start_time = GetTickCount();
    while (!g_player.stop_flag) {
        Sleep(100);
    }
    PlaySoundW(NULL, 0, 0);
    return 0;
}

int wav_play_start(const char *filename) {
    if (g_player.is_playing) return -1;
    // マルチバイト→ワイド文字列変換
    MultiByteToWideChar(CP_ACP, 0, filename, -1, g_player.filename, MAX_PATH);
    strncpy(g_player.mb_filename, filename, MAX_PATH - 1); // 追加
    g_player.mb_filename[MAX_PATH - 1] = '\0';             // 念のため終端
    g_player.stop_flag = 0;
    g_player.is_paused = 0;
    g_player.total_paused = 0;
    g_player.thread = CreateThread(NULL, 0, wav_play_thread, NULL, 0, NULL);
    if (!g_player.thread) return -1;
    g_player.is_playing = 1;
    // WAVヘッダから情報取得
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;
    WAVHeader header;
    fread(&header, sizeof(WAVHeader), 1, fp);
    fclose(fp);
    g_player.data_size = header.data_size;
    g_player.byterate = header.byterate;
    return 0;
}

void wav_play_stop() {
    if (!g_player.is_playing) return;
    g_player.stop_flag = 1;
    WaitForSingleObject(g_player.thread, INFINITE);
    CloseHandle(g_player.thread);
    g_player.is_playing = 0;
}

int wav_get_length_sec() {
    // 1. byterate/data_sizeから計算（現状）
    if (g_player.byterate != 0)
        return (int)(g_player.data_size / g_player.byterate);

    // 2. ファイルから再度ヘッダを読む方法（冗長だが確実）
    FILE *fp = fopen(g_player.mb_filename, "rb"); // 修正
    if (!fp) return 0;
    WAVHeader header;
    if (fread(&header, sizeof(WAVHeader), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    if (header.byterate == 0) return 0;
    return (int)(header.data_size / header.byterate);
}

//実装サンプル
// int main(int argc, char *argv[]) {
//     if (argc < 2) {
//         printf("Usage: %s <wavfile>\n", argv[0]);
//         return 1;
//     }
//     // // 
//     // if (wav_play_start(argv[1]) != 0) {
//     //     printf("Failed to play WAV file.\n");
//     //     return 1;
//     // }

//     printf("Commands: PLAY, STOP, QUIT, LENGTH\n");
//     char cmd[32];
//     while (1) {
//         printf("> ");
//         if (!fgets(cmd, sizeof(cmd), stdin)) break;
//         if (_stricmp(cmd, "PLAY\n") == 0) {
//             if (!g_player.is_playing)
//                 wav_play_start(argv[1]);
//         } else if (_stricmp(cmd, "STOP\n") == 0) {
//             wav_play_stop();
//         } else if (_stricmp(cmd, "QUIT\n") == 0) {
//             wav_play_stop();
//             break;
//         } else if (_stricmp(cmd, "LENGTH\n") == 0) {
//             printf("Length: %d sec\n", wav_get_length_sec());
//         } else {
//             printf("Unknown command.\n");
//         }
//     }
//     return 0;
// }