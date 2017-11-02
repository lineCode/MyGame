#pragma once

#include "dxbase.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "D3D11Buffer.h"




struct VertexPositionColorTexture
{
	Vector4f position;
	Vector4f color;
	Vector2f textureCoordinate;

	static const int InputElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

struct VertexPositionColor
{
	Vector3f position;
	Vector4f color;
	static const int InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};