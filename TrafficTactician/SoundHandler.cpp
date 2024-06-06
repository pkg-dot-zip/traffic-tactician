/*
 * Docs used to create this file: https://learn.microsoft.com/en-us/previous-versions/dd743680(v=vs.85)
 */


#include "SoundHandler.h"

#include <Windows.h>
#include <Mmsystem.h>

#pragma comment (lib, "winmm.lib")
void playWindowsSound()
{
	PlaySound((LPCTSTR)SND_ALIAS_SYSTEMSTART, NULL, SND_ALIAS_ID);
}

void playSoundSnippet(std::string_view fileName)
{
	// PlaySound(TEXT("filename.wav"), NULL, SND_FILENAME | SND_ASYNC);

	PlaySound(TEXT("test.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void forceStopSound()
{
	PlaySound(NULL, 0, 0);
}