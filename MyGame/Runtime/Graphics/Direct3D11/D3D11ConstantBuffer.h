#pragma once
#include "dxbase.h"
#include <memory>

class D3D11ConstantBuffer
{
public:
	D3D11ConstantBuffer(UINT StartSlot_, ShaderType type_);
	~D3D11ConstantBuffer();

	void Release();
	void SetSize(int size_);
	void* Map();
	void Apply();
	/// Direct3D object.
	ID3D11Buffer* pConstantBuffer;
private:
	bool bMap;
	void* dataPtr;
	UINT StartSlot;
	ShaderType Type;
};
typedef std::shared_ptr<D3D11ConstantBuffer>D3D11ConstantBufferPtr;