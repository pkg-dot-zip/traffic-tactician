#pragma once
#include "easylogging++.h"

class SoundHandler
{
public:
	static SoundHandler& getInstance() {
		static SoundHandler instance;
		return instance;
	}

	static void playSoundSnippet(const std::string& fileName);
	static void forceStopSound();
private:
	SoundHandler() = default;
	~SoundHandler() = default;

	SoundHandler(const SoundHandler&) = delete;
	SoundHandler& operator=(const SoundHandler&) = delete;


	static std::wstring convertToLCPWSTR(const std::string& s);
};
