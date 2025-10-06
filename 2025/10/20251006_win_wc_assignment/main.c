// win_wc.c
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <wchar.h>
#include <stdbool.h>

#define BUF_SIZE (64 * 1024)

typedef struct {
    unsigned long long lines;
    unsigned long long words;
    unsigned long long bytes;
} Count;

static inline bool is_space_ascii(unsigned char c) {
    // ASCII 空白類：SP(0x20), TAB(0x09), CR(0x0D), LF(0x0A), VT(0x0B), FF(0x0C)
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
}

static void count_buffer(const unsigned char* buf, DWORD n, Count* acc, bool* in_word) {
    for (DWORD i = 0; i < n; ++i) {
        unsigned char c = buf[i];
        if (c == '\n') acc->lines++;
        if (is_space_ascii(c)) {
            *in_word = false;
        } else {
            if (!*in_word) {
                acc->words++;
                *in_word = true;
            }
        }
    }
}

static BOOL process_stdin(Count* out) {
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
    if (hin == INVALID_HANDLE_VALUE || hin == NULL) return FALSE;

    unsigned char* buf = (unsigned char*)HeapAlloc(GetProcessHeap(), 0, BUF_SIZE);
    if (!buf) return FALSE;

    BOOL ok = TRUE;
    bool in_word = false; // ★ 修正：BOOL → bool
    for (;;) {
        DWORD read = 0;
        if (!ReadFile(hin, buf, BUF_SIZE, &read, NULL)) { ok = FALSE; break; }
        if (read == 0) break;
        out->bytes += read;
        count_buffer(buf, read, out, &in_word);
    }
    HeapFree(GetProcessHeap(), 0, buf);
    return ok;
}

static BOOL process_file(LPCWSTR path, Count* out) {
    HANDLE h = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (h == INVALID_HANDLE_VALUE) return FALSE;

    LARGE_INTEGER sz;
    if (!GetFileSizeEx(h, &sz)) { CloseHandle(h); return FALSE; }
    out->bytes += (unsigned long long)sz.QuadPart;

    unsigned char* buf = (unsigned char*)HeapAlloc(GetProcessHeap(), 0, BUF_SIZE);
    if (!buf) { CloseHandle(h); return FALSE; }

    BOOL ok = TRUE;
    bool in_word = false; // ★ 修正：BOOL → bool
    for (;;) {
        DWORD read = 0;
        if (!ReadFile(h, buf, BUF_SIZE, &read, NULL)) { ok = FALSE; break; }
        if (read == 0) break;
        count_buffer(buf, read, out, &in_word);
    }
    HeapFree(GetProcessHeap(), 0, buf);
    CloseHandle(h);
    return ok;
}

static void print_result(Count c, LPCWSTR name) {
    // 数値は ASCII で出したいので、ワイド→UTF-8にして WriteFile
    wchar_t line[256];
    _snwprintf(line, 256, L"%llu %llu %llu %ls\r\n", c.lines, c.words, c.bytes, name);
    int bytes = WideCharToMultiByte(CP_UTF8, 0, line, -1, NULL, 0, NULL, NULL);
    char* u8 = (char*)HeapAlloc(GetProcessHeap(), 0, bytes);
    if (!u8) return;
    WideCharToMultiByte(CP_UTF8, 0, line, -1, u8, bytes, NULL, NULL);
    DWORD written = 0;
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), u8, (DWORD)(bytes - 1), &written, NULL);
    HeapFree(GetProcessHeap(), 0, u8);
}

int wmain(int argc, wchar_t** argv) {
    Count total = (Count){0,0,0};

    if (argc == 1) {
        Count c = (Count){0,0,0};
        if (!process_stdin(&c)) {
            DWORD w = 0;
            WriteFile(GetStdHandle(STD_ERROR_HANDLE), "stdin error\n", 12, &w, NULL);
            return 1;
        }
        print_result(c, L"-");
        return 0;
    }

    int files = 0;
    for (int i = 1; i < argc; ++i) {
        if (wcscmp(argv[i], L"-") == 0) {
            Count c = (Count){0,0,0};
            if (!process_stdin(&c)) {
                DWORD w = 0;
                WriteFile(GetStdHandle(STD_ERROR_HANDLE), "stdin error\n", 12, &w, NULL);
                continue;
            }
            print_result(c, L"-");
            total.lines += c.lines; total.words += c.words; total.bytes += c.bytes;
            files++;
        } else {
            Count c = (Count){0,0,0};
            if (!process_file(argv[i], &c)) {
                // 簡易エラー表示（UTF-8 固定）
                const char* msg = "open/read error: ";
                DWORD w = 0;
                WriteFile(GetStdHandle(STD_ERROR_HANDLE), msg, (DWORD)lstrlenA(msg), &w, NULL);
                int bytes = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, NULL, 0, NULL, NULL);
                char* u8 = (char*)HeapAlloc(GetProcessHeap(), 0, bytes);
                if (u8) {
                    WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, u8, bytes, NULL, NULL);
                    WriteFile(GetStdHandle(STD_ERROR_HANDLE), u8, (DWORD)(bytes - 1), &w, NULL);
                    HeapFree(GetProcessHeap(), 0, u8);
                }
                WriteFile(GetStdHandle(STD_ERROR_HANDLE), "\n", 1, &w, NULL);
                continue;
            }
            print_result(c, argv[i]);
            total.lines += c.lines; total.words += c.words; total.bytes += c.bytes;
            files++;
        }
    }

    if (files >= 2) {
        print_result(total, L"total");
    }
    return 0;
}
