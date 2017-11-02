#include "D3D11Shader.h"
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include "BaseFunc.h"
#include "DeviceManager.h"
#include "StringHash.h"
#include "Math/Matrix4x4.h"

D3D11Shader::D3D11Shader()
{
	m_vertexShader = NULL;
	m_pixelShader = NULL;
	m_pGeometryShader = NULL;
	m_pGeometryShaderOutPut = NULL;
	m_pHullShader = NULL;
	m_pDomainShader = NULL;
	m_pComputeShader = NULL;

	for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
		useTextureUnit_[i] = false;
}


D3D11Shader::~D3D11Shader()
{
	SAFE_RELEASE(m_vertexShader);
	SAFE_RELEASE(m_pixelShader);
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pGeometryShaderOutPut);
	SAFE_RELEASE(m_pHullShader);
	SAFE_RELEASE(m_pDomainShader);
	SAFE_RELEASE(m_pComputeShader);

}

bool D3D11Shader::CompileFromFile(const char* pFileName, ShaderType type_)
{
	const char* entryPoint = 0;
	const char* profile = 0;
	if (type_ == VS)
	{
		entryPoint = "VS";
		profile = "vs_5_0";
	}
	else if (type_ == PS)
	{
		entryPoint = "PS";
		profile = "ps_5_0";
	}
	else if (type_ == GS)
	{
		entryPoint = "GSMain";
		profile = "gs_5_0";
	}
	else if (type_ == GS_OUTPUT)
	{
		entryPoint = "GSMain";
		profile = "gs_5_0";
	}
	else if (type_ == HS)
	{
		entryPoint = "HS";
		profile = "hs_5_0";
	}
	else if (type_ == DS)
	{
		entryPoint = "DS";
		profile = "ds_5_0";
	}
	else if (type_ == CS)
	{
		entryPoint = "CS";
		profile = "cs_5_0";
	}
	unsigned flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR; //行优先
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG; //debug
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION; //不要优化
#endif
	std::vector<D3D_SHADER_MACRO> macros;
	D3D_SHADER_MACRO endMacro;
	endMacro.Name = 0;
	endMacro.Definition = 0;
	macros.push_back(endMacro);

	ID3DBlob* shaderCode = 0;
	ID3DBlob* errorMsgs = 0;


	HRESULT result =
		D3DCompileFromFile(gAnsiToUnicode(pFileName).c_str(),
			&macros[0],
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint,
			profile,
			flags,
			0,
			&shaderCode, &errorMsgs);
	if (result != S_OK)
	{
		DebugMyMsg("Error compiling shader.  Check shader-error.txt for message.\n");
		DebugMyMsg("%s\n", (char*)(errorMsgs->GetBufferPointer()));
		assert(false);
		SAFE_RELEASE(errorMsgs);
		return false;
	}
	unsigned char* bufData = (unsigned char*)shaderCode->GetBufferPointer();
	unsigned bufSize = (unsigned)shaderCode->GetBufferSize();
	ParseParameters(bufData, bufSize, type_);
	if (type_ == VS)
	{
		vertexbyteCode_.resize((unsigned)shaderCode->GetBufferSize());
		memcpy(&vertexbyteCode_[0], shaderCode->GetBufferPointer(), vertexbyteCode_.size());
	}
	else if (type_ == PS)
	{
		pixelbyteCode_.resize((unsigned)shaderCode->GetBufferSize());
		memcpy(&pixelbyteCode_[0], shaderCode->GetBufferPointer(), pixelbyteCode_.size());
	}
	else if (type_ == GS)
	{
		geometrybytecode_.resize((unsigned)shaderCode->GetBufferSize());
		memcpy(&geometrybytecode_[0], shaderCode->GetBufferPointer(), geometrybytecode_.size());
	}
	else if (type_ == GS_OUTPUT)
	{
		geometryOutbytecode_.resize((unsigned)shaderCode->GetBufferSize());
		memcpy(&geometryOutbytecode_[0], shaderCode->GetBufferPointer(), geometryOutbytecode_.size());
	}
	else if (type_ == HS)
	{
		hsbytecode_.resize((unsigned)shaderCode->GetBufferSize());
		memcpy(&hsbytecode_[0], shaderCode->GetBufferPointer(), hsbytecode_.size());
	}
	else if (type_ == DS)
	{
		dsbytecode_.resize((unsigned)shaderCode->GetBufferSize());
		memcpy(&dsbytecode_[0], shaderCode->GetBufferPointer(), dsbytecode_.size());
	}
	else if (type_ == CS)
	{
		csbytecode_.resize((unsigned)shaderCode->GetBufferSize());
		memcpy(&csbytecode_[0], shaderCode->GetBufferPointer(), csbytecode_.size());
	}
	SAFE_RELEASE(shaderCode);
	return true;
}

