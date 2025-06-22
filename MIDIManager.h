#pragma once
#include"MidiFile.h"
#include <chrono>
class MIDIManager//MIDIの入室力管理用クラス
{
   

    static int m_trackCount;//トラック数
    static int m_noteCount;//ノート数
    static int m_BPM;
    static bool m_isPlay;//再生しているかフラグ
    static bool m_isStop;//停止フラグこれによって停止からの再生か初期化あるいは初めての再生かを区別
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
    
     static  bool GetIsPlay();//再生しているかの取得

    
     static void SetNotesSize();
   
     static void MakeNotes();  //ここでノーツを作る
     static void StartActiveNotes(int type);//再生処理　引数により再生パターン切り替え
   
     

     static void StartPlayback();//再生時呼び出しでOK
     static void StopPlayback();
     

     static void ResetPlayback();//初期化

};

