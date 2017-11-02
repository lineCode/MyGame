cbuffer  MatrixBuffer: register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};
cbuffer  MyDefine: register(b1)
{
	uint spriteType; //0�??有�?�色,1�??有纹理，2纹理加�?�色
	uint bShow3D;
}

cbuffer  MyGS: register(b1)
{
	uint testa; 
}


Buffer<float4> BoneTransforms :  register(t0);
//Texture2D BoneTransforms :  register(t0);
Texture2D BoneTransforms1 :  register(t1);

struct GridPoint
{
	float4 row;
};
StructuredBuffer<float4> BoneTransforms2  :register(t2);

struct VertexInputType
{
	float4 position : SV_Position;
	uint id : SV_VertexID;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};
float4 defaultColor;

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	uint id = input.id;
	output.tex = float2((id << 1) & 2, id & 2);
	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	uint  dim = -1;
	BoneTransforms.GetDimensions( dim);
	if (dim == 41)
	{
		int  iBone = 0;
		float4 row1 = BoneTransforms.Load(iBone);
		float4 row2 = BoneTransforms.Load(iBone + 1);
		float4 row3 = BoneTransforms.Load(iBone + 2);
		float4 row4 = BoneTransforms.Load(iBone + 3);
		matrix	mm = float4x4(row1, row2, row3, row4);

		output.position = mul(input.position, mm);
	}
	else
	{
		float4 row1 = BoneTransforms2[0];
		float4 row2 = BoneTransforms2[1];
		float4 row3 = BoneTransforms2[2];
		float4 row4 = BoneTransforms2[3];
		matrix	mm = float4x4(row1, row2, row3, row4);

		output.position = mul(input.position, mm);
	}

	if(bShow3D)
	{
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);
	}

	// Store the input color for the pixel shader to use.
	output.color = input.color;
	//output.tex = input.tex;
	return output;
}

struct GeoOut
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};


// The draw GS just expands points into lines.
[maxvertexcount(100)]
void GSMain(line  PixelInputType gin[2],
	inout LineStream<GeoOut> output)
{
	float width = (gin[1].position.x - gin[0].position.x);
	float height = (gin[1].position.y - gin[0].position.y);

	GeoOut V0;
	int ntmp = 0;
	int tmp = testa;
	
	for (int i = 0; i < 100; ++i)
	{
		V0.position = (gin[0].position);
		V0.position.x += i* width / 100.0f;
		V0.position.y += i* height / 100.0f;
		ntmp++;
		if (ntmp % 2 == 0)
		{
			V0.color = float4(0, 0, 0, 0);
		}
		else
		{
			V0.color = gin[0].color;
		}
		V0.tex = gin[0].tex;
		output.Append(V0);
	}
}

Texture2D shaderTexture: register(t0);
SamplerState SampleType: register(s0);
float4 PS(PixelInputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 textureColor =  input.color;
	return textureColor;
	if(spriteType == 0)
	{
		return textureColor* defaultColor;
	}
	else if(spriteType == 1)
	{
		float4 diffColor = shaderTexture.Sample(SampleType, input.tex);
		return diffColor;
	}
	else if(spriteType == 2)
	{
		float4 diffColor = shaderTexture.Sample(SampleType, input.tex);
		return diffColor * textureColor;
	}
	return textureColor;
}