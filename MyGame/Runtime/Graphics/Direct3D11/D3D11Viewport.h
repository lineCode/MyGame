#pragma once
/************************************************************************/
/* �ӿ� ����һ��������                                                                     */
/************************************************************************/
#include <d3d11.h>

class D3D11Viewport
{
public:
	D3D11Viewport();
	~D3D11Viewport();
	bool Initialize(HWND hwnd, int nWidth, int nHeigth);
	void Resize(int InSizeX, int InSizeY, bool bInIsFullscreen);
	void Release();
	void  SetBkgColor(const FLOAT ColorRGBA[4])
	{
		m_bkgColor[0] = ColorRGBA[0];
		m_bkgColor[1] = ColorRGBA[1];
		m_bkgColor[2] = ColorRGBA[2];
	}
	const float* GetBkgColor()
	{
		return m_bkgColor;
	}

	void Begin();
	void Lock();
	void UnLock();

	void Flip();

public:
	ID3D11RenderTargetView* GetRenderTargetView() { return m_renderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() { return m_depthStencilView; }
	IDXGISwapChain* GetDXGISwapChain() { return m_pSwapChain; }
	D3D11_VIEWPORT GetViewPort() { return m_viewport; } //����ȥ�޸��ӿڵĲ���

private:
	bool CreateWindowSizeDependentResources();
private:
	
	IDXGIFactory*          m_pDXGIFactory;
	ID3D11Device*           m_pd3dDevice;

	IDXGISwapChain*         m_pSwapChain;
	ID3D11RenderTargetView	*m_renderTargetView;		//��Ⱦ������ͼ
	ID3D11DepthStencilView	*m_depthStencilView;		//���/ģ����ͼ
	ID3D11Texture2D			*m_depthStencilBuffer;		//���/ģ�建����


	D3D11_VIEWPORT m_viewport;
	int m_nWidth;
	int m_nHeight;
	HWND		m_hWnd;				//���ھ��
	float m_bkgColor[4];
};

