#include "Sprite.h"
#include "DeviceManager.h"
#include "D3D11Shader.h"





//获取绘制2D的矩阵
XMMATRIX GetShow2DMatrix(int nWidth, int nHegith)
{
	// Transform NDC space [-1,+1]^2 to screen space [0,1]^2
	XMMATRIX toTexSpace;
	toTexSpace.SetColumn(0,Vector4f( 0.5f * nWidth, 0.0f, 0.0f, 0.0f));
	toTexSpace.SetColumn(1, Vector4f(0.0f, -0.5f * nHegith, 0.0f, 0.0f));
	toTexSpace.SetColumn(2, Vector4f(0.0f, 0.0f, 1.0f, 0.0f));
	toTexSpace.SetColumn(3, Vector4f(0.5f * nWidth, 0.5f * nHegith, 0.0f, 1.0f));
	return toTexSpace.Invert_Full();
	//DX屏幕坐标以左上角为(0,0) 深度值(0.0f ,1.0f)
	//OPENGL 以右下角为(0,0)深度值(-1.0f - 1.0f)
}


Sprite& Sprite::GetInstance()
{
	static Sprite _instance;
	return _instance;
}

void Sprite::ResetSize(int nWidth, int nHeight)
{
	Matrix4x4f matrixScale;
	matrixScale.SetTranslate(Vector3f(100,200,300));
	matrixScale.SetColumn(0, Vector4(2, 2, 2, 2));
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

Sprite::Sprite(int initialBufferCapacity /*= 1024*/, int maxBufferSizePerPrimitive /*= 32768*/)
{
	
	m_indexBuffer = nullptr;
	m_pInputLayout = nullptr;

	m_initialBufferCapacity = initialBufferCapacity;
	m_maxBufferSizePerPrimitive = maxBufferSizePerPrimitive;

	BoneTransformsSRV = nullptr;
	m_BonesBuffer = nullptr;

	BoneTransformsStructSRV = nullptr;
	m_BonesStructBuffer = nullptr;

}
void Sprite::Release()
{
	SAFE_RELEASE(m_indexBuffer);
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(BoneTransformsSRV);
	SAFE_RELEASE(m_BonesBuffer);

	SAFE_RELEASE(BoneTransformsStructSRV);
	SAFE_RELEASE(m_BonesStructBuffer);
}


Sprite::~Sprite()
{
	Release();
}

void Sprite::InitRHI()
{

	CreateIndexBuffer(0, m_initialBufferCapacity);
	//CreateVertexBuffer(0, m_maxBufferSizePerPrimitive);


	D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC; //动态缓存
	int ByteWidth = sizeof(VertexPositionColorTexture)* m_maxBufferSizePerPrimitive; //缓冲区大小
	int CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU 可写
	VertexBufferPtr = D3D11VertexBuffer::Create(ByteWidth, Usage, CPUAccessFlags);
	VertexBufferPtr->CreateRHI();


	float x = (float)50;
	float y = (float)50;
	float h = (float)120;
	float w = (float)120;

	XMFLOAT4 LeftTop = XMFLOAT4(x, y, 0.0f, 0.0f);
	XMFLOAT4 RightTop = XMFLOAT4(x + w, y, 0.0f, 0.0f);

	XMFLOAT4 LeftBottom = XMFLOAT4(x, (y + h), 0.0f, 0.0f);
	XMFLOAT4 RightBottom = XMFLOAT4(x + w, (y + h), 0.0f, 0.0f);
	const XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1);
	VertexPositionColorTexture vertices[8] =
	{
		//正面的四个点
		{ LeftBottom, color, XMFLOAT2(0, 1) },
		{ LeftTop, color, XMFLOAT2(0, 0) },

		{ LeftTop, color, XMFLOAT2(0, 0) },
		{ RightTop, color, XMFLOAT2(1, 0) },

		{ RightTop, color, XMFLOAT2(1, 0) },
		{ RightBottom, color, XMFLOAT2(1, 1) },

		{ RightBottom, color, XMFLOAT2(1, 1) },
		{ LeftBottom, color, XMFLOAT2(0, 1) }
	};

	ByteWidth = sizeof(VertexPositionColorTexture) * 8; //缓冲区大小
	CPUAccessFlags = 0; //CPU 可写
	Usage = D3D11_USAGE_DEFAULT; //静态缓存 可以通过update更新
	StaticVertexBufferPtr = D3D11VertexBuffer::Create(ByteWidth, Usage, CPUAccessFlags, vertices);
	StaticVertexBufferPtr->CreateRHI();

	shaderPtr = std::make_shared<D3D11Shader>();
	shaderPtr->CompileFromFile("shader\\sprite.hlsl",VS);
	shaderPtr->CompileFromFile("shader\\sprite.hlsl",PS);
	shaderPtr->CompileFromFile("shader\\sprite.hlsl", GS);

	shaderPtr->CreateGPUBuffer(VS);
	shaderPtr->CreateGPUBuffer(PS);
	shaderPtr->CreateGPUBuffer(GS);

	shaderPtr->CreateInputLayout<VertexPositionColorTexture>(&m_pInputLayout);

	

/**********************************************************************************************************************************************************/

	//Buffer<float4> BoneTransforms :  register(t0);  //每个阶段都可以用
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = 4 * sizeof(Vector4); // 这边测试的是传入一个矩阵
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//因为要绑定到着色器中
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC; //动态缓存(用map unmap 方式去更新)
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU 可写
	bufferDesc.MiscFlags = 0; //Buffer resource view  （最普通的buffer视图）
	bufferDesc.StructureByteStride = 0;
	HRESULT hr = g_objDeviceManager.GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_BonesBuffer);

	//create SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC bufferSRVDesc = {};
	bufferSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// 每个的大小 这边 应该算是 float4
	bufferSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;//The resource is a buffer.
	bufferSRVDesc.Buffer.ElementOffset = 0;
	bufferSRVDesc.Buffer.ElementWidth = bufferDesc.ByteWidth/sizeof(Vector4);//元素数量 = 总大小/每个的大小

	hr = g_objDeviceManager.GetDevice()->CreateShaderResourceView(m_BonesBuffer, &bufferSRVDesc, &BoneTransformsSRV);

