#pragma once
#include "dxbase.h"
#include "D3D11Buffer.h"


class D3DBufferSample
{
public:
	D3DBufferSample();
	~D3DBufferSample();
	void InitRHI();
	void InitShader();
	void Release();
	void OnRenderBefore();
	void OnRender();
	void OnRenderCS();
private:

	void CreateIndexBuffer( int nSize);

	ID3D11ShaderResourceView* BoneTransformsSRV;
	ID3D11Buffer* m_BonesBuffer;

	ID3D11ShaderResourceView* BoneTransformsStructSRV;
	ID3D11Buffer* m_BonesStructBuffer;

	ID3D11Buffer* m_StreamOutVB;
	ID3D11Buffer* m_StreamOutIB;
	ID3D11Buffer* m_StreamOutIBTmp;
	ID3D11Buffer* m_StreamOutIBTmp1;


	ID3D11InputLayout * m_pInputLayout;
	std::shared_ptr<class D3D11Shader> shaderPtr0;
	std::shared_ptr<class D3D11Shader> shaderPtr1;
	std::shared_ptr<class D3D11Shader> shaderPtr2;
	D3D11VertexBufferPtr VertexBufferPtr;
	ID3D11Buffer	*m_indexBuffer;


	ID3D11Buffer* m_StructBuffer;
	ID3D11UnorderedAccessView* m_pStructuredBufferUAV;
	ID3D11ShaderResourceView* m_pStructuredBufferSRV;

	
	struct BufferStruct
	{
		UINT color[4];
	};

};

