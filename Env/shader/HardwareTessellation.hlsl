
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
	uint id : SV_VertexID; //hlsl原语，放哪里都可以
};



struct HullInputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};

//////////////
// TYPEDEFS //
//: 在该阶段需要确定一些细分因子
//如细分三角形时，输出结构体为：
//////////////
struct ConstantOutputType  
{
	//由于三角形有三条边，所以需要三个SV_TessFactor字段的元素用于分别指定三条边的细分因子
    float edges[3] : SV_TessFactor; //字段用于指定如何细分边(边细分分多少部分)
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
[domain("tri")] //指定图元类型为三角形
[partitioning("integer")]//指定细分模式
[outputtopology("triangle_cw")]//指定图元拓扑结构为顺时针三角形
[outputcontrolpoints(3)]//指定输出控制点个数，也决定了该函数将执行几次
[patchconstantfunc("ColorPatchConstantFunction")]//指定patch constant函数(hs函数执行几次以后，会调用一次)


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