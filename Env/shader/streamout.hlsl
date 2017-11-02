cbuffer  MatrixBuffer: register(b0)
{
	matrix worldMatrix;
};

struct VertexInputType
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};

PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	// Store the input color for the pixel shader to use.
	output.color = input.color;
	output.tex = input.tex;
	return output;
}


float4 PS(PixelInputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 textureColor = input.color;
	return textureColor;
}