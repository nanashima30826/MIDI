#include "MIDINotes.h"
/*==============================================================================

   �m�[�c�Ǘ� [MIDINotes.cpp]
														 Author :�����ې�
														 Date   :2025 06 02
--------------------------------------------------------------------------------

==============================================================================*/
#include "input.h"
#include "sprite.h"
#include "texture.h"
#include "GUI.h"
#include"sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
//�ϐ�
//*****************************************************************************
std::vector<MIDINotes*>NotesList;
 int MIDINotes::m_MIDINotesTexNO = 0;
 

//=============================================================================
// ����������
//=============================================================================
void MIDINotes::InitMIDINotes(void)
{
	
   //�e�N�X�`���̓ǂݍ���
	m_MIDINotesTexNO = LoadTexture((char*)"data/TEXTURE/notes.png");
	
}

//=============================================================================
// �I������
//=============================================================================
void  MIDINotes::UninitMIDINotes(void)
{
	for (MIDINotes* note : NotesList) {
		delete note;
	}
	NotesList.clear();
}

//=============================================================================
// �X�V����
//=============================================================================
void  MIDINotes::UpdateMIDINotes(void)
{
	
	for (auto Notes:NotesList)
	{
		if (GetKeyboardPress(DIK_UP)|| GetKeyboardPress(DIK_W))
		{
			Notes->m_pos.y -= 1;
		}
		if (GetKeyboardPress(DIK_DOWN) || GetKeyboardPress(DIK_S))
		{
			Notes->m_pos.y += 1;
		}
		if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_A))
		{
			Notes->m_pos.x -= 1;
		}
		if (GetKeyboardPress(DIK_RIGHT) || GetKeyboardPress(DIK_D))
		{
			Notes->m_pos.x += 1;
		}
		
		
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  MIDINotes::DrawMIDINotes(void)
{
	for (auto Midi:NotesList)
	{
		if (Midi->m_use)
		{
			if (Midi->m_active)
			{
				DrawSpriteColor(m_MIDINotesTexNO,
					Midi->m_pos.x, Midi->m_pos.y,
					Midi->m_size.x, Midi->m_size.y,
					Midi->m_uv.x, Midi->m_uv.y,
					Midi->m_uvwh.x, Midi->m_uvwh.y, GUI::GetActiveColor()[0], GUI::GetActiveColor()[1], GUI::GetActiveColor()[2], GUI::GetActiveColor()[3]);
			}
			else {
				DrawSpriteColor(m_MIDINotesTexNO,
					Midi->m_pos.x, Midi->m_pos.y,
					Midi->m_size.x, Midi->m_size.y,
					Midi->m_uv.x, Midi->m_uv.y,
					Midi->m_uvwh.x, Midi->m_uvwh.y, GUI::GetNotesColor()[0], GUI::GetNotesColor()[1], GUI::GetNotesColor()[2], GUI::GetNotesColor()[3]);
			}
		}
	}
}
void MIDINotes::SetSize(D3DXVECTOR2 size)
{
	m_size = size;
}
void MIDINotes::SetPos(D3DXVECTOR2 pos)
{
	m_pos = pos;
}
D3DXVECTOR2 MIDINotes::GetPos()
{
	return m_pos;
}
D3DXVECTOR2 MIDINotes::GetSize()
{
	return m_size;
}

MIDINotes::MIDINotes(D3DXVECTOR2 pos, D3DXVECTOR2 size,bool use)
{

	m_pos = pos;
	m_size = size;
	m_use = use;
}


 std::vector<MIDINotes*> MIDINotes::GetMIDINotes()
{
	return NotesList;
}
void MIDINotes::SetMIDINotes(D3DXVECTOR2 p, D3DXVECTOR2 s)
{
	
	NotesList.push_back(new MIDINotes(p, s, true));
	
}
void MIDINotes::SetActive(int number,bool isActive)//�g���b�N�ɂ��čl����ׂ�
{
	int count = 0;
	for (MIDINotes* Midi : NotesList)
	{
		if(count>=number)
		{
			break;
		}
		Midi->m_active = isActive;
		count += 1;
	}
}

