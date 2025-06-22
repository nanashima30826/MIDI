/////////////////////
//2025 0606 MIDIのノーツの処理　七島維吹
///////////////////
#pragma once
#include"renderer.h"
#include<vector>
class MIDINotes//描画するノーツの管理
{
	D3DXVECTOR2		m_pos{};	//位置ベクトル

	D3DXVECTOR2		m_size{};	//大きさベクトル
	D3DXVECTOR2     m_uv=D3DXVECTOR2(0.0,0.0);
	D3DXVECTOR2     m_uvwh=D3DXVECTOR2(1.0,1.0);
	D3DXVECTOR2     m_texsize{};
	static int m_MIDINotesTexNO;
	bool m_use=false,m_active=false;
public:
	static void InitMIDINotes();
	static void UninitMIDINotes();
	static void UpdateMIDINotes();
	static void DrawMIDINotes();
	
	void SetSize(D3DXVECTOR2 size);
	void SetPos(D3DXVECTOR2 pos);
	D3DXVECTOR2 GetPos();
	D3DXVECTOR2 GetSize();
	MIDINotes(D3DXVECTOR2 pos,D3DXVECTOR2 size,bool use);
	static std::vector<MIDINotes*> GetMIDINotes(void);
	static void SetMIDINotes(D3DXVECTOR2 p, D3DXVECTOR2 s);//MIDInotesの追加処理
	static void SetActive(int number,bool isActive);//再生時の処理
};

