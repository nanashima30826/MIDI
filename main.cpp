/*==============================================================================

   メイン[main.cpp]
                                                         Author : 七島維吹
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
#include <commctrl.h>  // TBM_GETPOS など
#pragma comment(lib, "comctl32.lib")  // リンク用
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			"GameWindow"				// ウインドウのクラス名
#define WINDOW_CAPTION		"界隈ビジュアライザー　ENTERで次のシーンに進む"			// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


//*****************************************************************************
// グローバル変数:
//*****************************************************************************
#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_CAPTION;	// デバッグ文字表示用
float g_sizex = 100, g_sizey = 10;
#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	//ランダムシードの初期化
	srand((unsigned int)time(NULL));
	
	// 時間計測用
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
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	int window_width = 960;
	int window_height = 540;

	
	
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_CAPTION,
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		window_width + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		window_height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);


	// DirectXの初期化(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	
	//メニュー表示
	HMENU hMenu = GUI::CreateMainMenu();
	SetMenu(hWnd, hMenu);
	// ウインドウの表示(Init()の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
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
			dwCurrentTime = timeGetTime();					// システム時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= ((float)1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_CAPTION);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
				SetWindowText(hWnd, g_DebugStr);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理
				
				dwFrameCount++;		// 処理回数のカウントを加算
			}
		}
	}
	
	timeEndPeriod(1);				// 分解能を戻す

	// 終了処理
	Uninit();

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
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

		switch (LOWORD(wParam))//ボタンおされたときの分岐
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
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	// 入力処理の初期化
	InitInput(hInstance, hWnd);
	
	// スプライトの初期化
	InitSprite();

	//フェード初期化
	Fade::InitFadeBG();
	
	//シーンの初期化
	Manager::Init();
		
	

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	
	// スプライトの終了処理
	UninitSprite();

	// テクスチャの全解放
	UninitTexture();

	// 入力処理の終了処理
	UninitInput();
	// レンダリングの終了処理
	UninitRenderer();

	Manager::Uninit();
	
	//フェード終了処理
	Fade::UninitFadeBG();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力処理の更新処理
	UpdateInput();
	
	Manager::Update();
	
	
	Fade::UpdateFadeBG();
	
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
	Clear();

	// マトリクス設定
	SetWorldViewProjection2D();//座標の2D変換

	// 2D描画なので深度無効
	SetDepthEnable(false);

	
	
	
	
	Manager::Draw();
	
// バックバッファ、フロントバッファ入れ替え
	Fade::DrawFadeBG();

	Present();
}


float frand(void)
{
	return (float)rand() / RAND_MAX;
}

