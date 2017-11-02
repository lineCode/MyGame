
struct VertexInputType
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
	uint id : SV_VertexID;
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
	output.position = input.position;
	// Store the input color for the pixel shader to use.
	output.color = input.color;
	output.tex = input.tex;
	return output;
}