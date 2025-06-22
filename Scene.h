////////////////////////
////2025 06 06 全てシーンの親クラス　nanashima
//////////////////////
#pragma once
class Scene
{
public:
	virtual  void Init()=0;
	virtual  void Uninit()=0;
	virtual  void Update()=0;
	virtual  void Draw()=0;
};

