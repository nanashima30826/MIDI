/*==============================================================================

   ���C��[main.cpp]
                                                         Author : �����ې�
                                                         Date   : 2025 06 06
--------------------------------------------------------------------------------

==============================================================================*/
#include <windows.h>
#include "main.h"
#include <time.h>
#include "renderer.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include"GUI.h"
#include"fade.h"
#include"sound.h"
#include"MIDIManager.h"
#include"Manager.h"
#include <commctrl.h>  // TBM_GETPOS �Ȃ�
#pragma comment(lib, "comctl32.lib")  // �����N�p
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME			"GameWindow"				// �E�C���h�E�̃N���X��
#define WINDOW_CAPTION		"�E�G�r�W���A���C�U�[�@ENTER�Ŏ��̃V�[���ɐi��"			// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
#ifdef _DEBUG
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_CAPTION;	// �f�o�b�O�����\���p
float g_sizex = 100, g_sizey = 10;
#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	//�����_���V�[�h�̏�����
	srand((unsigned int)time(NULL));
	
	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	int window_width = 960;
	int window_height = 540;

	
	
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_CAPTION,
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		CW_USEDEFAULT,																		// �E�B���h�E�̍����W
		CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		window_width + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		window_height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);


	// DirectX�̏�����(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;
	
	//���j���[�\��
	HMENU hMenu = GUI::CreateMainMenu();
	SetMenu(hWnd, hMenu);
	// �E�C���h�E�̕\��(Init()�̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	
	// ���b�Z�[�W���[�v
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();					// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_CAPTION);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��
				
				dwFrameCount++;		// �����񐔂̃J�E���g�����Z
			}
		}
	}
	
	timeEndPeriod(1);				// ����\��߂�

	// �I������
	Uninit();

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hSizeX=nullptr, hSizeY=nullptr;
	
	
    
    switch (uMsg)
    {
    case WM_DESTROY:
		
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        break;
	case WM_COMMAND:

		switch (LOWORD(wParam))//�{�^�������ꂽ�Ƃ��̕���
		{
		case ID_SELECT_MIDI:
		{
			char filename[MAX_PATH] = { 0 };
			GUI::SelectMidiFile(filename, MAX_PATH);
			break;
		}
		case ID_SELECT_BACK_COLOR:
			GUI::ShowColorDialog(hWnd, ID_SELECT_BACK_COLOR);
			break;

		case ID_SELECT_NOTES_COLOR:
			GUI::ShowColorDialog(hWnd, ID_SELECT_NOTES_COLOR);
			break;

		case ID_SELECT_ACTIVE_NOTES_COLOR:
			GUI::ShowColorDialog(hWnd, ID_SELECT_ACTIVE_NOTES_COLOR);
			break;
		case ID_SELECT_PLAY:
			MIDIManager::StartPlayback();
			break;
		case ID_SELECT_RESET:
			MIDIManager::ResetPlayback();
			break;
		case ID_SELECT_STOP:
			MIDIManager::StopPlayback();
			break;
		case ID_SELECT_TYPE01:
			MIDIManager::SetPlayType(0);
			break;
		case ID_SELECT_TYPE02:
			MIDIManager::SetPlayType(1);
			break;
		case ID_SELECT_SIZEX:
			hSizeX=GUI::CreateSizeControl(hWnd,true);
			break;
		case ID_SELECT_SIZEY:
			hSizeY=GUI::CreateSizeControl(hWnd, false);
			break;
		default:
			break;
		}
		break;
	case WM_HSCROLL:
	{
		HWND hScroll = (HWND)lParam;
		int controlID = GetDlgCtrlID(hScroll);
		
		

		switch (controlID)
		{
		case ID_SELECT_SLIDERX:
			GUI::SetSetedSize((float)SendMessage(hSizeX, TBM_GETPOS, 0, 0), true);
			MIDIManager::SetNotesSize();
			break;
		case ID_SELECT_SLIDERY:
			GUI::SetSetedSize((float)SendMessage(hSizeY, TBM_GETPOS, 0, 0), false);
			MIDIManager::SetNotesSize();
			break;
		default:
			break;
		}

	}
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_�����O�����̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	// ���͏����̏�����
	InitInput(hInstance, hWnd);
	
	// �X�v���C�g�̏�����
	InitSprite();

	//�t�F�[�h������
	Fade::InitFadeBG();
	
	//�V�[���̏�����
	Manager::Init();
		
	

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	
	// �X�v���C�g�̏I������
	UninitSprite();

	// �e�N�X�`���̑S���
	UninitTexture();

	// ���͏����̏I������
	UninitInput();
	// �����_�����O�̏I������
	UninitRenderer();

	Manager::Uninit();
	
	//�t�F�[�h�I������
	Fade::UninitFadeBG();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͏����̍X�V����
	UpdateInput();
	
	Manager::Update();
	
	
	Fade::UpdateFadeBG();
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();//���W��2D�ϊ�

	// 2D�`��Ȃ̂Ő[�x����
	SetDepthEnable(false);

	
	
	
	
	Manager::Draw();
	
// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Fade::DrawFadeBG();

	Present();
}


float frand(void)
{
	return (float)rand() / RAND_MAX;
}

