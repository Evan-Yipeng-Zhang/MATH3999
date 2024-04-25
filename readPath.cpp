#include "algorithm.h"
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

std::string readToolPath() {
    std::string basePath = getExecutablePath();
    std::ifstream configFile(basePath + "\\Blossom4Path");
    std::string path;
    if (getline(configFile, path)) {
        size_t pos = path.find("=");
        if (pos != std::string::npos) {
            return path.substr(pos + 2); // Skip '=' and space
        }
    }
    return "";
}

std::string getExecutablePath() {
    wchar_t wpath[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL) {
        GetModuleFileNameW(hModule, wpath, MAX_PATH);
        PathRemoveFileSpecW(wpath);
    }
    else {
        // Handle the error, possibly returning an empty or default path
        return ".";
    }
    // Convert wide string to standard string using UTF-8 encoding
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wpath);
}