#include "DeviceManager.h"
#include "BaseFunc.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")//
#pragma comment(lib,"d3dcompiler.lib") //编译着色器依赖

DeviceManager* DeviceManager::GetInstancePtr()
{
	static DeviceManager _instance;
	return &_instance;
}

DeviceManager& DeviceManager::GetInstance()
{
	return *(GetInstancePtr());
}


DeviceManager::DeviceManager()
{
	m_pImmediateContext = NULL;
	m_pd3dDevice = NULL;
	m_pDXGIFactory = NULL;
}


DeviceManager::~DeviceManager()
{
	Release();
}

void DeviceManager::InitD3DDevice()
{

	D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_UNKNOWN;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL	curLevel;
	//创建设备ID3D11Device和设备上下文ID3D11DeviceContext
	HRESULT hr = D3D11CreateDevice(
		NULL, //设为NULL以选择默认的适配器
		D3D_DRIVER_TYPE_HARDWARE,//DriverType设置驱动类型，一般毫无疑问选择硬件加速
		NULL,//Flags为可选参数，一般为NULL ,D3D11_CREATE_DEVICE_DEBUG
		createDeviceFlags,
		featureLevels,//为我们提供给程序的特征等级的一个数组
		numFeatureLevels,//数组中元素个数
		D3D11_SDK_VERSION,//SDKVersion恒定为D3D11_SDK_VERSION
		&m_pd3dDevice,
		&curLevel,//pFeatureLevel为最后程序选中的特征等级
		&m_pImmediateContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Craete device failed!", L"ERROR", MB_OK);
		return;
	}

	IDXGIDevice *pDxgiDevice(NULL);
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDxgiDevice));
	if (FAILED(hr))
	{
		return;
	}
	IDXGIAdapter *pDxgiAdapter(NULL);
	hr = pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDxgiAdapter));
	hr = pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&m_pDXGIFactory));


	SAFE_RELEASE(pDxgiAdapter);
	SAFE_RELEASE(pDxgiDevice);
}

void DeviceManager::Release()
{
	SAFE_RELEASE(m_pImmediateContext);

/*
#if defined(DEBUG) || defined(_DEBUG)
	ID3D11Debug *d3dDebug;
	HRESULT hr = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	SAFE_RELEASE(d3dDebug);
#endif*/


	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pDXGIFactory);
}
