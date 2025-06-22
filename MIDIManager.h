#pragma once
#include"MidiFile.h"
#include <chrono>
class MIDIManager//MIDI�̓����͊Ǘ��p�N���X
{
   

    static int m_trackCount;//�g���b�N��
    static int m_noteCount;//�m�[�g��
    static int m_BPM;
    static bool m_isPlay;//�Đ����Ă��邩�t���O
    static bool m_isStop;//��~�t���O����ɂ���Ē�~����̍Đ������������邢�͏��߂Ă̍Đ��������
    static int  m_playType;

    static std::chrono::steady_clock::time_point m_midiStartTime;
    static std::vector<std::vector<int>> m_velocity;
    static std::vector<std::vector<int>> m_pitch;
    static std::vector<std::vector<double>> m_starttime;
    static std::vector<std::vector<double>>m_duration;
           

           
public:
     static void ReadMIDI(std::string filsname);
     static std::vector<std::vector<int>> GetVelocity();
     static std::vector<std::vector<int>> GetPitch();
     static int GettrackCount();
     static int GetnoteCount();
     
     static std::vector<std::vector<double>> GetStartTime();
     static std::vector<std::vector<double>> GetDuration();
     static int GetPlayType();
     static void SetPlayType(int type);
    
     static  bool GetIsPlay();//�Đ����Ă��邩�̎擾

    
     static void SetNotesSize();
   
     static void MakeNotes();  //�����Ńm�[�c�����
     static void StartActiveNotes(int type);//�Đ������@�����ɂ��Đ��p�^�[���؂�ւ�
   
     

     static void StartPlayback();//�Đ����Ăяo����OK
     static void StopPlayback();
     

     static void ResetPlayback();//������

};

