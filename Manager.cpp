/////////////////////////////////
//2025 06 06 �V�[���̑S�̊Ǘ��}�l�[�W���[
/////////////////////////////////
#include "Manager.h"
#include"renderer.h"
#include"input.h"
#include"title.h"

Scene* Manager::m_Scene{};//�����o���X�^�e�B�b�N�ɂ�����cpp���ł�����x�錾����
Scene* Manager::m_NextScene{};
void Manager::Init()
{
	Sound::InitMaster();
	m_Scene = new Title();
	m_Scene->Init();
}


void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;
}

void Manager::Update()
{
	m_Scene->Update();
}

void Manager::Draw()
{
	m_Scene->Draw();
	if (m_NextScene != nullptr)
	{
		m_Scene->Uninit();
		delete m_Scene;

		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}

}