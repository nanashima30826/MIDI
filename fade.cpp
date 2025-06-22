/*==============================================================================

   ���_�Ǘ� [fadeBG.cpp]
														 Author :�����ې�
														 Date   :2025 06 06
--------------------------------------------------------------------------------

==============================================================================*/
#include "fade.h"
#include "input.h"
#include "sprite.h"
#include "texture.h"
#include"Manager.h"
#include"title.h"
#include"game.h"
#include"result.h"



//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADESPEED 0.01f;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �ϐ�
//*****************************************************************************
int Fade::m_FadeBGTexNO;//�e�N�X�`��id
 float Fade::m_FadeAlpha;//�t�F�[�h�A���t�@�n
 MODE_FADE Fade::m_FadeState;//�t�F�[�h�̏��


//=============================================================================
// ����������
//=============================================================================
void Fade::InitFadeBG(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	m_FadeBGTexNO = LoadTexture((char*)"data/TEXTURE/fade_white.png");
	m_FadeAlpha = 0.0f;
	m_FadeState = MODE_FADE::FADE_NONE;
}

//=============================================================================
// �I������
//=============================================================================
void Fade::UninitFadeBG(void)
{
	
}

//=============================================================================
// �X�V����
//=============================================================================
void Fade::UpdateFadeBG(void)
{
	switch (m_FadeState)
	{
	case MODE_FADE::FADE_IN:
		m_FadeAlpha -= FADESPEED;
		if (m_FadeAlpha<=0.0f)
		{
			//�t�F�[�h�C���I��
			m_FadeState = MODE_FADE::FADE_NONE;
		}
		break;
	case MODE_FADE::FADE_OUT:
		m_FadeAlpha += FADESPEED;
		if (m_FadeAlpha >= 1.0f)
		{
			//�t�F�[�h�A�E�g�I��
			m_FadeState = MODE_FADE::FADE_IN;
			Scene* nowScene = Manager::GetScene();
			if (Title* title = dynamic_cast<Title*>(nowScene))
			{
				Manager::SetScene<Game>();
			}
			if (Game* game = dynamic_cast<Game*>(nowScene))
			{
				Manager::SetScene<Result>();
			}
			if (Result* result = dynamic_cast<Result*>(nowScene))
			{
				Manager::SetScene<Title>();
			}

			
		}
		break;
	case MODE_FADE::FADE_NONE:
		m_FadeAlpha = 0.0f;
		break;
	default:
		break;
	}
}


//=============================================================================
// �`�揈��//�^�C�g��BG�̕`��
//=============================================================================
void Fade::DrawFadeBG(void)
{
	//�t�F�[�h�̕`��
DrawSpriteColor(m_FadeBGTexNO,
		SCREEN_WIDTH/2, SCREEN_HEIGHT/2,
		SCREEN_WIDTH,SCREEN_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f,1.0f,1.0f,1.0f,m_FadeAlpha);
}
///////////////////////////////////////////////////////
//�t�F�[�h�Z�b�g
///////////////////////////////////////////////////////
void Fade::SetFade(MODE_FADE state)
{
	//�t�F�[�h�X�e�[�g�Z�b�g
	m_FadeState = state;

	//�A���t�@�n�̏�����
	if (state == MODE_FADE::FADE_IN)
	{
		m_FadeAlpha = 1.0f;
	}
	else if (state == MODE_FADE::FADE_OUT)
	{
		m_FadeAlpha = 0.0f;
	}

}
	
		

