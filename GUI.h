#pragma once
#include"renderer.h"
#include <windows.h>  
#include <dwrite.h>
#include <commdlg.h> // ChooseColor ‚ðŽg‚¤‚½‚ß‚É•K—v
#include<string>
#include <array>
#include <wchar.h>


#define ID_SELECT_MIDI 1001
#define ID_SELECT_BACK_COLOR 1002
#define ID_SELECT_NOTES_COLOR 1003
#define ID_SELECT_ACTIVE_NOTES_COLOR 1004
#define ID_SELECT_PLAY 1005
#define ID_SELECT_RESET 1006
#define ID_SELECT_STOP 1007
#define ID_SELECT_TYPE01 1008
#define ID_SELECT_TYPE02 1009
#define ID_SELECT_SIZEX 1010
#define ID_SELECT_SIZEY 1011
#define ID_SELECT_SLIDERX 1012
#define ID_SELECT_SLIDERY 1013

#define COLOR_NUMBER (4)
class GUI
{
	static D3DXVECTOR2 m_size;
	static std::string m_defaultPath;
	static std::string m_fileName;
	static bool m_ischange;
	
	static std::array<float, COLOR_NUMBER> m_notesColor, m_backColor, m_activeColor;
public:
	
	static bool SelectMidiFile(char* outFileName, DWORD outFileNameSize);

	static std::string GetFileName();

	static void SetIsChange(bool change);

	static bool GetIsChange();
	static D3DXVECTOR2 GetSetedSize();
	static void SetSetedSize(float size,bool XorY);

	static std::array<float, COLOR_NUMBER>GetNotesColor();
	static std::array<float, COLOR_NUMBER>GetBackColor();
	static std::array<float, COLOR_NUMBER>GetActiveColor();

	static void ShowColorDialog(HWND hwndOwner,int code);

	static HWND CreateSizeControl(HWND hWndParent,bool XorY);
	

	static HMENU CreateMainMenu();

	

};

