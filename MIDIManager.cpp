////////////////////////////////
//2025 06 06 nanashima MIDI�̑S�̊Ǘ�
////////////////////////////////
#include"MidiFile.h"
#include<iostream>
#include"MIDIManager.h"
#include"GUI.h"
#include"MIDINotes.h"
#include<Windows.h>


int MIDIManager::m_BPM = 0;
int MIDIManager::m_trackCount = 0;
int MIDIManager::m_noteCount = 0;
int MIDIManager::m_playType = 0;
bool MIDIManager::m_isPlay = false;
bool MIDIManager::m_isStop = false;
std::chrono::steady_clock::time_point MIDIManager::m_midiStartTime;
std::vector<std::vector<int>> MIDIManager::m_velocity;//�z���1�����ڂ̓g���b�N�ԍ�
std::vector<std::vector<int>> MIDIManager::m_pitch;
std::vector<std::vector<double>> MIDIManager::m_starttime;
std::vector<std::vector<double>>MIDIManager::m_duration;
void MIDIManager::ReadMIDI(std::string filename) {
    smf::MidiFile midi;

    if (!midi.read(filename)) {
        std::cerr << "MIDI�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B\n";
        return;
    }

    midi.doTimeAnalysis();      // ���ԉ�́i�b�P�ʁj
    midi.linkNotePairs();       // NoteOn/NoteOff�y�A�����O

    // �ϐ�������
    m_trackCount = midi.getTrackCount();
    m_noteCount = 0;
    m_BPM = 0;

    // �e�f�[�^�z����g���b�N���ɍ��킹�ă��T�C�Y
    m_pitch.resize(m_trackCount);
    m_velocity.resize(m_trackCount);
    m_starttime.resize(m_trackCount);
    m_duration.resize(m_trackCount);

    // �f�o�b�O�o��
    char debugBuf[256];
    sprintf_s(debugBuf, "Track��: %d\n", m_trackCount);
    OutputDebugStringA(debugBuf);

    // �e���|�擾�i�ŏ��ɏo�Ă���e���|�C�x���g���̗p�j
    for (int track = 0; track < m_trackCount; ++track) {
        for (int i = 0; i < midi[track].size(); ++i) {
            const auto& event = midi[track][i];
            if (event.isTempo()) {
                m_BPM = static_cast<int>(event.getTempoBPM());
                std::cout << "�e���|: " << m_BPM << " BPM\n";
                break; // �ŏ��̃e���|�̂ݎ擾
            }
        }
        if (m_BPM > 0) break;
    }

    // Tick resolution�i�f�o�b�O�p�j
    int tpqn = midi.getTicksPerQuarterNote();
    sprintf_s(debugBuf, "TPQN: %d\n", tpqn);
    OutputDebugStringA(debugBuf);

    // �e�g���b�N�̃C�x���g����
    for (int track = 0; track < m_trackCount; ++track) {
        // �g���b�N���ƂɃf�[�^���N���A
        m_pitch[track].clear();
        m_velocity[track].clear();
        m_starttime[track].clear();
        m_duration[track].clear();

        for (int i = 0; i < midi[track].size(); ++i) {
            const auto& event = midi[track][i];

            if (event.isNoteOn()) {
                m_pitch[track].push_back(event.getKeyNumber());
                m_velocity[track].push_back(event.getVelocity());
                m_starttime[track].push_back(event.seconds);
                m_duration[track].push_back(event.getDurationInSeconds());
                m_noteCount++;
            }
        }
    }

    // �ŏI�f�o�b�O�o��
    sprintf_s(debugBuf, "���m�[�g��: %d\n", m_noteCount);
    OutputDebugStringA(debugBuf);
}


 std::vector<std::vector<int>> MIDIManager::GetVelocity()
{
    return m_velocity;
}

 int MIDIManager::GettrackCount()
 {
     return m_trackCount;
 }

 int MIDIManager::GetnoteCount()
 {
     return m_noteCount;
 }

 

std::vector<std::vector<int>> MIDIManager::GetPitch()
{
    return m_pitch;
}
std::vector<std::vector<double>> MIDIManager::GetStartTime()
{
    return  m_starttime;
}
std::vector<std::vector<double>> MIDIManager::GetDuration()
{
    return  m_duration;
}
int MIDIManager::GetPlayType()
{
    return m_playType;
}
void MIDIManager::SetPlayType(int type)
{
    m_playType = type;
}

bool MIDIManager::GetIsPlay()
{
    return m_isPlay;
}
void MIDIManager::MakeNotes()
{
    D3DXVECTOR2 size = GUI::GetSetedSize();
    
    for (int track = 1; track < m_trackCount; ++track) {//track��0�Ԗڂ͉���񂪂Ȃ��@BPM�Ƃ��̏�񂪓����Ă���̂Ŋ܂߂Ȃ�
        for (int i = 0; i < m_noteCount; i++)
        {
            MIDINotes::SetMIDINotes(D3DXVECTOR2((m_starttime[track][i] * size.x + size.x), (-m_pitch[track][i] * size.y) + size.y * size.y*size.y), D3DXVECTOR2(m_duration[track][i] * size.x, size.y));
        }
    }

}
void MIDIManager::SetNotesSize()
{
    D3DXVECTOR2 size = GUI::GetSetedSize();

    for (int track = 1; track < m_trackCount; ++track) {//track��0�Ԗڂ͉���񂪂Ȃ��@BPM�Ƃ��̏�񂪓����Ă���̂Ŋ܂߂Ȃ�
        int i = 0;
        for (auto Midi:MIDINotes::GetMIDINotes())
        {
            Midi->SetSize(D3DXVECTOR2(m_duration[track][i] * size.x, size.y));
            Midi->SetPos(D3DXVECTOR2((m_starttime[track][i] * size.x + size.x), (-m_pitch[track][i] * size.y) + size.y *100));
            i++;
        }
    }

}
void MIDIManager::StartActiveNotes(int type) {
    
    double currentTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - m_midiStartTime).count();

   

    for (int track = 0; track < m_trackCount; ++track) {
        for (size_t i = 0; i < m_starttime[track].size(); ++i) {
            double noteStart = m_starttime[track][i];
            double noteEnd=m_starttime[track][i] + m_duration[track][i];
            switch (type)
            {
            case 0:
                if (currentTime>=noteStart&&currentTime<=noteEnd) {
                    MIDINotes::SetActive(i + 1, true);
                }
                break;
            case 1:
                if (currentTime >= noteStart && currentTime <= noteEnd) {
                MIDINotes::SetActive(i+1,true);
                MIDINotes::SetActive(i, false);
                }
                else if ( currentTime >noteEnd ) {
                MIDINotes::SetActive(i + 1, false);
                }
                break;
            default:
                break;
            }
        }
    }
}

void MIDIManager::StartPlayback() {
    if (m_isStop==false)
    {
        m_midiStartTime = std::chrono::steady_clock::now();//�Đ��J�n�p
    }
    else
    {
        m_isStop = false;
    }
    m_isPlay = true;
}
void MIDIManager::StopPlayback() {
    m_midiStartTime = std::chrono::steady_clock::now();//��~�p
    m_isStop = true;
    m_isPlay = false;
}
void MIDIManager::ResetPlayback()
{
    m_isStop == false;
    m_isPlay = false;
    for (int track = 0; track < m_trackCount; ++track) {
        for (size_t i = 0; i < m_starttime[track].size(); ++i) {

                MIDINotes::SetActive(i + 1,false); //�S�����Ƃɖ߂� 
        }
    }
}