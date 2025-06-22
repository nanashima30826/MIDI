#pragma once
#include"scene.h";
class Manager
{
private:
	static Scene* m_Scene;//メンバをスタティックにしたらcpp側でもう一度宣言する　現在シーン
	static Scene* m_NextScene;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static Scene* GetScene() { return m_Scene; }
	template<typename T>
	static void SetScene()
	{
		m_NextScene = new T;
	}


};
