#include "D3D11Viewport.h"
#include "DeviceManager.h"
#include "BaseFunc.h"

D3D11Viewport::D3D11Viewport()
{
	m_pd3dDevice = DeviceManager::GetInstance().GetDevice();
	m_pDXGIFactory = DeviceManager::GetInstance().GetFactory();
	m_renderTargetView = NULL;
	m_depthStencilView = NULL;
	m_depthStencilBuffer = NULL;

	m_bkgColor[0] = 0.58f;
	m_bkgColor[1] = 0.58f;
	m_bkgColor[2] = 0.58f;
	m_bkgColor[3] = 1.f;

	m_bkgColor[0] = 0.1921568627450980392156862745098f;
	m_bkgColor[1] = 0.30196078431372549019607843137255f;
	m_bkgColor[2] = 0.47450980392156862745098039215686f;
	m_bkgColor[3] = 1.0f;

}

void D3D11Viewport::Release()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_depthStencilBuffer);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_depthStencilView);
}


D3D11Viewport::~D3D11Viewport()
{
	Release();
}

bool D3D11Viewport::Initialize(HWND hwnd, int nWidth, int nHeigth)
{
	m_hWnd = hwnd;
	m_nWidth = nWidth;
	m_nHeight = nHeigth;

	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC scDesc = { 0 };////���ṹ�����ý������൱����  
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//���������ݸ�ʽ  
	scDesc.BufferDesc.Width = m_nWidth;  //��������С 
	scDesc.BufferDesc.Height = m_nHeight;
	scDesc.BufferDesc.RefreshRate.Numerator = 60; //ˢ���ʣ�һ�������趨���� 
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //�̶����� 
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //�̶�����
	scDesc.BufferCount = 1; //����������  
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //UsageΪRender Target Output  
	scDesc.Flags = 0;
	scDesc.OutputWindow = m_hWnd; //�����ھ��  
	scDesc.SampleDesc.Count = 1; //������  
	scDesc.SampleDesc.Quality = 0; //����֧�ֵȼ�
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL; //���ò��� 
	scDesc.Windowed = true;//����ģʽ 

	hr = m_pDXGIFactory->CreateSwapChain(m_pd3dDevice, &scDesc, &m_pSwapChain);
	if (!CreateWindowSizeDependentResources())
	{
		hr = S_FALSE;
	}
	return hr == S_OK;
}

void D3D11Viewport::Resize(int InSizeX, int InSizeY, bool bInIsFullscreen)
{
	if (m_nWidth == InSizeX && m_nHeight == InSizeY)
	{
		return;
	}
	m_nWidth = InSizeX;
	m_nHeight = InSizeY;
	CreateWindowSizeDependentResources();
}

bool D3D11Viewport::CreateWindowSizeDependentResources()
{
	SAFE_RELEASE(m_depthStencilBuffer);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_depthStencilView);

	HRESULT hr;
	m_pSwapChain->ResizeBuffers(1, m_nWidth, m_nHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	//	3. ����RenderTargetView,D3D11�д�����ͼ��Ҫ��Ӧ����Դ
	ID3D11Texture2D *backBuffer(NULL);
	//��ȡ�󻺳�����ַ
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	//������ͼ
	hr = m_pd3dDevice->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView);
	if (FAILED(hr))return false;

	//�ͷź󻺳�������  
	SAFE_RELEASE(backBuffer);

	/************************************************************************/
	/*        4. ������ȡ�ģ�建��������Ӧ��ͼ,����������Ҫ������һ��2ά����                                                             */
	/************************************************************************/
	D3D11_TEXTURE2D_DESC dsDesc;
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.Width = m_nWidth;
	dsDesc.Height = m_nHeight;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.MipLevels = 1;
	dsDesc.ArraySize = 1;
	dsDesc.CPUAccessFlags = 0;
	dsDesc.SampleDesc.Count = 1;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.MiscFlags = 0;
	dsDesc.Usage = D3D11_USAGE_DEFAULT;

	int w = m_nWidth;
	int h = m_nHeight;

	hr = m_pd3dDevice->CreateTexture2D(&dsDesc, nullptr, &m_depthStencilBuffer);

	//������ͼ����DepthStencilView
	// Not needed since this is a 2d texture
	hr = m_pd3dDevice->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_depthStencilView);

	//������������ͼ��Ȼ��Ҫ�󶨵���Ⱦ����
	//	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = (float)(m_nWidth)-m_viewport.TopLeftX*2;
	m_viewport.Height = (float)(m_nHeight)-m_viewport.TopLeftY*2;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	return true;
}


void D3D11Viewport::Begin()
{
	ID3D11DeviceContext*  d3dcontext = g_objDeviceManager.GetDeviceContext();
	ID3D11RenderTargetView* rt[1] = { this->GetRenderTargetView() };
	d3dcontext->OMSetRenderTargets(1, rt, this->GetDepthStencilView());
	D3D11_VIEWPORT vp = this->GetViewPort();
	D3D11_VIEWPORT vps[3] = { vp ,vp ,vp };
	d3dcontext->RSSetViewports(3, vps);
	d3dcontext->RSSetState(NULL);
	d3dcontext->OMSetDepthStencilState(NULL, 0);
	d3dcontext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	d3dcontext->ClearDepthStencilView(this->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	float color[4];
	color[0] = 0.58f;
	color[1] = 0.58f;
	color[2] = 0.58f;
	color[3] = 1.f;
	d3dcontext->ClearRenderTargetView(*rt, this->GetBkgColor());
}

void D3D11Viewport::Lock()
{
	ID3D11DeviceContext*  d3dcontext = g_objDeviceManager.GetDeviceContext();
	ID3D11RenderTargetView* rt[1] = {NULL };
	d3dcontext->OMSetRenderTargets(1, rt, NULL);
}

void D3D11Viewport::UnLock()
{
	ID3D11DeviceContext*  d3dcontext = g_objDeviceManager.GetDeviceContext();
	ID3D11RenderTargetView* rt[1] = { this->GetRenderTargetView() };
	d3dcontext->OMSetRenderTargets(1, rt, this->GetDepthStencilView());
}

void D3D11Viewport::Flip()
{
	if (m_pSwapChain->Present(0, 0) == S_OK)
	{

	}
}

