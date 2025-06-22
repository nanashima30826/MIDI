/*==============================================================================

   頂点管理 [fadeBG.cpp]
														 Author :七島維吹
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
// マクロ定義
//*****************************************************************************
#define FADESPEED 0.01f;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// 変数
//*****************************************************************************
int Fade::m_FadeBGTexNO;//テクスチャid
 float Fade::m_FadeAlpha;//フェードアルファ地
 MODE_FADE Fade::m_FadeState;//フェードの状態


//=============================================================================
// 初期化処理
//=============================================================================
void Fade::InitFadeBG(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャの読み込み
	m_FadeBGTexNO = LoadTexture((char*)"data/TEXTURE/fade_white.png");
	m_FadeAlpha = 0.0f;
	m_FadeState = MODE_FADE::FADE_NONE;
}

//=============================================================================
// 終了処理
//=============================================================================
void Fade::UninitFadeBG(void)
{
	
}

//=============================================================================
// 更新処理
//=============================================================================
void Fade::UpdateFadeBG(void)
{
	switch (m_FadeState)
	{
	case MODE_FADE::FADE_IN:
		m_FadeAlpha -= FADESPEED;
		if (m_FadeAlpha<=0.0f)
		{
			//フェードイン終了
			m_FadeState = MODE_FADE::FADE_NONE;
		}
		break;
	case MODE_FADE::FADE_OUT:
		m_FadeAlpha += FADESPEED;
		if (m_FadeAlpha >= 1.0f)
		{
			//フェードアウト終了
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
// 描画処理//タイトルBGの描画
//=============================================================================
void Fade::DrawFadeBG(void)
{
	//フェードの描画
DrawSpriteColor(m_FadeBGTexNO,
		SCREEN_WIDTH/2, SCREEN_HEIGHT/2,
		SCREEN_WIDTH,SCREEN_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f,1.0f,1.0f,1.0f,m_FadeAlpha);
}
///////////////////////////////////////////////////////
//フェードセット
///////////////////////////////////////////////////////
void Fade::SetFade(MODE_FADE state)
{
	//フェードステートセット
	m_FadeState = state;

	//アルファ地の初期化
	if (state == MODE_FADE::FADE_IN)
	{
		m_FadeAlpha = 1.0f;
	}
	else if (state == MODE_FADE::FADE_OUT)
	{
		m_FadeAlpha = 0.0f;
	}

}
	
		

