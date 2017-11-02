#include "D3D11Buffer.h"
#include "DeviceManager.h"
#include "BaseFunc.h"

D3D11_BUFFER_DESC  g_Structured_Buffer_DESC = {
};
void D3D11Buffer::Init()
{
	
}



D3D11Buffer::D3D11Buffer()
{
	m_pResource = NULL;
	mBufferDesc = {};
	mInitData = {};
}


D3D11Buffer::~D3D11Buffer()
{
	SAFE_RELEASE(m_pResource);
}



D3D11VertexBuffer::D3D11VertexBuffer()
{

}

D3D11VertexBuffer::~D3D11VertexBuffer()
{

}


D3D11VertexBufferPtr D3D11VertexBuffer::Create(UINT ByteWidth, D3D11_USAGE Usage, UINT CPUAccessFlags, void* pSysMem /*=nullptr*/)
{
	D3D11VertexBuffer *instance = new D3D11VertexBuffer();
	D3D11VertexBufferPtr instancePtr(instance);
	instancePtr->mBufferDesc.ByteWidth = ByteWidth;
	instancePtr->mBufferDesc.Usage = Usage;
	instancePtr->mBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instancePtr->mBufferDesc.CPUAccessFlags = CPUAccessFlags;
	instancePtr->mInitData.pSysMem = pSysMem;
	instancePtr->mInitData.SysMemPitch = 0;//2d 3d 纹理的才有意义
	instancePtr->mInitData.SysMemSlicePitch = 0;//2d 3d 纹理的才有意义
	return instancePtr;
}

void D3D11VertexBuffer::CreateRHI()
{
	HRESULT result = S_FALSE;

	if (mInitData.pSysMem == NULL)
	{
		result = g_objDeviceManager.GetDevice()->CreateBuffer(&mBufferDesc, NULL, &m_pResource);
	}
	else
	{
		result = g_objDeviceManager.GetDevice()->CreateBuffer(&mBufferDesc, &mInitData, &m_pResource);
	}

	if (result != S_OK)
	{
		printf("Failed to create D3D11VertexBuffer");
		return;
	}
}