/**********************************************************************************************************************************************************/
/**********************************************************************************************************************************************************/

//StructuredBuffer<float4> BoneTransforms2  :register(t2);  //每个阶段都可以用
	D3D11_BUFFER_DESC structBufferDesc = {};
	structBufferDesc.ByteWidth = 4 * sizeof(Vector4); // 这边测试的是传入一个矩阵
	structBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//因为要绑定到着色器中
	structBufferDesc.Usage = D3D11_USAGE_DEFAULT; // 可以通过update更新
	structBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU 可写
	structBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; //结构体缓存视图
	structBufferDesc.StructureByteStride = sizeof(Vector4); //必须设置结构体大小
	hr = g_objDeviceManager.GetDevice()->CreateBuffer(&structBufferDesc, NULL, &m_BonesStructBuffer);

	//create SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC structBufferSRVDesc = {};
	structBufferSRVDesc.Format = DXGI_FORMAT_UNKNOWN;// 结构体大小在StructureByteStride中设置了，所以这边不允许设置
	structBufferSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;//The resource is a buffer.
	structBufferSRVDesc.Buffer.ElementOffset = 0;
	structBufferSRVDesc.Buffer.ElementWidth = bufferDesc.ByteWidth / sizeof(Vector4);//元素数量 = 总大小/每个的大小（StructureByteStride）

	hr = g_objDeviceManager.GetDevice()->CreateShaderResourceView(m_BonesStructBuffer, &structBufferSRVDesc, &BoneTransformsStructSRV);

/**********************************************************************************************************************************************************/

}

