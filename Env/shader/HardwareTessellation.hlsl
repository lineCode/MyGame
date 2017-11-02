
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float3 position : POSITION;
    float4 color : COLOR;
	uint id : SV_VertexID; //hlslԭ������ﶼ����
};



struct HullInputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};

//////////////
// TYPEDEFS //
//: �ڸý׶���Ҫȷ��һЩϸ������
//��ϸ��������ʱ������ṹ��Ϊ��
//////////////
struct ConstantOutputType  
{
	//�����������������ߣ�������Ҫ����SV_TessFactor�ֶε�Ԫ�����ڷֱ�ָ�������ߵ�ϸ������
    float edges[3] : SV_TessFactor; //�ֶ�����ָ�����ϸ�ֱ�(��ϸ�ֶַ��ٲ���)
    float inside : SV_InsideTessFactor;
};

struct HullOutputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

/////////////
// GLOBALS //
/////////////
cbuffer TessellationBuffer
{
    float tessellationAmount;
    float3 padding;
};


////////////////////////////////////////////////////////////////////////////////
// Patch Constant Function
////////////////////////////////////////////////////////////////////////////////
ConstantOutputType ColorPatchConstantFunction(InputPatch<HullInputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{    
	float tessellationAmountTmp = tessellationAmount;
	tessellationAmountTmp =5;
	ConstantOutputType output;
	if (patchId == 0)
	{
		// Set the tessellation factors for the three edges of the triangle.
		output.edges[0] = tessellationAmountTmp;
		output.edges[1] = tessellationAmountTmp;
		output.edges[2] = tessellationAmountTmp;
		// Set the tessellation factor for tessallating inside the triangle.
		output.inside = tessellationAmountTmp;// tessellationAmount;
	}
	else
	{
		// Set the tessellation factors for the three edges of the triangle.
		output.edges[0] = 1;
		output.edges[1] = 1;
		output.edges[2] = 1;
		// Set the tessellation factor for tessallating inside the triangle.
		output.inside = 0;
	}

	return output;
}



////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

HullInputType VS1(VertexInputType input)
{
    HullInputType output;

	// Pass the vertex position into the hull shader.
    output.position = input.position;
    
	// Pass the input color into the hull shader.
    output.color = input.color;
    
    return output;
}


HullInputType VS(uint id : SV_VertexID)
{
	HullInputType output;
	const  float3 vPos0 = { -1.0f, -1.0f, 0.0f };
	const  float3 vPos1 = { 0.0f, 1.0f, 0.0f };
	const  float3 vPos2 = { 1.0f, -1.0f, 0.0f };

	const  float3 vPos3 = { 1.0f, -1.0f, 0.0f };
	const  float3 vPos4 = { 2.0f, 1.0f, 0.0f };
	const  float3 vPos5 = { 3.0f, -1.0f, 0.0f };

	const float3 vertices[6] = { vPos0,vPos1,vPos2,vPos3,vPos4,vPos5 };
	output.position = vertices[id];
	if (id % 2 == 0)
	{
		output.color = float4(1, 0, 0, 1);
	}
	else
	{
		output.color = float4(0, 1, 0, 1);
	}
	return output;
}



////////////////////////////////////////////////////////////////////////////////
// Hull Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")] //ָ��ͼԪ����Ϊ������
[partitioning("integer")]//ָ��ϸ��ģʽ
[outputtopology("triangle_cw")]//ָ��ͼԪ���˽ṹΪ˳ʱ��������
[outputcontrolpoints(3)]//ָ��������Ƶ������Ҳ�����˸ú�����ִ�м���
[patchconstantfunc("ColorPatchConstantFunction")]//ָ��patch constant����(hs����ִ�м����Ժ󣬻����һ��)


HullOutputType HS(InputPatch<HullInputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HullOutputType output;


	// Set the position for this control point as the output position.
    output.position = patch[pointId].position;

	// Set the input color as the output color.
    output.color = patch[pointId].color;

    return output;
}


////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]

PixelInputType DS(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, 3> patch)
{
	float3 vertexPosition;
	PixelInputType output;
 

 	// Determine the position of the new vertex.
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;


	// Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.color = uvwCoord.x * patch[0].color + uvwCoord.y * patch[1].color + uvwCoord.z * patch[2].color;

	// Send the input color into the pixel shader.
	//output.color = patch[0].color;

    return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS(PixelInputType input) : SV_TARGET
{
    return input.color;
}