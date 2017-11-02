#pragma once

#include <d3d11.h>

/************************************************************************/
/*           DX π‹¿Ì∆˜                                                           */
/************************************************************************/
class DeviceManager
{
public:
	static DeviceManager& GetInstance();
	static DeviceManager* GetInstancePtr();

public:
	DeviceManager();
	~DeviceManager();
	/** If it hasn't been initialized yet, initializes the D3D device. */
	virtual void InitD3DDevice();
	virtual void Release();
	// Accessors.
	ID3D11Device* GetDevice() const
	{
		return m_pd3dDevice;
	}
	ID3D11DeviceContext* GetDeviceContext() const
	{
		return m_pImmediateContext;
	}

	IDXGIFactory* GetFactory() const
	{
		return m_pDXGIFactory;
	}

private:
	ID3D11Device*           m_pd3dDevice;
	ID3D11DeviceContext*    m_pImmediateContext;
	IDXGIFactory*          m_pDXGIFactory;

};
#define  g_objDeviceManager DeviceManager::GetInstance()