void Sprite::ShowRect(int x1, int y1, int x2, int y2, const XMFLOAT4& color /*= XMFLOAT4(1, 1, 1, 1)*/, float dt /*= 0*/)
{
	float x = (float)x1;
	float y = (float)y1;
	float h = (float)y2 - y1;
	float w = (float)x2 - x1;

	XMFLOAT4 LeftTop = XMFLOAT4(x, y, 0.0f, 0.0f);
	XMFLOAT4 RightTop = XMFLOAT4(x + w, y, 0.0f, 0.0f);

	XMFLOAT4 LeftBottom = XMFLOAT4(x, (y + h), 0.0f, 0.0f);
	XMFLOAT4 RightBottom = XMFLOAT4(x + w, (y + h), 0.0f, 0.0f);
	VertexPositionColorTexture vertices[8] =
	{
		//正面的四个点
		{ LeftBottom, color, XMFLOAT2(0, 1) },
		{ LeftTop, color, XMFLOAT2(0, 0) },

		{ LeftTop, color, XMFLOAT2(0, 0) },
		{ RightTop, color, XMFLOAT2(1, 0) },

		{ RightTop, color, XMFLOAT2(1, 0) },
		{ RightBottom, color, XMFLOAT2(1, 1) },

		{ RightBottom, color, XMFLOAT2(1, 1) },
		{ LeftBottom, color, XMFLOAT2(0, 1) }
	};

	XMMATRIX toTexSpace = GetShow2DMatrix(m_nWidth, m_nHeight);
	Quaternionf q = EulerToQuaternion(Vector3(0, 0, dt));
	Matrix4x4f rot;
	QuaternionToMatrix(q, rot);

	XMMATRIX toPos;
	XMMATRIX toPos1;

	Matrix4x4f ret;
	ret = toTexSpace*toPos1.SetTranslate(Vector3(x, y, 0))*rot*toPos.SetTranslate(Vector3(-x, -y, 0));

/*
	toPos.SetTranslate(Vector3(-x, -y, 0));
	MultiplyMatrices4x4(&rot, &toPos, &ret);//平移回原点 旋转
	toPos.SetTranslate(Vector3(x, y, 0));
	MultiplyMatrices4x4(&toPos, &ret, &ret1);//平移回原点
	MultiplyMatrices4x4(&toTexSpace, &ret1, &ret);//平移回原点*/
	


	DrawPrimitiveUP(PRIMITIVE_LINELIST, 8, vertices, ret, NULL, SpriteType::ONLY_COLOR);
}

void Sprite::CreateVertexBuffer(int nType, int nSize)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC; //动态缓存
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionColorTexture)* nSize; //缓冲区大小
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //CPU 可写
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	HRESULT result = S_FALSE;
	result = g_objDeviceManager.GetDevice()->CreateBuffer(&vertexBufferDesc, NULL, VertexBufferPtr->GetResourcePPtr());
}

void Sprite::CreateIndexBuffer(int nType, int nSize)
{
	D3D11_BUFFER_DESC  indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(UINT)* nSize;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	HRESULT result = S_FALSE;
	result = g_objDeviceManager.GetDevice()->CreateBuffer(&indexBufferDesc, NULL, &m_indexBuffer);
	if (FAILED(result))
	{
		return;
	}
}

void Sprite::DrawPrimitiveUP(PrimitiveType PrimitiveType, unsigned int PrimitiveCount, VertexPositionColorTexture *pVertexs, Matrix model /*= XMMatrixIdentity()*/, ID3D11ShaderResourceView*pTexture /*= NULL*/, SpriteType spriteType /*= COLOR_TEX*/)
{
	ID3D11DeviceContext* pDeviceContext = g_objDeviceManager.GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	pDeviceContext->Map(VertexBufferPtr->GetResource(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);
	//	Update the vertex buffer here.
	int nSize = PrimitiveCount;
	if (nSize > m_maxBufferSizePerPrimitive)
	{
		nSize = m_maxBufferSizePerPrimitive;
	}
	memcpy(mappedResource.pData, pVertexs, nSize * sizeof(VertexPositionColorTexture));
	//	Reenable GPU access to the vertex buffer data.
	pDeviceContext->Unmap(VertexBufferPtr->GetResource(), 0);

	unsigned int stride;
	unsigned int offset;
	stride = sizeof(VertexPositionColorTexture);
	offset = 0;

	pDeviceContext->IASetInputLayout(m_pInputLayout);
	pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)PrimitiveType);
	pDeviceContext->IASetVertexBuffers(0, 1, VertexBufferPtr->GetResourcePPtr(), &stride, &offset);
	shaderPtr->VSSetConstantBuffers("worldMatrix", &model);

	pDeviceContext->Map(m_BonesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy_s(mappedResource.pData, mappedResource.RowPitch,  &model, 1 * sizeof(Matrix));
	pDeviceContext->Unmap(m_BonesBuffer, 0);
	shaderPtr->VSSetShaderResources("BoneTransforms", BoneTransformsSRV);




	int ByteWidth = sizeof(Matrix) * 1; //缓冲区大小
	D3D11_BOX destRegion;
	destRegion.left = 0;
	destRegion.right = ByteWidth;
	destRegion.top = 0;
	destRegion.bottom = 1;
	destRegion.front = 0;
	destRegion.back = 1;

	pDeviceContext->UpdateSubresource(m_BonesStructBuffer, 0, &destRegion, &model, ByteWidth, 0);

	shaderPtr->VSSetShaderResources("BoneTransforms2", BoneTransformsStructSRV);
	int a = 10;
	if (m_bDebug)
	{
		a = 0;
	}
//	shaderPtr->GSSetConstantBuffers("testa", &a);


	if (pTexture)
	{
		
	}
	else
	{
		spriteType = ONLY_COLOR;
	}
   shaderPtr->PSSetConstantBuffers("SpriteType", &spriteType);
   XMFLOAT4 color = { 0.5f,0.5f,0.5f,0.5f };
   shaderPtr->PSSetConstantBuffers("defaultColor", &color);
   shaderPtr->Apply();


	pDeviceContext->Draw(PrimitiveCount, 0);
}


