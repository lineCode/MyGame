#include "GameMain.h"
#include "C3Sound/C3SoundBase.h"
#include "C3Sound/C3Sound.h"
#include "C3Sound/C3SoundGameObj.h"
#include "C3Sound/Wwise_IDs.h"
#pragma comment(lib,"C3Sound.lib")

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "Graphics/Direct3D11/DeviceManager.h"
#include "Graphics/Direct3D11/D3D11Viewport.h"
#include "Graphics/Direct3D11/D3DBufferSample.h"
#include "Graphics/Direct3D11/Sprite.h"

CGameMain g_gameMain;



CGameMain::CGameMain()
{
	m_pBackSound = NULL;
	m_hWnd = NULL;
	m_pSample = new  D3DBufferSample();
}


CGameMain::~CGameMain()
{
	SAFE_DELETE(m_pD3D11Viewport);
	SAFE_DELETE(m_pSample);
	g_objSprite.Release();
	g_objDeviceManager.Release();

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
	g_objDeviceManager.InitD3DDevice();
	if (m_pD3D11Viewport == NULL)
	{
		m_pD3D11Viewport = new class D3D11Viewport();
	}

	m_hWnd = hWnd;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	bool bInit = m_pD3D11Viewport->Initialize(hWnd, nWidth, nHeight);

	m_pSample->InitRHI();
	g_objSprite.InitRHI();
	g_objSprite.ResetSize(nWidth, nHeight);


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
	m_pD3D11Viewport->Begin();
	g_objSprite.ShowRect(100, 100, 200, 200);
	g_objSprite.ShowRectTest(105, 105, 195, 195);
	m_pSample->OnRender();
	m_pD3D11Viewport->Flip();
}

void CGameMain::DestoryGameMain()
{
	ShutDownSoundEngine();
}
