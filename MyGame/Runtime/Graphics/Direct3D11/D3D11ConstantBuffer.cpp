#include "D3D11ConstantBuffer.h"
#include "DeviceManager.h"
#include "BaseFunc.h"

D3D11ConstantBuffer::D3D11ConstantBuffer(UINT StartSlot_, ShaderType type_)
{
	pConstantBuffer = NULL;
	bMap = false;
	dataPtr = NULL;
	StartSlot = StartSlot_;
	Type = type_;
}

D3D11ConstantBuffer::~D3D11ConstantBuffer()
{
	Release();
}

void D3D11ConstantBuffer::Release()
{
	SAFE_RELEASE(pConstantBuffer);
	bMap = false;
	dataPtr = NULL;
}

void D3D11ConstantBuffer::SetSize(int size_)
{
	Release();
	D3D11_BUFFER_DESC bufferDesc;

	// Round up to next 16 bytes
	size_ += 15;
	size_ &= 0xfffffff0;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = size_;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	g_objDeviceManager.GetDevice()->CreateBuffer(&bufferDesc, NULL, (ID3D11Buffer**)&pConstantBuffer);
	if (!pConstantBuffer)
	{
		printf("Failed to create constant buffer");
		return;
	}
}

void* D3D11ConstantBuffer::Map()
{
	if (bMap && dataPtr)
	{
		return dataPtr;
	}
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	g_objDeviceManager.GetDeviceContext()->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	bMap = true;
	dataPtr = mappedResource.pData;
	return mappedResource.pData;
}

void D3D11ConstantBuffer::Apply()
{
	if (bMap)
	{
		bMap = false;
		dataPtr = NULL;
		g_objDeviceManager.GetDeviceContext()->Unmap(pConstantBuffer, 0);
	}
	if (Type == VS)
	{
		g_objDeviceManager.GetDeviceContext()->VSSetConstantBuffers(StartSlot, 1, &pConstantBuffer);
	}
	else if (Type == GS)
	{
		g_objDeviceManager.GetDeviceContext()->GSSetConstantBuffers(StartSlot, 1, &pConstantBuffer);
	}
	else if (Type == PS)
	{
		g_objDeviceManager.GetDeviceContext()->PSSetConstantBuffers(StartSlot, 1, &pConstantBuffer);
	}
	else if (Type == HS)
	{
		g_objDeviceManager.GetDeviceContext()->HSSetConstantBuffers(StartSlot, 1, &pConstantBuffer);
	}
	else if (Type == DS)
	{
		g_objDeviceManager.GetDeviceContext()->DSSetConstantBuffers(StartSlot, 1, &pConstantBuffer);
	}
}
