#include "D3DBufferSample.h"
#include "DeviceManager.h"
#include "vaDirectXTools.h"
#include "VertexType.h"
#include "D3D11Shader.h"
#include "DirectXHelp.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"

typedef  Vector4f Vector4;
typedef  Vector3f Vector3;
typedef  Vector2f Vector2;



#define THREAD_GRID_SIZE_X 4
#define THREAD_GRID_SIZE_Y 4

#define  THREAD_GROUPS_X 4
#define  THREAD_GROUPS_Y 4

D3DBufferSample::D3DBufferSample()
{
	BoneTransformsSRV = NULL;
	m_BonesBuffer = NULL;
	BoneTransformsStructSRV = NULL;
	m_BonesStructBuffer = NULL;
	m_StreamOutVB = NULL;
}

void D3DBufferSample::Release()
{
	SAFE_RELEASE(BoneTransformsSRV);
	SAFE_RELEASE(m_BonesBuffer);
	SAFE_RELEASE(BoneTransformsStructSRV);
	SAFE_RELEASE(m_BonesStructBuffer);
	SAFE_RELEASE(m_StreamOutVB);
}


D3DBufferSample::~D3DBufferSample()
{
	Release();
}

void D3DBufferSample::InitRHI()
{
	HRESULT hr = S_OK;

	/************************************************************************/
	/* 常量缓存          BindFlags 不能和其他的共用                                                           */
	/************************************************************************/
	int size_ = 100;
	size_ += 15;
	size_ &= 0xfffffff0;
	ID3D11Buffer* constantBuffer = NULL;
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = 200 * sizeof(VertexPositionColorTexture);
	constantBufferDesc.CPUAccessFlags = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_STREAM_OUTPUT | D3D11_BIND_VERTEX_BUFFER;
	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&constantBufferDesc, NULL, &constantBuffer);
	vaDirectXTools::NameObject(constantBuffer, "constantBuffer");
	SAFE_RELEASE(constantBuffer);

	/************************************************************************/
	/*            Buffer                                                          */
	/************************************************************************/
	//Buffer<float4> BoneTransforms :  register(t0);  //每个阶段都可以用
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = 4 * sizeof(Vector4); // 这边测试的是传入一个矩阵
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//因为要绑定到着色器中
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //动态缓存(用map unmap 方式去更新)
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU 可写
	bufferDesc.MiscFlags = 0; //Buffer resource view  （最普通的buffer视图）
	bufferDesc.StructureByteStride = 0;
	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_BonesBuffer);
	vaDirectXTools::NameObject(m_BonesBuffer, "m_BonesBuffer");

	//create SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC bufferSRVDesc = {};
	bufferSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// 每个的大小 这边 应该算是 float4
	bufferSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;//The resource is a buffer.
	bufferSRVDesc.Buffer.ElementOffset = 0;
	bufferSRVDesc.Buffer.ElementWidth = bufferDesc.ByteWidth / sizeof(Vector4);//元素数量 = 总大小/每个的大小

	hr = g_objDeviceManager.GetDevice()->CreateShaderResourceView(m_BonesBuffer, &bufferSRVDesc, &BoneTransformsSRV);
	vaDirectXTools::NameObject(BoneTransformsSRV, "BoneTransformsSRV");

	/************************************************************************/
	/*          StructuredBuffer                                                            */
	/************************************************************************/
	//StructuredBuffer<float4> BoneTransforms2  :register(t2);  //每个阶段都可以用
	D3D11_BUFFER_DESC structBufferDesc = {};
	structBufferDesc.ByteWidth = 4 * sizeof(Vector4); // 这边测试的是传入一个矩阵
	structBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//因为要绑定到着色器中
	structBufferDesc.Usage = D3D11_USAGE_DEFAULT; // 可以通过update更新
	structBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU 可写
	structBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; //结构体缓存视图
	structBufferDesc.StructureByteStride = sizeof(Vector4); //必须设置结构体大小
	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&structBufferDesc, NULL, &m_BonesStructBuffer);
	vaDirectXTools::NameObject(m_BonesStructBuffer, "m_BonesStructBuffer");

	//create SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC structBufferSRVDesc = {};
	structBufferSRVDesc.Format = DXGI_FORMAT_UNKNOWN;// 结构体大小在StructureByteStride中设置了，所以这边不允许设置
	structBufferSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;//The resource is a buffer.
	structBufferSRVDesc.Buffer.ElementOffset = 0;
	structBufferSRVDesc.Buffer.ElementWidth = bufferDesc.ByteWidth / sizeof(Vector4);//元素数量 = 总大小/每个的大小（StructureByteStride）

	hr = g_objDeviceManager.GetDevice()->CreateShaderResourceView(m_BonesStructBuffer, &structBufferSRVDesc, &BoneTransformsStructSRV);
	vaDirectXTools::NameObject(BoneTransformsStructSRV, "BoneTransformsStructSRV");
	/**********************************************************************************************************************************************************/


	/************************************************************************/
	/* 流输出                                                                     */
	/************************************************************************/
	D3D11_BUFFER_DESC outputbufferDesc = {};
	outputbufferDesc.ByteWidth = 200 * sizeof(VertexPositionColorTexture);
	outputbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	outputbufferDesc.BindFlags = D3D11_BIND_STREAM_OUTPUT | D3D11_BIND_VERTEX_BUFFER;
	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&outputbufferDesc, NULL, &m_StreamOutVB);
	vaDirectXTools::NameObject(m_StreamOutVB, "m_StreamOutVB");


	outputbufferDesc.ByteWidth = 3 * sizeof(Vector4);
	outputbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	outputbufferDesc.BindFlags = D3D11_BIND_STREAM_OUTPUT | D3D11_BIND_INDEX_BUFFER;
	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&outputbufferDesc, NULL, &m_StreamOutIB);
	vaDirectXTools::NameObject(m_StreamOutIB, "m_StreamOutIB");

	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&outputbufferDesc, NULL, &m_StreamOutIBTmp);
	vaDirectXTools::NameObject(m_StreamOutIBTmp, "m_StreamOutIBTmp");

	outputbufferDesc.Usage = D3D11_USAGE_STAGING;
	outputbufferDesc.BindFlags = 0;
	outputbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&outputbufferDesc, NULL, &m_StreamOutIBTmp1);
	vaDirectXTools::NameObject(m_StreamOutIBTmp1, "m_StreamOutIBTmp1");

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT; //动态缓存
	int ByteWidth = sizeof(VertexPositionColor) * 7; //缓冲区大小
	int CPUAccessFlags = 0;

	Vector3 vPos0(-3.0f, -1.0f, 0.0f);
	Vector3 vPos1(-2.0f, 1.0f, 0.0f);
	Vector3 vPos2(-1.0f, -1.0f, 0.0f);


	Vector3 vPos3(0.0f, 1.0f, 0.0f);
	Vector3 vPos4(1.0f, -1.0f, 0.0f);

	Vector3 vPos5(-2.0f, -3.0f, 0.0f);
	Vector3 vPos6(0.0f, -3.0f, 0.0f);

	Vector4f color = Vector4f(0, 1, 0, 1);
	Vector4f color1 = Vector4f(1, 0, 0, 1);

	VertexPositionColor vertices[] =
	{
		{ vPos0, color },
		{ vPos1, color },
		{ vPos2, color },
		{ vPos3, color },
		{ vPos4, color },
		{ vPos5, color1 },
		{ vPos6, color1 }
	};

	VertexBufferPtr = D3D11VertexBuffer::Create(ByteWidth, Usage, CPUAccessFlags, vertices);
	VertexBufferPtr->CreateRHI();
	vaDirectXTools::NameObject(VertexBufferPtr->GetResource(), "VertexBufferPtr");
	InitShader();
	CreateIndexBuffer(12);

	D3D11_BUFFER_DESC sbDesc;
	sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	sbDesc.CPUAccessFlags = 0;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sbDesc.StructureByteStride = sizeof(BufferStruct);
	sbDesc.ByteWidth = sizeof(BufferStruct) * THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;
	sbDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&sbDesc, NULL, &m_StructBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
	sbUAVDesc.Buffer.FirstElement = 0;
	sbUAVDesc.Buffer.Flags = 0;
	sbUAVDesc.Buffer.NumElements = THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;
	sbUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	hr = g_objDeviceManager.GetDevice()->CreateUnorderedAccessView(m_StructBuffer, &sbUAVDesc, &m_pStructuredBufferUAV);


	//create SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC sbSRVDesc = {};
	sbSRVDesc.Format = DXGI_FORMAT_UNKNOWN;// 结构体大小在StructureByteStride中设置了，所以这边不允许设置
	sbSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;//The resource is a buffer.
	sbSRVDesc.Buffer.ElementOffset = 0;
	sbSRVDesc.Buffer.ElementWidth = THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;//元素数量 = 总大小/每个的大小（StructureByteStride）

	hr = g_objDeviceManager.GetDevice()->CreateShaderResourceView(m_StructBuffer, &sbSRVDesc, &m_pStructuredBufferSRV);


}

