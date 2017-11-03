#pragma once
#include "dxbase.h"

#include "D3D11Buffer.h"
#include "VertexType.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"

typedef  Vector4f Vector4;
typedef  Vector3f Vector3;
typedef  Vector2f Vector2;
typedef Matrix4x4f Matrix;

typedef Matrix4x4f XMMATRIX;

typedef  Vector4f XMFLOAT4;
typedef  Vector3f XMFLOAT3;
typedef  Vector2f XMFLOAT2;

/************************************************************************/
/* 2D绘制精灵                                                                     */
/************************************************************************/


enum PrimitiveType
{
	PRIMITIVE_UNDEFINED = 0,
	PRIMITIVE_POINTLIST = 1,
	PRIMITIVE_LINELIST = 2,
	PRIMITIVE_LINESTRIP = 3,
	PRIMITIVE_TRIANGLELIST = 4,
	PRIMITIVE_TRIANGLESTRIP = 5,
};

enum SpriteType
{
	ONLY_COLOR = 0,
	ONLY_TEX = 1,
	COLOR_TEX = 2,
	COLOR_TEXT = 3

};


class Sprite
{
public:
	static Sprite& GetInstance();
	//必须设置画布大小
	void ResetSize(int nWidth, int nHeight);
public:
	Sprite(int initialBufferCapacity = 1024, int maxBufferSizePerPrimitive = 32768);
	~Sprite();
	void InitRHI();
	void Release();

	void ShowRect(int x1, int y1, int x2, int y2, const XMFLOAT4& color = XMFLOAT4(1, 1, 1, 1), float dt = 0);
	void ShowRectTest(int x1, int y1, int x2, int y2, const XMFLOAT4& color = XMFLOAT4(1, 1, 1, 1), float dt = 0);


	void ShowRectTest();

	void SetDebug(bool btest) { m_bDebug = btest; }

	XMMATRIX GetShow2DMatrix();

public:
	void DrawPrimitiveUP(PrimitiveType PrimitiveType, unsigned int PrimitiveCount,
		VertexPositionColorTexture *pVertexs, Matrix model = Matrix::identity,
		ID3D11ShaderResourceView*pTexture = NULL, SpriteType spriteType = COLOR_TEX);

protected:
	void CreateVertexBuffer(int nType, int nSize);
	void CreateIndexBuffer(int nType, int nSize);

private:
	int m_maxBufferSizePerPrimitive;
	int m_initialBufferCapacity;
	int					m_nWidth;
	int					m_nHeight;

private:
	//ID3D11Buffer *m_vertexBuffer;
	D3D11VertexBufferPtr VertexBufferPtr;
	D3D11VertexBufferPtr StaticVertexBufferPtr;

	ID3D11Buffer	*m_indexBuffer;
	ID3D11InputLayout * m_pInputLayout;
	 std::shared_ptr<class D3D11Shader> shaderPtr;

	 ID3D11ShaderResourceView* BoneTransformsSRV;
	 ID3D11Buffer* m_BonesBuffer;

	 ID3D11ShaderResourceView* BoneTransformsStructSRV;
	 ID3D11Buffer* m_BonesStructBuffer;

	 bool m_bDebug;
};

#define g_objSprite Sprite::GetInstance()
