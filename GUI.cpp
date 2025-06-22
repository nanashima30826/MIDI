/////////////////////////////
////2025/06/06 nanashima GUI管理
////////////////////////////
#include "GUI.h"
#include <commctrl.h>  // スピン・トラックバーなど共通コントロール
#pragma comment(lib, "comctl32.lib")
#include <tchar.h>
#include <filesystem>

#define MAXSLIDER 1000
#define MINSLIDER 1
#define SLIDERSIZEX 200
#define SLIDERSIZEY 100
#define SLIDERPOSXX 100
#define SLIDERPOSYX 500
#define SLIDERPOSY 100


D3DXVECTOR2 GUI::m_size=D3DXVECTOR2(100.0,10.0);
std::string GUI::m_fileName = "";
std::string GUI::m_defaultPath = "none";
bool GUI::m_ischange = false;


std::array<float, COLOR_NUMBER> GUI::m_notesColor = { 1.0, 1.0, 1.0, 1.0 };
std::array<float, COLOR_NUMBER> GUI::m_activeColor = { 1.0, 0.0, 0.0, 1.0 };
std::array<float, COLOR_NUMBER> GUI::m_backColor = { 0.0, 0.0, 1.0, 1.0 };
bool GUI::SelectMidiFile(char* outFileName, DWORD outFileNameSize)
{
    if (m_defaultPath == "none")
    {
        char curDir[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, curDir);
        m_defaultPath = curDir;
    }
    m_ischange = true;
    OPENFILENAME ofn = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = outFileName;
    ofn.nMaxFile = outFileNameSize;
    ofn.lpstrFilter = "MIDI Files\0*.mid\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
   

    if (GetOpenFileName(&ofn) == TRUE)
    {
        std::filesystem::path fullPath(outFileName);
        m_fileName = std::filesystem::absolute(fullPath).string();  // ← 絶対パスとして保持
        SetCurrentDirectory(m_defaultPath.c_str());
        return true;
    }
    SetCurrentDirectory(m_defaultPath.c_str());
    return false;
}





std::string GUI::GetFileName()
{
    return m_fileName;
}
void GUI::SetIsChange(bool change)
{
    m_ischange = change;
}
bool GUI::GetIsChange()
{
    return m_ischange;
}
D3DXVECTOR2 GUI::GetSetedSize()
{
    return m_size;
}
void GUI::SetSetedSize(float size, bool XorY)
{
    if (XorY)
    {
        m_size.x = size;
    }
    else
    {
        m_size.y = size;
    }
}
std::array<float, COLOR_NUMBER> GUI::GetNotesColor()
{
    return m_notesColor;
}
std::array<float, COLOR_NUMBER> GUI::GetBackColor()
{
    return m_backColor;
}
std::array<float, COLOR_NUMBER> GUI::GetActiveColor()
{
    return m_activeColor;
}
void GUI::ShowColorDialog(HWND hwndOwner,int code) {
    CHOOSECOLOR cc = { 0 };
    static COLORREF customColors[16] = { 0 };
    COLORREF chosenColor = RGB(255, 255, 255); // 初期色（白）

    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = hwndOwner;
    cc.lpCustColors = customColors;
    cc.rgbResult = chosenColor;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    std::array<float, COLOR_NUMBER> outColor;
    if (ChooseColor(&cc)) {
        // ユーザーが色を選んだ
        chosenColor = cc.rgbResult;

        float r = GetRValue(chosenColor) / 255.0f;//255で割ることでDirectXの0.0~1.0での色表現に
        float g = GetGValue(chosenColor) / 255.0f;
        float b = GetBValue(chosenColor) / 255.0f;
        float a = 1.0f; // 必要に応じてαを設定
       
        outColor = { r,g,b,a };

        switch (code)
        {
        case ID_SELECT_BACK_COLOR:
            m_backColor = outColor;
            break;
        case ID_SELECT_NOTES_COLOR:
            m_notesColor = outColor;
            break;
        case ID_SELECT_ACTIVE_NOTES_COLOR:
            m_activeColor = outColor;
            break;
        default:
            break;
        }
    }
    
}
HWND GUI::CreateSizeControl(HWND hWnd,bool XorY)//Xなら真
{
   
        HINSTANCE hInst = GetModuleHandle(NULL);

        

        if (XorY)
        {
            HWND hSliderX = CreateWindowEx(
                0,
                TRACKBAR_CLASS,
                NULL,
                WS_OVERLAPPEDWINDOW | WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
                SLIDERPOSXX, SLIDERPOSY, SLIDERSIZEX, SLIDERSIZEY,//位置と大きさ
                hWnd,
                (HMENU)ID_SELECT_SLIDERX,
                hInst,
                NULL
            );
            SendMessage(hSliderX, TBM_SETRANGE, TRUE, MAKELPARAM(MINSLIDER, MAXSLIDER));
            SendMessage(hSliderX, TBM_SETPOS, TRUE, 100); // 初期値も設定
            return hSliderX;
        }
        else
        {
            HWND hSliderY = CreateWindowEx(
                0,
                TRACKBAR_CLASS,
                NULL,
                WS_OVERLAPPEDWINDOW | WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
                SLIDERPOSYX, SLIDERPOSY, SLIDERSIZEX, SLIDERSIZEY,//位置と大きさ
                hWnd,
                (HMENU)ID_SELECT_SLIDERY,
                hInst,
                NULL
            );
            SendMessage(hSliderY, TBM_SETRANGE, TRUE, MAKELPARAM(MINSLIDER, MAXSLIDER));
            SendMessage(hSliderY, TBM_SETPOS, TRUE, 10); // 初期位置も設定
            return hSliderY;
        }
}

// メニュー作成関数
HMENU GUI::CreateMainMenu() {
    HMENU hMenu = CreateMenu();

    HMENU hMIDI = CreatePopupMenu();
    AppendMenu(hMIDI, MF_STRING, ID_SELECT_MIDI, TEXT("SelectMIDI"));

    HMENU hCOLOR = CreatePopupMenu();
    AppendMenu(hCOLOR, MF_STRING, ID_SELECT_BACK_COLOR, TEXT("BackGroundCOLOR"));
    AppendMenu(hCOLOR, MF_STRING, ID_SELECT_NOTES_COLOR, TEXT("NotesCOLOR"));
    AppendMenu(hCOLOR, MF_STRING, ID_SELECT_ACTIVE_NOTES_COLOR, TEXT("NotesActiveCOLOR"));

    HMENU hPLAY = CreatePopupMenu();
    AppendMenu(hPLAY, MF_STRING, ID_SELECT_PLAY, TEXT("PlayNotes"));
    AppendMenu(hPLAY, MF_STRING, ID_SELECT_RESET, TEXT("ResetNotes"));
    AppendMenu(hPLAY, MF_STRING, ID_SELECT_STOP, TEXT("StopNotes"));
    HMENU hTYPE = CreatePopupMenu();
    AppendMenu(hTYPE, MF_STRING, ID_SELECT_TYPE01, TEXT("Type01"));
    AppendMenu(hTYPE, MF_STRING, ID_SELECT_TYPE02, TEXT("Type02"));
    HMENU hSIZE = CreatePopupMenu();
    AppendMenu(hSIZE, MF_STRING, ID_SELECT_SIZEX, TEXT("SizeX"));
    AppendMenu(hSIZE, MF_STRING, ID_SELECT_SIZEY, TEXT("SizeY"));
    

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMIDI, TEXT("SelectMIDI"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hCOLOR, TEXT("Color"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hPLAY, TEXT("Play"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hTYPE, TEXT("Type"));
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSIZE, TEXT("Size"));

    return hMenu;
}