void D3DBufferSample::InitShader()
{
	shaderPtr0 = std::make_shared<D3D11Shader>();
	shaderPtr1 = std::make_shared<D3D11Shader>();
	shaderPtr2 = std::make_shared< D3D11Shader>();

	shaderPtr0->CompileFromFile("shader\\D3DBufferSample\\streamoutput.hlsl", VS);
	shaderPtr0->CreateGPUBuffer(VS);
	//shaderPtr0->CreateInputLayout<VertexPositionColor>(&m_pInputLayout);

	D3D11_SO_DECLARATION_ENTRY pDecl[] =
	{
		// semantic name, semantic index, start component, component count, output slot
		{ 0,"Position", 0, 2, 1, 0 }
	};
	//可以直接绑定顶点着色器输出
	//也可以绑定到几何着色器输出
	shaderPtr0->CreateGeometryShaderWithStreamOutput(pDecl, 1, D3D11_SO_NO_RASTERIZED_STREAM);

	shaderPtr1->CompileFromFile("shader\\D3DBufferSample\\base.hlsl", VS);
	shaderPtr1->CreateGPUBuffer(VS);
	shaderPtr1->CompileFromFile("shader\\D3DBufferSample\\base.hlsl", PS);
	shaderPtr1->CreateGPUBuffer(PS);
	shaderPtr1->CreateInputLayout<VertexPositionColor>(&m_pInputLayout);

	shaderPtr2->CompileFromFile("shader\\csbase.hlsl", CS);
	shaderPtr2->CreateGPUBuffer(CS);
	
}