void Sprite::ShowRectTest()
{
	ID3D11DeviceContext* pDeviceContext = g_objDeviceManager.GetDeviceContext();


	unsigned int stride;
	unsigned int offset;
	stride = sizeof(VertexPositionColorTexture);
	offset = 0;
	XMMATRIX toTexSpace = GetShow2DMatrix(m_nWidth, m_nHeight);

	pDeviceContext->IASetInputLayout(m_pInputLayout);
	pDeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pDeviceContext->IASetVertexBuffers(0, 1, StaticVertexBufferPtr->GetResourcePPtr(), &stride, &offset);
	shaderPtr->VSSetConstantBuffers("worldMatrix", &toTexSpace);
	int spriteType = COLOR_TEXT;
	shaderPtr->PSSetConstantBuffers("SpriteType", &spriteType);
	shaderPtr->Apply();


	pDeviceContext->Draw(8, 0);
}

void Sprite::ShowRectTest(int x1, int y1, int x2, int y2, const XMFLOAT4& color /*= XMFLOAT4(1, 1, 1, 1)*/, float dt /*= 0*/)
{
	float x = (float)x1;
	float y = (float)y1;
	float h = (float)y2 - y1;
	float w = (float)x2 - x1;

	XMFLOAT4 LeftTop = XMFLOAT4(x, y, 0.0f, 0.0f);
	XMFLOAT4 RightTop = XMFLOAT4(x + w, y, 0.0f, 0.0f);

	XMFLOAT4 LeftBottom = XMFLOAT4(x, (y + h), 0.0f, 0.0f);
	XMFLOAT4 RightBottom = XMFLOAT4(x + w, (y + h), 0.0f, 0.0f);
	VertexPositionColorTexture vertices[8] =
	{
		//正面的四个点
		{ LeftBottom, color, XMFLOAT2(0, 1) },
		{ LeftTop, color, XMFLOAT2(0, 0) },

		{ LeftTop, color, XMFLOAT2(0, 0) },
		{ RightTop, color, XMFLOAT2(1, 0) },

		{ RightTop, color, XMFLOAT2(1, 0) },
		{ RightBottom, color, XMFLOAT2(1, 1) },

		{ RightBottom, color, XMFLOAT2(1, 1) },
		{ LeftBottom, color, XMFLOAT2(0, 1) }
	};

	XMMATRIX toTexSpace = GetShow2DMatrix(m_nWidth, m_nHeight);
	toTexSpace =  toTexSpace;
	ID3D11DeviceContext* pDeviceContext = g_objDeviceManager.GetDeviceContext();
	int ByteWidth = sizeof(VertexPositionColorTexture) * 8; //缓冲区大小
	D3D11_BOX destRegion;
	destRegion.left = 0;
	destRegion.right =  ByteWidth;
	destRegion.top = 0;
	destRegion.bottom = 1;
	destRegion.front = 0;
	destRegion.back = 1;
	
	pDeviceContext->UpdateSubresource(StaticVertexBufferPtr->GetResource(), 0, &destRegion, vertices, ByteWidth, 0);


	ShowRectTest();
}
