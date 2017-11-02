#include "GameMain.h"
#include "C3Sound/C3SoundBase.h"
#include "C3Sound/C3Sound.h"
#include "C3Sound/C3SoundGameObj.h"
#include "C3Sound/Wwise_IDs.h"
#pragma comment(lib,"C3Sound\\C3Sound.lib")

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"


CGameMain g_gameMain;



CGameMain::CGameMain()
{
	m_pBackSound = NULL;
	m_hWnd = NULL;

}


CGameMain::~CGameMain()
{

}

void CGameMain::OnLButtonDown(int x, int y, __int64 iIndex, float fForce /*= .0f*/)
{

}

void CGameMain::OnLButtonUp(int x, int y, __int64 iIndex)
{

}

void CGameMain::OnMouseMove(int x, int y, __int64 iIndex, float fForce /*= .0f*/)
{

}

void CGameMain::OnActivate(UINT nState, bool bMinimized)
{

}

void CGameMain::OnRButtonUp(int x, int y)
{

}

void CGameMain::OnMouseWheel(int zDelta)
{

}

void CGameMain::OnLButtonDblClk(int x, int y)
{

}

void CGameMain::OnKeyDown(UINT uiKey)
{

}

void CGameMain::OnKeyUp(UINT uiKey)
{

}

bool CGameMain::InitGameMain(HINSTANCE ins, HWND hWnd, int nWidth, int nHeight, void* hIosMainWnd)
{
	m_hWnd = hWnd;
	m_nWidth = nWidth;
	m_nHeight = nHeight;


	bool bRet = InitSoundEngine("Res", "English(US)");
	if (!m_pBackSound)
	{
		m_pBackSound = CreateASoundGameObj("Birds");
		bRet = BindBank(m_pBackSound, "Birds.bnk");
		if (bRet)
		{
			m_pBackSound->PostEvent(AK::EVENTS::PLAY_01);
		}
	}
	return true;
}

void CGameMain::ProcessGame()
{
	ProcressSound();

	event_Render();
}

void CGameMain::DestoryGameMain()
{
	ShutDownSoundEngine();
}
