/*==============================================================================

   ���_�Ǘ�[resultBG.h]
														 Author : �����ې�
                                                         Date   : 2025 06 06
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class ResultBG
{
	static struct RESULTBG {
		D3DXVECTOR2		pos;	//�ʒu�x�N�g��
		D3DXVECTOR2		size;	//�傫���x�N�g��
	};
	static int m_ResultBGTexNO;
	static RESULTBG m_ResultBG;
public:
	static void InitResultBG(void);
	static void UninitResultBG(void);
	static void UpdateResultBG(void);
	static void DrawResultBG(void);
};
