#pragma once
#include "PlatformInterface.h"
#include "dxbase.h"
#include <memory>
#include "D3D11ConstantBuffer.h"
#include <unordered_map>

#define HP_HashMap std::unordered_map

typedef std::vector<D3D11_INPUT_ELEMENT_DESC> LayoutVector;

struct ShaderParameter
{
	ShaderParameter() :
		type_(VS),
		buffer_(0),
		offset_(0),
		size_(0),
		bufferPtr_(0)
	{
	}
	ShaderParameter(ShaderType type, const std::string& name, unsigned buffer, unsigned offset, unsigned size, D3D11ConstantBufferPtr ptr = 0) :
		type_(type),
		name_(name),
		buffer_(buffer),
		offset_(offset),
		size_(size),
		bufferPtr_(ptr)
	{

	}
	/// %Shader type.
	ShaderType type_;
	/// Name of the parameter.
	std::string name_;
	/// Constant buffer index.
	unsigned buffer_;
	/// Offset in constant buffer.
	unsigned offset_;
	/// Size of parameter in bytes.
	unsigned size_;
	/// Constant buffer pointer. Defined only in shader programs.
	D3D11ConstantBufferPtr bufferPtr_;
};

class D3D11Shader
{
public:
	D3D11Shader();
	~D3D11Shader();
	bool CompileFromFile(const char* pFileName, ShaderType type_);
	HRESULT CreateGPUBuffer(ShaderType type_);
	HRESULT CreateGeometryShaderWithStreamOutput(const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,int nCount, UINT   RasterizedStream=0);

	ID3D11VertexShader* GetVertexShader() {	return m_vertexShader;}
	ID3D11PixelShader* GetPixelShader() { return m_pixelShader; }

	template< class VertexTypes >
	HRESULT CreateInputLayout(ID3D11InputLayout **ppInputLayout);



	void VSSetConstantBuffers(const char* name, void* pBuffer, int nSize = 0);
	void HSSetConstantBuffers(const char* name, void* pBuffer, int nSize = 0);
	void DSSetConstantBuffers(const char* name, void* pBuffer, int nSize = 0);
	void GSSetConstantBuffers(const char* name, void* pBuffer, int nSize = 0);
	void PSSetConstantBuffers(const char* name, void* pBuffer, int nSize = 0);

	/************************************************************************/
	/* 采样状态由外层调用                                                                     */
	/************************************************************************/
	void VSSetShaderResources(const char* name, ID3D11ShaderResourceView* pShaderResourceViews = NULL);
	void GSSetShaderResources(const char* name, ID3D11ShaderResourceView* pShaderResourceViews = NULL);
	void PSSetShaderResources(const char* name, ID3D11ShaderResourceView* pShaderResourceViews = NULL);
	void CSSetShaderResources(const char* name, ID3D11ShaderResourceView* pShaderResourceViews = NULL);

	void CSSetUnorderedAccessViews(const char* name, ID3D11UnorderedAccessView* pUnorderedAccessView, const UINT *pUAVInitialCounts);

	void Apply();
	void ApplyGeometryShaderOutPut();
private:
	static HRESULT CreateInputLayout(LayoutVector vecLayout, const void *pBuffer, int nSize, ID3D11InputLayout **ppInputLayout);

	void SetConstantBuffers(ShaderType type_, const char* name, void* pBuffer, int nSize = 0);
	void SetShaderResources(ShaderType type_, const char* name, ID3D11ShaderResourceView* pShaderResourceViews = NULL);
	/// Inspect the constant parameters and input layout (if applicable) from the shader bytecode.
	void ParseParameters(unsigned char* bufData, unsigned bufSize, ShaderType type_);
private:
	/// Bytecode. Needed for inspecting the input signature and parameters.
	std::vector<unsigned char> vertexbyteCode_;
	std::vector<unsigned char> pixelbyteCode_;
	std::vector<unsigned char> geometrybytecode_;
	std::vector<unsigned char> geometryOutbytecode_;
	std::vector<unsigned char> hsbytecode_;
	std::vector<unsigned char> dsbytecode_;
	std::vector<unsigned char> csbytecode_;


	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11GeometryShader *m_pGeometryShader;
	ID3D11GeometryShader *m_pGeometryShaderOutPut;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;
	ID3D11ComputeShader* m_pComputeShader;




	bool useTextureUnit_[MAX_TEXTURE_UNITS];


	HP_HashMap<unsigned, ShaderParameter>  parametersArray[MAX_SHADER];
	HP_HashMap<std::string, unsigned int> shaderresourcesArray[MAX_SHADER];

};

template< class VertexTypes >
HRESULT D3D11Shader::CreateInputLayout(ID3D11InputLayout  **ppInputLayout)
{
	LayoutVector vecLayout;
	for (int i = 0; i < VertexTypes::InputElementCount; ++i)
	{
		vecLayout.push_back(VertexTypes::InputElements[i]);
	}
	return CreateInputLayout(vecLayout, &vertexbyteCode_[0], vertexbyteCode_.size(), ppInputLayout);
}