HRESULT D3D11Shader::CreateGPUBuffer(ShaderType type_)
{
	if (type_ == VS)
	{
		HRESULT result = g_objDeviceManager.GetDevice()->CreateVertexShader(
			&vertexbyteCode_[0],
			vertexbyteCode_.size(),
			(ID3D11ClassLinkage *)NULL,
			&m_vertexShader);
		return result;
	}
	else if (type_ == PS)
	{
		HRESULT result = g_objDeviceManager.GetDevice()->CreatePixelShader(
			&pixelbyteCode_[0],
			pixelbyteCode_.size(),
			(ID3D11ClassLinkage *)NULL,
			&m_pixelShader);
		return result;
	}
	else if (type_ ==GS)
	{
		HRESULT result = g_objDeviceManager.GetDevice()->CreateGeometryShader(
			&geometrybytecode_[0],
			geometrybytecode_.size(),
			(ID3D11ClassLinkage *)NULL,
			&m_pGeometryShader);
		return result;
	}
	else if (type_ == HS)
	{
		HRESULT result = g_objDeviceManager.GetDevice()->CreateHullShader(
			&hsbytecode_[0],
			hsbytecode_.size(),
			(ID3D11ClassLinkage *)NULL,
			&m_pHullShader);
		return result;
	}
	else if (type_ == DS)
	{
		HRESULT result = g_objDeviceManager.GetDevice()->CreateDomainShader(
			&dsbytecode_[0],
			dsbytecode_.size(),
			(ID3D11ClassLinkage *)NULL,
			&m_pDomainShader);
		return result;
	}
	else if (type_ == CS)
	{
		HRESULT result = g_objDeviceManager.GetDevice()->CreateComputeShader(
			&csbytecode_[0],
			csbytecode_.size(),
			(ID3D11ClassLinkage *)NULL,
			&m_pComputeShader);
		return result;
	}
	return S_FALSE;
}

HRESULT D3D11Shader::CreateGeometryShaderWithStreamOutput(const D3D11_SO_DECLARATION_ENTRY *pSODeclaration, 
	int nCount,
	UINT   RasterizedStream)
{
	//不经过光栅化阶段用 RasterizedStream = D3D11_SO_NO_RASTERIZED_STREAM 
	HRESULT result = g_objDeviceManager.GetDevice()->CreateGeometryShaderWithStreamOutput(
		&vertexbyteCode_[0], vertexbyteCode_.size(), pSODeclaration,
		nCount, NULL, 0, RasterizedStream, NULL, &m_pGeometryShaderOutPut);
	return result;
}

HRESULT D3D11Shader::CreateInputLayout(LayoutVector vecLayout, const void *pBuffer, int nSize, ID3D11InputLayout **ppInputLayout)
{
	HRESULT result = g_objDeviceManager.GetDevice()->CreateInputLayout(
		&vecLayout[0],
		vecLayout.size(),
		pBuffer,
		nSize,
		(ID3D11InputLayout**)ppInputLayout);
	return result;
}


