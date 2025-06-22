#pragma once
/////////////////////////////
////2025/06/06 nanashima âπê∫ä«óù
////////////////////////////
#include <xaudio2.h>



class Sound
{
private:
	static IXAudio2* m_Xaudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	IXAudio2SourceVoice* m_SourceVoice{};
	BYTE* m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};


public:
	static void InitMaster();
	static void UninitMaster();

	void SetVolume(float volume);
	void Stop();
	void Uninit();

	void Load(const char* FileName);
	void Play(bool Loop = false);


};

