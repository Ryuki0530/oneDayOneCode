#include <windows.h>
#include <shellapi.h>
#include <string>
#include <vector>
#include <iostream>

void PrintUsage();
void PrintError(const wchar_t* msg);
bool IsConsoleHandle(HANDLE h);

int wmain()
{
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv) {
        PrintError(L"コマンドライン引数の取得に失敗しました。");
        return 2;
    }

    // 標準出力・標準エラーのハンドル取得
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);

    auto WriteLine = [&](const std::wstring& s, HANDLE h, bool isError = false) {
        DWORD written = 0;
        bool isConsole = IsConsoleHandle(h);
        std::wstring out = s + L"\r\n";
        if (isConsole) {
            WriteConsoleW(h, out.c_str(), (DWORD)out.size(), &written, nullptr);
        } else {
            // UTF-16LEバイト列で出力
            WriteFile(h, out.c_str(), (DWORD)(out.size() * sizeof(wchar_t)), &written, nullptr);
        }
    };

    if (argc == 1) {
        PrintUsage();
        LocalFree(argv);
        return 2;
    }

    std::wstring cmd = argv[1];
    if (cmd == L"--echo") {
        if (argc < 3) {
            PrintError(L"--echo の後に文字列を指定してください。");
            LocalFree(argv);
            return 2;
        }
        std::wstring msg = argv[2];
        WriteLine(msg, hStdOut);
        LocalFree(argv);
        return 0;
    } else if (cmd == L"--repeat") {
        if (argc < 4) {
            PrintError(L"--repeat の後に回数と文字列を指定してください。");
            LocalFree(argv);
            return 2;
        }
        int count = 0;
        try {
            count = std::stoi(argv[2]);
        } catch (...) {
            PrintError(L"回数は整数で指定してください。");
            LocalFree(argv);
            return 2;
        }
        if (count < 1 || count > 10000) {
            PrintError(L"回数は1～10000の範囲で指定してください。");
            LocalFree(argv);
            return 2;
        }
        std::wstring msg = argv[3];
        for (int i = 0; i < count; ++i) {
            WriteLine(msg, hStdOut);
        }
        LocalFree(argv);
        return 0;
    } else {
        PrintError(L"不正な引数です。--echo または --repeat を指定してください。");
        LocalFree(argv);
        return 2;
    }
}

void PrintUsage() {
    const wchar_t* usage =
        L"od1c_echo - コマンドライン出力ツール\n"
        L"使い方:\n"
        L"  od1c_echo.exe --echo 文字列\n"
        L"  od1c_echo.exe --repeat 回数 文字列\n";
    DWORD written = 0;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    bool isConsole = IsConsoleHandle(hStdOut);
    std::wstring out = usage;
    if (isConsole) {
        WriteConsoleW(hStdOut, out.c_str(), (DWORD)out.size(), &written, nullptr);
    } else {
        WriteFile(hStdOut, out.c_str(), (DWORD)(out.size() * sizeof(wchar_t)), &written, nullptr);
    }
}

void PrintError(const wchar_t* msg) {
    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    bool isConsole = IsConsoleHandle(hStdErr);
    std::wstring out = std::wstring(L"エラー: ") + msg + L"\r\n";
    DWORD written = 0;
    if (isConsole) {
        WriteConsoleW(hStdErr, out.c_str(), (DWORD)out.size(), &written, nullptr);
    } else {
        WriteFile(hStdErr, out.c_str(), (DWORD)(out.size() * sizeof(wchar_t)), &written, nullptr);
    }
}

// 標準出力/エラーがコンソールか判定
bool IsConsoleHandle(HANDLE h) {
    DWORD type = GetFileType(h);
    if (type == FILE_TYPE_CHAR) {
        DWORD mode;
        return GetConsoleMode(h, &mode) != 0;
    }
    return false;
}