void D3D11Shader::ParseParameters(unsigned char* bufData, unsigned bufSize, ShaderType type_)
{
	ID3D11ShaderReflection* reflection = 0;
	D3D11_SHADER_DESC shaderDesc;

	D3DReflect(bufData, bufSize, IID_ID3D11ShaderReflection, (void**)&reflection);
	if (!reflection)
	{
		printf("Failed to reflect vertex shader's input signature");
		return;
	}

	reflection->GetDesc(&shaderDesc);

	HP_HashMap<std::string, unsigned> cbRegisterMap;
	for (unsigned i = 0; i < shaderDesc.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
		reflection->GetResourceBindingDesc(i, &resourceDesc);
		std::string resourceName(resourceDesc.Name);
		if (resourceDesc.Type == D3D_SIT_CBUFFER)
			cbRegisterMap[resourceName] = resourceDesc.BindPoint;
		else if (resourceDesc.Type == D3D_SIT_SAMPLER && resourceDesc.BindPoint < MAX_TEXTURE_UNITS)
			useTextureUnit_[resourceDesc.BindPoint] = true;
		else if (resourceDesc.Type == D3D_SIT_TEXTURE)
		{
			HP_HashMap<std::string, unsigned int> & shaderresources = shaderresourcesArray[type_];
			shaderresources[resourceName] = resourceDesc.BindPoint;
		}
		else if (resourceDesc.Type == D3D_SIT_STRUCTURED)
		{
			HP_HashMap<std::string, unsigned int> & shaderresources = shaderresourcesArray[type_];
			shaderresources[resourceName] = resourceDesc.BindPoint;
		}
		else if (resourceDesc.Type == D3D_SIT_UAV_RWSTRUCTURED)
		{
			HP_HashMap<std::string, unsigned int> & shaderresources = shaderresourcesArray[type_];
			shaderresources[resourceName] = resourceDesc.BindPoint;
		}
	}

	for (unsigned i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* cb = reflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC cbDesc;
		cb->GetDesc(&cbDesc);
		unsigned cbRegister = cbRegisterMap[std::string(cbDesc.Name)];
		D3D11ConstantBufferPtr newShader(new D3D11ConstantBuffer(cbRegister, type_));
		newShader->SetSize(cbDesc.Size);
		for (unsigned j = 0; j < cbDesc.Variables; ++j)
		{
			ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC varDesc;
			var->GetDesc(&varDesc);
			std::string varName(varDesc.Name);
			HP_HashMap<unsigned, ShaderParameter>& parameters_ = parametersArray[type_];
			parameters_[StringHash(varName)] = ShaderParameter(type_, varName, cbRegister, varDesc.StartOffset, varDesc.Size, newShader);
	
		}
	}

}

void D3D11Shader::SetConstantBuffers(ShaderType type_, const char* name, void* pBuffer, int nSize /*= 0*/)
{
	StringHash varName(name);
	HP_HashMap<unsigned, ShaderParameter>& parameters_ = parametersArray[type_];
	HP_HashMap<unsigned, ShaderParameter>::iterator iter = parameters_.find(varName);
	if (iter != parameters_.end())
	{
		ShaderParameter parameter = iter->second;
		D3D11ConstantBufferPtr buffer_ = parameter.bufferPtr_;
		if (buffer_ != NULL)
		{
			if (nSize == 0)
			{
				nSize = parameter.size_;
			}
			void* dataPtr = buffer_->Map();
			unsigned char*  pDataBuffer = (unsigned char*)dataPtr;
			memcpy_s(pDataBuffer + parameter.offset_, nSize, pBuffer, nSize);

			struct MatrixBuffer
			{
				Matrix4x4f  worldMatrix;
				Matrix4x4f  viewMatrix;
				Matrix4x4f  projectionMatrix;
			};
			MatrixBuffer a;
			memcpy_s(&a, sizeof(MatrixBuffer), dataPtr, sizeof(MatrixBuffer));
			int aa;
			aa = 100;
		}
	}
}

void D3D11Shader::VSSetConstantBuffers(const char* name, void* pBuffer, int nSize /*= 0*/)
{
	SetConstantBuffers(VS, name, pBuffer, nSize);
}

void D3D11Shader::HSSetConstantBuffers(const char* name, void* pBuffer, int nSize /*= 0*/)
{
	SetConstantBuffers(HS, name, pBuffer, nSize);
}

void D3D11Shader::DSSetConstantBuffers(const char* name, void* pBuffer, int nSize /*= 0*/)
{
	SetConstantBuffers(DS, name, pBuffer, nSize);
}

void D3D11Shader::GSSetConstantBuffers(const char* name, void* pBuffer, int nSize /*= 0*/)
{
	SetConstantBuffers(GS, name, pBuffer, nSize);
}

void D3D11Shader::PSSetConstantBuffers(const char* name, void* pBuffer, int nSize /*= 0*/)
{
	SetConstantBuffers(PS, name, pBuffer, nSize);
}


void D3D11Shader::VSSetShaderResources(const char* name , ID3D11ShaderResourceView* pShaderResourceViews /*= NULL*/)
{
	SetShaderResources(VS, name, pShaderResourceViews);
}

void D3D11Shader::GSSetShaderResources(const char* name, ID3D11ShaderResourceView* pShaderResourceViews /*= NULL*/)
{
	SetShaderResources(GS, name, pShaderResourceViews);
}

