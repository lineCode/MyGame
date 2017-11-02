#include "DeviceManager.h"
#include "BaseFunc.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")//
#pragma comment(lib,"d3dcompiler.lib") //������ɫ������

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
	//�����豸ID3D11Device���豸������ID3D11DeviceContext
	HRESULT hr = D3D11CreateDevice(
		NULL, //��ΪNULL��ѡ��Ĭ�ϵ�������
		D3D_DRIVER_TYPE_HARDWARE,//DriverType�����������ͣ�һ���������ѡ��Ӳ������
		NULL,//FlagsΪ��ѡ������һ��ΪNULL ,D3D11_CREATE_DEVICE_DEBUG
		createDeviceFlags,
		featureLevels,//Ϊ�����ṩ������������ȼ���һ������
		numFeatureLevels,//������Ԫ�ظ���
		D3D11_SDK_VERSION,//SDKVersion�㶨ΪD3D11_SDK_VERSION
		&m_pd3dDevice,
		&curLevel,//pFeatureLevelΪ������ѡ�е������ȼ�
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
