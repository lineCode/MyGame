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

#include "imgui/imgui.h"
#include "imgui_impl_dx11.h"

CGameMain g_gameMain;



CGameMain::CGameMain()
{
	m_pBackSound = NULL;
	m_hWnd = NULL;
	m_pSample = new  D3DBufferSample();
	m_bInit = false;
}


CGameMain::~CGameMain()
{
	SAFE_DELETE(m_pD3D11Viewport);
	SAFE_DELETE(m_pSample);
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
extern  LRESULT ImGui_ImplDX11_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);
void CGameMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplDX11_WndProcHandler(hWnd, message, wParam, lParam);
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
	ImGui_ImplDX11_Init(hWnd, g_objDeviceManager.GetDevice(), g_objDeviceManager.GetDeviceContext());
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
	m_bInit = true;
	return true;
}

void CGameMain::OnResize()
{
	if (m_bInit&&m_hWnd)
	{
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		int nWidth = rect.right - rect.left;
		int nHeight = rect.bottom - rect.top;
		if (m_nWidth != nWidth || m_nHeight != nHeight)
		{
			m_nWidth = nWidth;
			m_nHeight = nHeight;
			m_pD3D11Viewport->Resize(m_nWidth, m_nHeight,false);
			g_objSprite.ResetSize(nWidth, nHeight);
		}
	}
}

void CGameMain::ProcessGame()
{
	if (!m_bInit)
	{
		return;
	}
	if (!m_tmUpdateAddTime.IsActive())
	{
		m_tmUpdateAddTime.Startup(1);
	}
	if (!m_tmAddTime.IsActive())
	{
		m_tmAddTime.Startup(1);
	}
	
	float fTimePass = (float)(m_tmUpdateAddTime.GetPassTime() / 1000.0);
	m_tmUpdateAddTime.Update();

	ProcressSound();
	ImGui_ImplDX11_NewFrame();
	event_Render();
	m_pD3D11Viewport->Begin();
	g_objSprite.ShowRect(100, 100, 200, 200, Vector4f(1,0,0,1), (float)(m_tmAddTime.GetPassTime() / 1000.0)*XM_PIDIV2);

	m_pSample->OnRender();
	//ShowGUI();
	

	m_pD3D11Viewport->Flip();
}

void CGameMain::DestoryGameMain()
{
	ImGui_ImplDX11_Shutdown();
	ShutDownSoundEngine();
	g_objSprite.Release();
	g_objDeviceManager.Release();
}

void CGameMain::ShowGUI()
{
	static char bufpass[64] = "";
	bool p_open = true;
	ImGuiIO& io = ImGui::GetIO();
	if (io.DisplaySize.x <= 0.0f || io.DisplaySize.y <= 0.0f)
		return;

	ImGui::SetNextWindowSize(ImVec2(200, io.DisplaySize.y), ImGuiSetCond_Always);
	if (ImGui::Begin("��Ч�鿴", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
	}
	ImGui::ShowTestWindow(&p_open);
	ImGui::Render();
}