void D3D11Shader::PSSetShaderResources(const char* name, ID3D11ShaderResourceView* pShaderResourceViews /*= NULL*/)
{
	SetShaderResources(PS, name, pShaderResourceViews);
}

void D3D11Shader::CSSetShaderResources(const char* name, ID3D11ShaderResourceView* pShaderResourceViews /*= NULL*/)
{
	SetShaderResources(CS, name, pShaderResourceViews);
}


void D3D11Shader::CSSetUnorderedAccessViews(const char* name, ID3D11UnorderedAccessView* pUnorderedAccessView, const UINT *pUAVInitialCounts)
{
	HP_HashMap<std::string, unsigned int>& shaderresources_vs = shaderresourcesArray[CS];
	HP_HashMap<std::string, unsigned int>::iterator iter = shaderresources_vs.find(name);
	if (iter != shaderresources_vs.end())
	{
		ID3D11DeviceContext		*m_deviceContext = g_objDeviceManager.GetDeviceContext();
		m_deviceContext->CSSetUnorderedAccessViews(iter->second, 1, &pUnorderedAccessView, pUAVInitialCounts);
	}
}

void D3D11Shader::SetShaderResources(ShaderType type_, const char* name, ID3D11ShaderResourceView* pShaderResourceViews /*= NULL*/)
{
	HP_HashMap<std::string, unsigned int>& shaderresources_vs = shaderresourcesArray[type_];
	HP_HashMap<std::string, unsigned int>::iterator iter = shaderresources_vs.find(name);
	if (iter != shaderresources_vs.end())
	{
		ID3D11DeviceContext		*m_deviceContext = g_objDeviceManager.GetDeviceContext();
		if (type_ == VS)
		{
			m_deviceContext->VSSetShaderResources(iter->second, 1, &pShaderResourceViews);
		}
		else if (type_ == HS)
		{
			m_deviceContext->HSSetShaderResources(iter->second, 1, &pShaderResourceViews);
		}
		else if (type_ == DS)
		{
			m_deviceContext->DSSetShaderResources(iter->second, 1, &pShaderResourceViews);
		}
		else	if (type_ == GS)
		{
			m_deviceContext->GSSetShaderResources(iter->second, 1, &pShaderResourceViews);
		}
		else	if (type_ == CS)
		{
			m_deviceContext->CSSetShaderResources(iter->second, 1, &pShaderResourceViews);
		}
		else	if (type_ == PS)
		{
			m_deviceContext->PSSetShaderResources(iter->second, 1, &pShaderResourceViews);
		}
	}
}


void D3D11Shader::Apply()
{
	for (int i = 0; i < MAX_SHADER; ++i)
	{
		HP_HashMap<unsigned, ShaderParameter>& pTmp = parametersArray[i];
		HP_HashMap<unsigned, ShaderParameter>::iterator iter = pTmp.begin();
		for (; iter != pTmp.end(); ++iter)
		{
			ShaderParameter parameter = iter->second;
			D3D11ConstantBufferPtr buffer_ = parameter.bufferPtr_;
			if (buffer_ != NULL)
			{
				buffer_->Apply();
			}
		}
	}
	
	g_objDeviceManager.GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->HSSetShader(m_pHullShader, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->DSSetShader(m_pDomainShader, NULL, 0);


	g_objDeviceManager.GetDeviceContext()->GSSetShader(m_pGeometryShader, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->CSSetShader(m_pComputeShader, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);
}

void D3D11Shader::ApplyGeometryShaderOutPut()
{
	for (int i = 0; i < PS; ++i)
	{
		HP_HashMap<unsigned, ShaderParameter>& pTmp = parametersArray[i];
		HP_HashMap<unsigned, ShaderParameter>::iterator iter = pTmp.begin();
		for (; iter != pTmp.end(); ++iter)
		{
			ShaderParameter parameter = iter->second;
			D3D11ConstantBufferPtr buffer_ = parameter.bufferPtr_;
			if (buffer_ != NULL)
			{
				buffer_->Apply();
			}
		}
	}
	g_objDeviceManager.GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->GSSetShader(m_pGeometryShaderOutPut, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->CSSetShader(NULL, NULL, 0);
	g_objDeviceManager.GetDeviceContext()->PSSetShader(NULL, NULL, 0);
}
