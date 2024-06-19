/*
 * Docs used to create this file: https://learn.microsoft.com/en-us/previous-versions/dd743680(v=vs.85)
 */
#include "SoundHandler.h"

#include <Windows.h>
#include <Mmsystem.h>

#pragma comment (lib, "winmm.lib")

 // From: https://stackoverflow.com/a/62762272/23283336
std::wstring SoundHandler::convertToLCPWSTR(const std::string& s)
{
    const int slength = static_cast<int>(s.length()) + 1;
    const int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

void SoundHandler::playSoundSnippet(const std::string& fileName)
{
    PlaySound(convertToLCPWSTR(fileName).c_str(), 0, SND_FILENAME | SND_ASYNC);
}

void SoundHandler::forceStopSound()
{
    PlaySound(NULL, 0, 0);
}
