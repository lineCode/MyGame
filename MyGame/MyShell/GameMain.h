#pragma once
#include "MySignal.h"
#include "PlatformInterface.h"




using namespace  XPG;

// this signal stuff is pretty old, not sure if it should be updated 
class ApplicationSignals
{
public:
	Signal0< void >                   event_Started;
	Signal0< void >                   event_Stopped;
	Signal0< void >                   event_BeforeStopped;
	Signal0< void >                   event_MouseCaptureChanged;

	Signal0< void >                   event_BeforeWindowResized;              // width, height, fullscreen
	Signal3< int, int, bool >         event_WindowResized;                    // width, height, fullscreen

	Signal1< float >                  event_Tick;
	Signal0< void >                   event_Render;

	//static LRESULT CALLBACK WndProcStatic( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	typedef LRESULT & LRESULTREF;
	typedef bool & boolRef;
	Signal6< boolRef, LRESULTREF, HWND, UINT, WPARAM, LPARAM >
		event_WinProcOverride;
};


class CGameMain :public ApplicationSignals
{
public:
	CGameMain();
	~CGameMain();

	bool InitGameMain(HINSTANCE ins, HWND hWnd, int nWidth, int nHeight, void* hIosMainWnd);


	void OnLButtonDown(int x, int y, __int64 iIndex, float fForce = .0f);
	void OnLButtonUp(int x, int y, __int64 iIndex);
	void OnMouseMove(int x, int y, __int64 iIndex, float fForce = .0f);


	void OnActivate(UINT nState, bool bMinimized);
	void OnRButtonUp(int x, int y);
	void OnMouseWheel(int zDelta);
	void OnLButtonDblClk(int x, int y);

	void OnKeyDown(UINT uiKey);
	void OnKeyUp(UINT uiKey);

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void ProcessGame();
	void DestoryGameMain();

	void ShowGUI();

private:
	class C3SoundGameObj* m_pBackSound;
	HWND m_hWnd;
	int m_nWidth;
	int m_nHeight;

	class D3D11Viewport* m_pD3D11Viewport;
	class D3DBufferSample* m_pSample;
};

extern CGameMain g_gameMain;