void D3DBufferSample::OnRenderBefore()
{
	ID3D11DeviceContext* pDeviceContext = g_objDeviceManager.GetDeviceContext();


	unsigned int stride;
	unsigned int offset;
	stride = sizeof(VertexPositionColor);
	offset = 0;

	pDeviceContext->IASetInputLayout(m_pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	ID3D11Buffer* bufferArray[1] = { 0 };
	pDeviceContext->IASetVertexBuffers(0, 1, bufferArray, &stride, &offset);


	shaderPtr0->ApplyGeometryShaderOutPut();
	pDeviceContext->SOSetTargets(1, &m_StreamOutIB, &offset);
	pDeviceContext->RSSetState(g_RS_CullNone_Wireframe);
	pDeviceContext->Draw(12, 0);
	// done streaming out--unbind the vertex buffer
	ID3D11Buffer* bufferArray1[1] = { 0 };
	pDeviceContext->SOSetTargets(1, bufferArray, &offset);
	std::swap(m_StreamOutIB, m_StreamOutIBTmp);
	pDeviceContext->CopyResource(m_StreamOutIBTmp1, m_StreamOutIBTmp);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pDeviceContext->Map(m_StreamOutIBTmp1, 0, D3D11_MAP_READ, 0, &mappedResource);
	std::vector<float> list;
	list.resize(12);
	float* tmp = (float*)mappedResource.pData;
	memcpy_s(&list[0], 12* sizeof(float), mappedResource.pData, 12*sizeof(float));
	pDeviceContext->Unmap(m_StreamOutIBTmp1,0);

	std::vector<UINT> ulist;
	for (int i =0;i < list.size();++i)
	{
		ulist.push_back(list[i]);
	}

	//pDeviceContext->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//memcpy_s(mappedResource.pData, mappedResource.RowPitch, &ulist[0], mappedResource.RowPitch);
	//pDeviceContext->Unmap(m_indexBuffer, 0);


	
}

void D3DBufferSample::OnRender()
{
	ID3D11DeviceContext* pDeviceContext = g_objDeviceManager.GetDeviceContext();


	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexPositionColor);
	offset = 0;
	Matrix4x4f world = Matrix4x4f::identity;
	Matrix4x4f view;
	Matrix3x3f view3;

	Matrix4x4f proj;

/*
	XMVECTOR EyePosition = XMLoadFloat4(&XMFLOAT4(0, 0, 0, 0));
	XMVECTOR EyeDirection = XMLoadFloat4(&XMFLOAT4(0, 0, 1, 0));
	XMVECTOR UpDirection = XMLoadFloat4(&XMFLOAT4(0, 1, 0, 0));

	view = XMMatrixLookToLH(EyePosition, EyeDirection, UpDirection);
	proj = Matrix::CreatePerspective(1280, 768, 0.0f, 1000.0f);
	proj = XMMatrixPerspectiveFovLH(XM_PIDIV2 / 2.0f, 1280.0f / (FLOAT)768.0f, 0.01f, 1000.0f);*/

	LookRotationToMatrixLeftHanded(Vector3(0,0,1), Vector3f(0,1,0), &view3);

	
	proj.SetPerspective(45, 1280.0f / (FLOAT)768.0f, 0.01f, 1000.0f);
	pDeviceContext->IASetInputLayout(m_pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->IASetVertexBuffers(0, 1, VertexBufferPtr->GetResourcePPtr(), &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//world.Transpose();
	//view.Transpose();
	//proj.Transpose();


	shaderPtr1->VSSetConstantBuffers("worldMatrix", &world);
	shaderPtr1->VSSetConstantBuffers("viewMatrix", &view);
	shaderPtr1->VSSetConstantBuffers("projectionMatrix", &proj);
	shaderPtr1->Apply();
	pDeviceContext->DrawIndexed(12, 0,0);

	OnRenderCS();
}

void D3DBufferSample::CreateIndexBuffer(int nSize)
{
	D3D11_BUFFER_DESC  indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(INT32)* nSize;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	INT32 list[] =
	{
		0,1,2,0,5,2,
		2,3,4,2,4,6
	};
	HRESULT result = S_FALSE;
	D3D11_SUBRESOURCE_DATA mInitData;
	mInitData.pSysMem = list;
	mInitData.SysMemPitch = 0;//2d 3d 纹理的才有意义
	mInitData.SysMemSlicePitch = 0;//2d 3d 纹理的才有意义

	result = g_objDeviceManager.GetDevice()->CreateBuffer(&indexBufferDesc, &mInitData, &m_indexBuffer);
	if (FAILED(result))
	{
		return;
	}
}

void D3DBufferSample::OnRenderCS()
{
	ID3D11DeviceContext* pDeviceContext = g_objDeviceManager.GetDeviceContext();
	UINT initCounts = 0;
	shaderPtr2->CSSetUnorderedAccessViews("g_OutBuff", m_pStructuredBufferUAV, &initCounts);
	shaderPtr2->Apply();
	pDeviceContext->Dispatch(THREAD_GROUPS_X, THREAD_GROUPS_Y, 1);
	ID3D11UnorderedAccessView* pNullUAV = NULL;
	shaderPtr2->CSSetUnorderedAccessViews("g_OutBuff", pNullUAV, &initCounts);
}
