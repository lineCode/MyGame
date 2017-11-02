
struct VertexInputType
{
	float3 position : POSITION;
    float4 color : COLOR;
	uint id : SV_VertexID;
};

struct PixelInputType
{
	float4 position : Position;
};


PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	// Change the position vector to be 4 units for proper matrix calculations.
	output.position = input.id;

	return output;
}