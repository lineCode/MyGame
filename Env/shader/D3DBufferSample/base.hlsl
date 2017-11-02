cbuffer  MatrixBuffer: register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


struct VertexInputType
{
	float3 position : POSITION;
    float4 color : COLOR;
	uint id : SV_VertexID;
};

struct PixelInputType
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
    output.position.xyz =  input.position;
	// Change the position vector to be 4 units for proper matrix calculations.
	output.position.w = 1.0f;
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.color = input.color;
	return output;
}

float4 PS(PixelInputType input) : SV_TARGET
{
    return input.color;
}